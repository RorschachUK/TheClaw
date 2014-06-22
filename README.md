TheClaw
=======

Semi-functional work in progress, detect hand movements with Leap Motion and broadcast over radio to a radio-enabled Arduino controlling a Phenoptix meArm

Python
------
Runs on computer with the Leap controller.  Requires Leap SDK to work.  On a Mac this meant copying Leap.py, LeapPython.so and libLeap.dylib from the SDK into the directory TheClaw.py runs from.  TheClaw.py also references the serial port used to connect to the Arduino running the meArm - which will need editing to reflect the serial connection you're using.  I used Ciseco SRF stick.

Arduino
-------
Runs on Arduino, I used a Ciseco XinoRF which is an Uno clone with a built in radio on board.  The XinoRF works by diverting serial to the radio link when pin 8 is set high - if you're using something else, you can edit the code to reflect how yours works.
