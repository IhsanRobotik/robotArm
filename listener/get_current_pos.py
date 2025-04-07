#!/usr/bin/env python3

import rospy
import moveit_commander
from geometry_msgs.msg import Pose

def get_current_pose():
    # Initialize MoveIt! commander and ROS node
    moveit_commander.roscpp_initialize([])
    rospy.init_node('get_current_pose', anonymous=True)

    # Initialize the MoveGroupCommander for controlling the arm
    arm = moveit_commander.MoveGroupCommander("ass")

    # Get the current pose of the end effector
    current_pose = arm.get_current_pose().pose

    rospy.loginfo("Current End-Effector Pose:")
    rospy.loginfo(f"Position  x: {current_pose.position.x}, y: {current_pose.position.y}, z: {current_pose.position.z}")
    rospy.loginfo(f"Orientation (quaternion)  x: {current_pose.orientation.x}, y: {current_pose.orientation.y}, z: {current_pose.orientation.z}, w: {current_pose.orientation.w}")

    # Shutdown MoveIt! commander
    moveit_commander.roscpp_shutdown()

if __name__ == '__main__':
    get_current_pose()
