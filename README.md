# udacityrobotics_whereami

Project "Where Am I" for Udacity Robotics Software Engineer course.

To launch:

 1. Clone this repo inside your catkin workspace (below /src)
 2. Run `catkin_make`
 3. Run `roslaunch my_robot world.launch` to launch gazebo with the world & the robot
 4. In a separate terminal, run `roslaunch my_robot amcl.launch` to launch the AMCL
 5. In a separate terminal, run `rosrun teleop_twist_keyboard teleop_twist_keyboard.py` to be able to control the robot via keyboard
 