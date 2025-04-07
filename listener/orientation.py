import sys
import rospy
import moveit_commander

# Initialize moveit_commander and ROS node
moveit_commander.roscpp_initialize(sys.argv)
rospy.init_node('print_current_orientation', anonymous=True)

# Instantiate a MoveGroupCommander object
group_name = "ass"  # Replace 'arm' with your move group name
move_group = moveit_commander.MoveGroupCommander(group_name)

# Get the current pose of the robot's end effector
current_pose = move_group.get_current_pose().pose

# Print the orientation of the end effector
print("Current End Effector Orientation:")
print(f"Orientation X: {current_pose.orientation.x}")
print(f"Orientation Y: {current_pose.orientation.y}")
print(f"Orientation Z: {current_pose.orientation.z}")
print(f"Orientation W: {current_pose.orientation.w}")

# Shutdown moveit_commander
moveit_commander.roscpp_shutdown()
