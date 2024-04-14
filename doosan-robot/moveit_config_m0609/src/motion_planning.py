#! /usr/bin/env python
import rospy
from moveit_python import MoveGroupInterface
from moveit_msgs.msg import MoveItErrorCodes

rospy.init_node('moveit_python_tutorial', anonymous=True)

move_group = MoveGroupInterface("arm", "world")

joints = ["joint1", "joint2","joint3","joint4","joint5","joint6",]
pose = [0.0, 0.0, 0.0, -1.57, 0.0, 1.57, 0.0]

while not rospy.is_shutdown():

    result = move_group.moveToJointPosition(joints, pose, 0.02)
    if result:

        if result.error_code.val == MoveItErrorCodes.SUCCESS:
            rospy.loginfo("Trajectory successfully executed!")
        else:
            rospy.logerr("Arm goal in state: %s",
                         move_group.get_move_action().get_state())
    else:
        rospy.logerr("MoveIt failure! No result returned.")

move_group.get_move_action().cancel_all_goals()