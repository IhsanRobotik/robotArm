#!/usr/bin/env python3

import rospy
import moveit_commander
from geometry_msgs.msg import Pose
step = 0

def move_end_effector_to_xyz(x, y, z, w):
    # Initialize MoveIt! commander and ROS node
    moveit_commander.roscpp_initialize([])
    rospy.init_node('simple_move_end_effector', anonymous=True)

    # Initialize the MoveGroupCommander for controlling the arm
    arm = moveit_commander.MoveGroupCommander("cobot")

    # Set tolerances for position and orientation
    arm.set_goal_position_tolerance(0.01)
    arm.set_goal_orientation_tolerance(0.01)

    # Define the target pose for the end effector
    target_pose = Pose()
    target_pose.position.x = x
    target_pose.position.y = y
    target_pose.position.z = z
    target_pose.orientation.w = w  # Keeping orientation fixed for simplicity

    # Set the pose target for the arm and move it
    arm.set_pose_target(target_pose)
    success = arm.go(wait=True)

    # Check if movement was successful
    if success:
        rospy.loginfo("End eto target poffector moved to target position successfully!")
        step =+ 1
    else:
        rospy.logwarn("Failed to move sition.")

    # Clear targets and shut down MoveIt! commander
    arm.clear_pose_targets()
    moveit_commander.roscpp_shutdown()

if __name__ == '__main__':


    # move to pos
    try:
        x_target = 0.1
        y_target = 0.2
        z_target = 0.
        w_target = 1
        move_end_effector_to_xyz(x_target, y_target, z_target, w_target)

    except rospy.ROSInterruptException:
        pass
