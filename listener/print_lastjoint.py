import sys
import rospy
import moveit_commander
from geometry_msgs.msg import Pose

# Initialize the moveit_commander and rospy node
moveit_commander.roscpp_initialize(sys.argv)
rospy.init_node('print_end_effector_position', anonymous=True)

# Instantiate a MoveGroupCommander object
group_name = "ass"  # Replace 'arm' with your move group name
move_group = moveit_commander.MoveGroupCommander(group_name)

# Get the current pose of the end effector
end_effector_pose = move_group.get_current_pose().pose

# Print the XYZ position of the end effector
print("End Effector Position:")
print(f"X: {end_effector_pose.position.x}")
print(f"Y: {end_effector_pose.position.y}")
print(f"Z: {end_effector_pose.position.z}")

# Shutdown moveit_commander
moveit_commander.roscpp_shutdown()
