TheClaw
=======

Semi-functional work in progress, detect hand movements with Leap Motion and send over serial to an Arduino controlling a Phenoptix meArm.  Can work over a suitable radio link such as XBees, I use Ciseco XinoRF & SRF stick, but works over USE serial link to computer too.

Does work, just a bit clumsy and probably needs some tweaks such as scaling factors and establishing a good height to count as a 'base' plane, so that your hand movements stay largely in a zone we can translate into the reachable range of the arm.  Currently the 'upturned cone' of the Leap's view doesn't map well to the arm's range because it needs to be able to see your hand at some height arbitrarily marked as 'ground' for the whole span reach of the arm.

Python
------
Runs on computer with the Leap controller.  Requires Leap SDK to work.  On a Mac this meant copying Leap.py, LeapPython.so and libLeap.dylib from the SDK into the directory TheClaw.py runs from.  Linux is probably similar, Windows will involve DLLs.

TheClaw.py also references the serial port used to connect to the Arduino running the meArm - which will need editing to reflect the serial connection you're using.

Arduino
-------
Runs on Arduino.  Two versions - one if you're connecting the servos directly to PWM ports, another if you're using Afafruit's Servo PWM driver board.  The included code includes setting pin 8 high which is how the Ciseco XinoRF Arduino clone I used connects Serial to its radio link rather than USB - if you're using a regular Uno and USB link rather than radio this will likely have no real effect on you.
