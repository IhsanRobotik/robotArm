#!/usr/bin/env python3

import serial
import time
import rospy
from sensor_msgs.msg import JointState

# Open the serial port with the appropriate timeout
arduino = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
time.sleep(2)

def joint_state_callback(msg):
    """
    Callback function to process joint states.
    Converts the joint positions to a string and sends it to the Arduino.
    """
    try:
        # Join the joint positions as a string, separated by commas
        position_str = ','.join(map(str, msg.position)) + '\n'  # Convert to string and add newline
        rospy.loginfo(f"Sending joint positions: {position_str}")
        arduino.write(position_str.encode())  # Send the string as bytes
    except Exception as e:
        rospy.logerr(f"Error while sending data to Arduino: {e}")

def joint_state_listener():
    # Initialize the ROS node
    rospy.init_node('panda_joint_state_listener', anonymous=True)

    # Subscribe to the /joint_states topic
    rospy.Subscriber('/joint_states', JointState, joint_state_callback)

    # Keep the node running
    rospy.spin()

if __name__ == '__main__':
    try:
        joint_state_listener()
    except rospy.ROSInterruptException:
        pass
    finally:
        # Close the serial connection when exiting the script
        if arduino.is_open:
            arduino.close()
