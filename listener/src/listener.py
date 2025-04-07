#!/usr/bin/env python3

import rospy
from sensor_msgs.msg import JointState

def joint_state_callback(msg):
    """
    Callback function to process joint states.
    Prints the joint names and their respective positions.
    """
    rospy.loginfo(msg.position)

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
