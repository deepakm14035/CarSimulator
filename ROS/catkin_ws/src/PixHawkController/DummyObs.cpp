#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/PoseStamped.h>
#include <math.h>
#include <sensor_msgs/NavSatFix.h>
#include "pixhwk_ctrl_node/PixHawk_Goal.h"
#include <iostream>
#include <fstream>
#include "nlgl.h"
#include "nlgl.cpp"
#include <can/e2o_status.h>
using namespace std;


int main(int argc, char** argv)
{
  ros::init(argc, argv, "DummyObs");
   ros::NodeHandle n;
  ros::NodeHandle private_nh("~");
  ros::TransportHints noDelay = ros::TransportHints().tcpNoDelay(true);
  
  ros::Publisher pubObs = n.advertise<std_msgs::Float64>("/distObst", 1);
  double obstacle;
  std_msgs::Float64 obs_msg;
  while(ros::ok())
  {
  	cin>>obstacle;
  	obs_msg.data=obstacle;
  	pubObs.publish(obs_msg);

  }

return 0;
}