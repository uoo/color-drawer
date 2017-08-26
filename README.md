# color-drawer
Draw/write in midair in color with Circuit Playground

This is a simple build that uses the Circuit Playground, along with its
Neopixels, capacitive sensors, and accelerometer to let you "draw" in midair
with a timed exposure.

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
