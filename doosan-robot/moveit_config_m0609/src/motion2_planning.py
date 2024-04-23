#! /usr/bin/env python
import rospy
from moveit_python import MoveGroupInterface
from moveit_msgs.msg import MoveItErrorCodes
import moveit_commander

rospy.init_node('moveit_python_tutorial', anonymous=True)

move_group = MoveGroupInterface("arm", "world")

joints = ["joint1", "joint2", "joint3", "joint4", "joint5", "joint6"]

pose1 = [0.0, 0.0, 0.0, -1.57, 0.0, 1.57, 0.0]
target =	{
    "home": [-1.57, 0.0, 1.57, 0.0, 1.57, 0.0, 0.0],
    "pose_1": [-1.0472, 0.6065, 1.4181, 0.248, 1.0748, 0.0, 0.0],
    "pose_2": [-1.0472, 0.7165, 1.4181, 1.6572, 1.0748, 0.0],
    "pose_3": [-1.0472, 0.7165, 1.4181, 1.6572, -0.2038, 0.0],
    "pose_4": [0.0443, 0.2747, 1.3552, 1.6572, -1.6749, 0.0],
    "pose_5": [0.0, 0.2747, 1.3552, -3.1638, -1.6749, 0.0]
}
while not rospy.is_shutdown():

    for pose_name, pose_value in target.items():
        result = move_group.moveToJointPosition(joints, pose_value, 0.02)
        if result:
            if result.error_code.val == MoveItErrorCodes.SUCCESS:
                rospy.loginfo(f"Trajectory {pose_name} successfully executed!")
                rospy.loginfo("Current Joint Values:")
                # print(moveit_commander.MoveGroupCommander("arm").get_current_joint_values())
                joint_values=moveit_commander.MoveGroupCommander("arm").get_current_joint_values()
                rounded_joint_values = [round(value, 2) for value in joint_values]

                rospy.loginfo(rounded_joint_values)
            else:
                rospy.logerr("Arm goal in state: %s", move_group.get_move_action().get_state())
        else:
            rospy.logerr("MoveIt failure! No result returned.")


move_group.get_move_action().cancel_all_goals()