Draw/write in midair in color with Circuit Playground
=====================================================

This is a simple build that uses the Circuit Playground, along with its
Neopixels, capacitive sensors, accelerometer, and onboard power supply to let you "draw" in midair with a timed exposure.

The capacitive sensors are used as switches to turn the Neopixels on and off in order to draw with light, and to change colors.

The accelerometer figures out which direction is currently "down", and uses
this information to determine which Neopixels to light, so they're displayed
as a pair at a 45° angle, to enable "calligraphic" writing.

To build it, put the Circuit Playground at the end of a stick, with the
Neopixels facing out.  Connect wires to the #6 pin ("on"),
the #9 pin ("change color"), and ground.  Run the wires down the stick to either bare ends or better yet, copper tape rings around the stick.
One workable arrangement is to put the ground tape in the middle, with the
"on" tape toward the Circuit Playground end so you can easily touch it
with a finger while holding the stick where the ground ring is.
The "switch color" ring can go behind that somewhere, where you can touch it
with your other hand to switch colors.  Note that the ground ring should be
covered with something insulating like paper or tape, as it merely supplies
a reference for the capacitive sensing (leaving it bare can confuse the
capacitive sensor, causing it to get "stuck" for a second or so, during which
you can't control the Neopixels.

I like to cover the Circuit Playground with some paper or a thin layer of white foam to act as a diffuser for the Neopixels.  Add a small battery to power it
and you're ready to go.

Note that determining the thresholds for the capacitive sensing is a little frustrating:  when the Circuit Playground is connected to a computer via USB, to enable watching the debug output and uploading new versions, the USB cable provides a solid ground, giving much higher capacitance.  When it's untethered, the capacitances will be lower.

To assemble this project without soldering, you can use ordinary clip leads to connect the pads on the Circuit Playground to the copper tape, or to wires leading to the copper tape.

Parts used for this project:
============================

* [Circuit Playground Classic](http://adafru.it/3000)
* [Copper Foil Tape with Conductive Adhesive](http://adafru.it/1128)
* power source (such as [a small LiPoly cell](http://adafru.it/1578))
* stick (dowel, meter stick, an actual tree branch, not at all critical)
* wide clear adhesive tape
* packing foam (optional)
* wire ([such as stranded hook-up wire](http://adafru.it/3111))
* clip leads (optional, [such as](http://adafru.it/1008))
