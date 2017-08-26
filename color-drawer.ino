// Moon Fairies for Circuit Playground

#include <Adafruit_CircuitPlayground.h>
#include <Wire.h>
#include <SPI.h>

// enable this for serial debugging (disable to run without a computer connected)
//#define DEBUG        Serial

// enable this to run LEDs in "calligraphic" mode in angled diagonal pair
#define GRAVITY

// enable this to run "test" sequence of possible color values
//#define TEST

// pin for capacitive sensing "LED on" state
#define CAP_ON         6

// pin for capactive sensing "next LED color"
#define CAP_COLOR      9

#define CAP_TIMEOUT   10000
#define CAP_AUTOCAL   1000000

// how many capacitive sensor samples to take/average
#define NSAMPLES       3

// number of Neopixel LEDs
#define NPIXELS       10

// adjust this to desired brightness
#define PIXEL_BRIGHT  80  // LED brightness

// adjust this for reliable sensing with whatever wiring and pads you're using
#define MIN_CAP_ON       10  // minimum capacitance reading for touch sensing
#define MIN_CAP_COLOR    25

#define LED_POSITIONS 12
// trig functions give their results in radians
// this converts radians to one of twelve LED positions
#define LED_DIV ((LED_POSITIONS * PI) / 180)

#define LED_OFFSET     2  // how many LEDs to advance from vertical to give desired visual angle

// time before accepting another LED color change (to avoid double triggering)
#define MIN_CHANGE    1000

// special "color" value to indicate end of color sequence
#define END           0x01000000

#ifdef TEST
// LED bit pattern test (useful for debugging/testing)
static uint32_t  tsequence[] = {
  0x00000001,
  0x00000010,
  0x00000100,
  0x00001000,
  0x00010000,
  0x00100000,
  0x01000000,
  0x10000000,
  0x04040404,
  0x04040400,
  0x00040404,
  END       // end of sequence
};
#else
// sequence of colors to use
static uint32_t  sequence[] = {
  0xff0000, // red (A)
  0xff00ff, // purple (d)
  0x0000ff, // blue (a)
  0x00ff00, // green (F)
  0xffff00, // yellow (r)
  0xff8000, // orange (u)
  0xff0000, // red (i)
  0x00ffff, // cyan (t)
  0xffffff, // white (flower)
  END       // end of sequence
};
#endif

static int                    colorindex;     // index of current color in array
static byte                   lastled;        // last LED lit
static boolean                last;           // last color change button state
static boolean                lastslide;      // last slide switch state
static uint32_t               curcolor;       // current color value
static uint32_t               lastsetcolor;   // last color valuee
static unsigned long          nextchangetime; // timestamp when next color change can happen
static CPlay_CapacitiveSensor cap_on;         // "on" capacitive sensor
static CPlay_CapacitiveSensor cap_color;      // "color" capacitive sensor


// initialization function

void
setup()
{
#ifdef DEBUG
  while (!DEBUG) {
  }

  DEBUG.begin(9600);
  DEBUG.println("Circuit Playground color drawer");
#endif

  colorindex     = 0;                     // start at beginning of color array
  curcolor       = sequence[colorindex];  // compute current color
  lastsetcolor   = 0;                     // previous color
  last           = true;                  // require a cycle before color change
  nextchangetime = 0;                     // next (first) change is valid any time
  lastled        = 0;
  cap_on         = CircuitPlayground.cap[CAP_ON   ];
  cap_color      = CircuitPlayground.cap[CAP_COLOR];

  CircuitPlayground.begin();

  CircuitPlayground.redLED(LOW);

  CircuitPlayground.setBrightness(PIXEL_BRIGHT);

  //cap_on.set_CS_Timeout_Millis(CAP_TIMEOUT);
  //cap_color.set_CS_Timeout_Millis(CAP_TIMEOUT);
  cap_on.set_CS_AutocaL_Millis(CAP_AUTOCAL);
  cap_color.set_CS_AutocaL_Millis(CAP_AUTOCAL);
}

// operation function, runs repeatedly

void
loop()
{
#ifdef GRAVITY
  int           led;      // bottom LED
  float         x;        // X acceleration
  float         y;        // Y acceleration
  float         theta;    // angle
#else
  byte          pixel;    // neopixel counter
#endif

  boolean       state;    // color change switch state
  boolean       slide;    // slide switch state
  uint16_t      cap;   // capacitance
  uint32_t      setcolor; // color to set LEDs to
  unsigned long now;      // current time

  slide = CircuitPlayground.slideSwitch();
  
  if (slide && !lastslide) {
    // slide switch on left, reset
    CircuitPlayground.redLED(HIGH);       // light LED for reset feedback
    colorindex = 0;                       // reset color index
    curcolor = sequence[colorindex];      // update color
    CircuitPlayground.playTone(200, 100); // low beep for reset feedback
    lastslide = slide;                    // remember we've reset
    return;                               // nothing more to do this loop
  }

  // slide switch on right, run

  lastslide = slide;                      // remember we're done resetting

  CircuitPlayground.redLED(LOW);          // okay, not resetting

  cap = CircuitPlayground.readCap(CAP_COLOR, NSAMPLES);
  //cap = cap_color.capacitiveSensorRaw(NSAMPLES);

#ifdef DEBUG
  DEBUG.print("color ");
  DEBUG.println(cap);
#endif

  state = (cap < MIN_CAP_COLOR) || CircuitPlayground.leftButton(); // color change?

  if (state != last) {
    now = millis();                           // get current time
    if (state && (now > nextchangetime)) {    // button pressed and time for update?
        ++colorindex;                         // yes, increment index
        if (sequence[colorindex] == END) {
          colorindex = 0;
        }
        curcolor = sequence[colorindex];      // compute current color
        CircuitPlayground.playTone(500, 100); // audible feedback for color change
        nextchangetime = now + MIN_CHANGE;    // compute timestamp before another update allowed
    }

    last = state;                             // remember state (once per press)
  }

  cap = CircuitPlayground.readCap(CAP_ON, NSAMPLES);
  //cap = cap_on.capacitiveSensorRaw(NSAMPLES);

#ifdef DEBUG
  DEBUG.print("on ");
  DEBUG.println(cap);
#endif

  // determine if LEDs should be lit (button pressed)
  setcolor = ((cap < MIN_CAP_ON) || CircuitPlayground.rightButton()) ? 0x000000 : curcolor;

#ifdef GRAVITY
  // read accelerometer to get magnitudes of X and Y acceleration, which are related to angle of Circuit Playground board
  x     = CircuitPlayground.motionX();
#ifdef DEBUG
  DEBUG.print(x);
  DEBUG.print(' ');
#endif
  y     = CircuitPlayground.motionY();
#ifdef DEBUG
  DEBUG.print(y);
  DEBUG.print(' ');
#endif

  // compute angle from X and Y readings
  theta = atan2(y, x);
#ifdef DEBUG
  DEBUG.print(theta);
  DEBUG.print(' ');
#endif

  // compute which LED to light based on angle, offset to get desired angle with respect to down
  led   = theta / LED_DIV + LED_OFFSET;

  // if angle is negative, wrap around to get positive number
  if (led < 0) {
    led += LED_POSITIONS;
  }
#ifdef DEBUG
  DEBUG.print(led);
  DEBUG.print(' ');
#endif

  // there are no LEDs at the 6 and 12 o'clock positions
  if ((led % 6) == 0) {
    // computed LED is at a blank position, bump forward one
    ++led;
  }

#ifdef DEBUG
  DEBUG.println(led);
#endif

  if ((led != lastled) || (setcolor != lastsetcolor)) { // different LED from last time?
    // yes, turn off old LEDs and light new ones
    CircuitPlayground.setPixelColor(lastled, 0);
    // also turn off LED on other side
    CircuitPlayground.setPixelColor((lastled + 5) % NPIXELS, 0);
    // light new LED
    CircuitPlayground.setPixelColor(led, setcolor);
    // also light LED on other side
    CircuitPlayground.setPixelColor((led + 5) % NPIXELS, setcolor);
    // remember which position is lit
    lastled      = led;
    lastsetcolor = setcolor;
  }
#else
  for (pixel = 0; pixel < NPIXELS; ++pixel) {         // for each LED
    CircuitPlayground.setPixelColor(pixel, setcolor); // set its state
  }
#endif
}
