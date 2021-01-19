ROS setup

This has been tested in ROS indigo.

Use 'catkin_make' to build project 'make install' in ompl-1.3.1-Source/demos folder. The OMPL library can be received from the other folder in this repository

Four terminals are required

Run the mentioned commands in this exact sequence

, execute = "roslaunch rosbridge_server rosbridge_websocket.launch'
In each of the below 3 points, run the 'source devel/setup.bash' command. 2. roscore 3. In the catkin_ws\src\unity_simulation_scene\launch folder, roslaunch unity_simulation_scene unity_simulation_scene.launch 4. rosrun carsim listener

Common error in sending data - "Could not process inbound connection" - remove any '\n' from messages sent from the listener.cpp

To do some debugging - listen to the topics that have been mentioned in listener.cpp