#include <planning.h>

namespace my_planning
{
        void MyPlanningClass::goToPoseGoal()
        {
            move_group.setPoseTarget(target_pose1);
            bool success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
            if(!success) //execute
                throw std::runtime_error("No plan found");

            move_group.move(); //blocking
        }
        
        void MyPlanningClass::goToPoseGoal(geometry_msgs::Pose &pose)
        {
            move_group.setPoseTarget(pose);
            ros::Duration(0.5).sleep();
            bool success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
            /*while (!success) //keep trying until a plan is found
            {
                
                success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
            }*/
            
            if(!success) //execute
                throw std::runtime_error("No plan found");

            move_group.move(); //blocking
        }

        void MyPlanningClass::goToJointState()
        {
            robot_state::RobotState current_state = *move_group.getCurrentState();
            //moveit::core::RobotStatePtr current_state = move_group.getCurrentState();
            std::vector<double> joint_positions;
            joint_model_group = current_state.getJointModelGroup(PLANNING_GROUP);
            current_state.copyJointGroupPositions(joint_model_group, joint_positions);
            //joint_positions = move_group.getCurrentJointValues();

            joint_positions[0] = 1.57;
            joint_positions[1] = 0.0;
            joint_positions[2] = 1.57;
            joint_positions[3] = 0.7;
            joint_positions[4] = 1.57;


            move_group.setJointValueTarget(joint_positions);
            bool success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
            if(!success)
                throw std::runtime_error("No plan found");

            move_group.move(); //blocking
        }

        void MyPlanningClass::cartesianPath()
        {
            std::vector<geometry_msgs::Pose> waypoints;
            waypoints.push_back(target_pose1);

            geometry_msgs::Pose target_pose2 = target_pose1;

            target_pose2.position.z -= 0.2;
            waypoints.push_back(target_pose2);

            target_pose2.position.y -= 0.2;
            waypoints.push_back(target_pose2);

            target_pose2.position.z += 0.2;
            target_pose2.position.y += 0.2;
            target_pose2.position.x -= 0.2;
            waypoints.push_back(target_pose2);

            move_group.setMaxVelocityScalingFactor(0.1);

            // We want the Cartesian path to be interpolated at a resolution of 1 cm
            // which is why we will specify 0.01 as the max step in Cartesian
            // translation.  We will specify the jump threshold as 0.0, effectively disabling it.
            // Warning - disabling the jump threshold while operating real hardware can cause
            // large unpredictable motions of redundant joints and could be a safety issue
            moveit_msgs::RobotTrajectory trajectory;
            const double jump_threshold = 0.0;
            const double eef_step = 0.01;
            double fraction = move_group.computeCartesianPath(waypoints, eef_step, jump_threshold, trajectory);

            move_group.move();
            ROS_INFO_STREAM("Percentage of path followed: " << fraction);
        }

        void MyPlanningClass::resetValues()
        {
            //set the start state and operational speed
            move_group.setStartStateToCurrentState();
            move_group.setMaxVelocityScalingFactor(1.0);
        }

         void MyPlanningClass::makeBox(std::string blk_name, double *pose)
         {
             moveit_msgs::CollisionObject box;
            //set the relative frame
            box.header.frame_id = move_group.getPlanningFrame();
            box.id = blk_name;

            shape_msgs::SolidPrimitive primitive;
            primitive.type = primitive.BOX;
            primitive.dimensions.resize(3);
            primitive.dimensions[0] = 0.2;
            primitive.dimensions[1] = 0.2;
            primitive.dimensions[2] = 1.0;

            geometry_msgs::Pose box_pose;
            box_pose.orientation.w = 1.0;
            box_pose.position.x = pose[0];
            box_pose.position.y = pose[1];
            box_pose.position.z = pose[2];

            box.primitives.push_back(primitive);
            box.primitive_poses.push_back(box_pose);
            box.operation = box.ADD;

            std::vector<moveit_msgs::CollisionObject> collisionObjects;
            collisionObjects.push_back(box);
            ros::Duration(2).sleep();
            virtual_world.addCollisionObjects(collisionObjects);
            ROS_INFO_STREAM("Added: " << blk_name);
         }

        void MyPlanningClass::addObjects()
        {
            double box_pose1[3] = {0.60, -0.67, 0.0,};
            makeBox("block_1", box_pose1);

            double box_pose2[3] = {0.0, 0.77, 0.0,};
            makeBox("block_2", box_pose2);
        }

        void MyPlanningClass::removeObjects()
        {
            std::vector<std::string> object_ids;
            object_ids.push_back("block_1");
            object_ids.push_back("block_2");
            virtual_world.removeCollisionObjects(object_ids);
        }
}