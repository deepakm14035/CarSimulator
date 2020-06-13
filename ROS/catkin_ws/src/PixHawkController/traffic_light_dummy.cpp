#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <std_msgs/String.h>
#include <geometry_msgs/PoseStamped.h>
#include <math.h>
#include <sensor_msgs/NavSatFix.h>
#include "pixhwk_ctrl_node/PixHawk_Goal.h"
#include "pixhwk_ctrl_node/traffic_light.h"
#include <iostream>
#include <fstream>
#include "nlgl.h"
#include "nlgl.cpp"
#include <can/e2o_status.h>
using namespace std;


int main(int argc, char** argv)
{
  ros::init(argc, argv, "traffic_light_tester");
  ros::NodeHandle n;
  ros::TransportHints noDelay = ros::TransportHints().tcpNoDelay(true);
  
  ros::Publisher pub = n.advertise<pixhwk_ctrl_node::traffic_light>("/traffic_light_data", 1);
  pixhwk_ctrl_node::traffic_light data;
  while(ros::ok())
  {
    cout<<"Enter distance to light | return | light color"<<endl;
    cin>>data.dist_light;
    cin>>data.color_light;
    data.header.stamp = ros::Time::now();
    pub.publish(data);
    cout<<"done"<<endl<<endl;
  }

return 0;
}