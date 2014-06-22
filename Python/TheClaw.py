# The Claw May 2014 York Hack Space
# A Leap Motion controller for the meArm robotic arm
# Operates over a serial link to the meArm's Arduino controller, this link can be
# direct over USB, or can be implemented via XBees, Ciseco XRFs, some bits of wire, or 
# anything else which provides a transparent wired or wireless serial connection.  
# I used Ciseco SRF/XinoRF.

import Leap, sys
from serial import Serial
import time

#Edit this line with the correct details for your serial port, I was using Ciseco SRF
port = Serial('/dev/tty.usbmodem000001',115200, timeout=0)
lastPing = time.time()
lastPos = ""
lastState = ""

class TheClawListener(Leap.Listener):
    def on_frame(self, controller):
    	global lastPing, lastPos, lastState
        # Get the most recent frame and report some basic information
        frame = controller.frame()

        if not frame.hands.is_empty:
            # Get the first hand
            hand = frame.hands[0]

			#Are we making a 'grab' gesture?
            if hand.grab_strength > 0.9:
                state = "close"
            else:
                state = "open"
            # Leap coordinates are front-on rather than top-down - also translate a little
            pos = [int(hand.palm_position.x), 50-int(hand.palm_position.z), int(hand.palm_position.y) - 50]
            # Get the hand's sphere radius and palm position
            posStr = "goto(" + str(pos[0]) + "," + str(pos[1]) + "," + str(pos[2]) +")"
            if posStr != lastPos and time.time() - lastPing > 0.02:
            	lastPos = posStr
            	port.write(posStr + ";")
            	print "Hand: %s at %s" % (state, posStr)
            	lastPing = time.time()
        else:
            state="reset"
        if state != lastState:
            lastState = state
            port.write(state + ";")
            print "New state: " + state
			
def main():

    # Create a sample listener and controller
    listener = TheClawListener()
    controller = Leap.Controller()

    # Have the sample listener receive events from the controller
    controller.add_listener(listener)


    # Keep this process running until Enter is pressed
    print "Press Enter to quit..."
    sys.stdin.readline()

    # Remove the sample listener when done
    controller.remove_listener(listener)


if __name__ == "__main__":
    main()
