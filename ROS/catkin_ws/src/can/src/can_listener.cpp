#include <cmath>
#include <cstring>
#include <can/e2o_status.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <ros/ros.h>
#include <sstream>
#include <string>
#include <std_msgs/String.h>
#include <std_msgs/Header.h>
#include <vector>
#include <std_msgs/Float64.h>

using namespace std;

string file_name;
int flag = 0;
ros::Time start_time;
void callback (const can::e2o_statusPtr& in_status)
{
  // location.header.stamp = in_cloud->header.stamp;

  if(flag == 0)
  {
    flag = 1;
    start_time = in_status->header.stamp;
  }

  ros::Time curr_time = in_status->header.stamp;
  ros::Duration diff = curr_time - start_time;
  cout<< diff <<"\n";

  ofstream out_status;
  std::string line;

  int velocity = in_status->velocity;
  int throttle = in_status->throttle;
  int brake = in_status->brake;
  int steering = in_status->steering;
  int steering_direc = in_status->steering_direc;
  
  out_status.open (file_name.c_str (), ios::out | ios::app);
  if (!out_status) 
  {
    cout << "Unable to open file : " << file_name << "\n";
    throw std::runtime_error ("Fatal error ");
  }
  out_status  << velocity << "," << throttle << "," << brake << "," << steering << "," << steering_direc << "," << diff <<"\n";
  out_status.close();
}
int flag_compass=0;
double start_time_compass=0;
void compass_callback(const std_msgs::Float64::ConstPtr& compass_msg)
{
  double curr_time = ros::Time::now().toSec();
  double compass_data = compass_msg->data;
  if(flag_compass == 0)
  {
    start_time_compass = ros::Time::now().toSec();
    curr_time = start_time_compass;
    flag_compass = 1;
  }
  double diff = curr_time - start_time_compass;
  ofstream out_compass;
  out_compass.open (file_name.c_str (), ios::out | ios::app);
  if (!out_compass) 
  {
    cout << "Unable to open file : " << file_name << "\n";
    throw std::runtime_error ("Fatal error ");
  }
  out_compass<<compass_data<<' '<<diff<<'\n';
  out_compass.close();

}
 
int main (int argc, char *argv[])
{
  ros::init (argc, argv, "can_listener");
  ros::NodeHandle nh;
  file_name = string(argv[1]);
  // ros::Subscriber subscriber = nh.subscribe ("/can_node/e2o_info", 10000, callback);
  ros::Subscriber subscriber = nh.subscribe("/mavros/global_position/compass_hdg", 10000, compass_callback);
  ros::spin();
  return 0;
}
