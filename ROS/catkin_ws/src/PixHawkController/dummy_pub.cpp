#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/PoseStamped.h>
#include <math.h>
#include <sensor_msgs/NavSatFix.h>
#include "pixhwk_ctrl_node/PixHawk_Goal.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include "nlgl.h"
#include "nlgl.cpp"
#include "std_msgs/String.h"
#include <can/e2o_status.h>
using namespace std;



float RandomFloat(float min, float max)
{
    // this  function assumes max > min, you may want 
    // more robust error checking for a non-debug build
    assert(max > min); 
    float random = ((float) rand()) / (float) RAND_MAX;

    // generate (in your case) a float between 0 and (4.5-.78)
    // then add .78, giving you a float between .78 and 4.5
    float range = max - min;  
    return (random*range) + min;
}




int main(int argc, char** argv)
{
  ros::init(argc, argv, "DummyObstacles");
  ros::NodeHandle n;
  ros::NodeHandle private_nh("~");
  std::string output_file = "/home/pratique/Swarath_Repo/swarath/trajectory_ompl.txt";
  int nosolfreq = 23;
  int count = 0;


  
  std::ofstream outFile(output_file.c_str());
  outFile<<"Skipped!"<<endl;

  ros::Publisher pubObsstatus = n.advertise<std_msgs::String>("DummyStatus", 1);
  ros::Rate loop_rate(10);
  
  while(ros::ok())
  {
  	std_msgs::String msg;
    std::stringstream ss;
    ss << "I am publishing dummy obstacles " << count;
    msg.data = ss.str();
   
    ROS_INFO("%s", msg.data.c_str());

    count++;
  	if(count % nosolfreq == 0)
  		{
  			  outFile<<"NoSolFound"<<endl;
  		}
  	else
  		{
  			outFile<<RandomFloat(0.0, 100.0)<<"\t";
  			outFile<<RandomFloat(0.0, 100.0)<<endl;
  		}

    pubObsstatus.publish(msg);
    ros::spinOnce();
  }

return 0;
}