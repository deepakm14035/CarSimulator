/*
 *  Copyright (C) 2015 Indraprastha Institute of Information Technology, Sanjit Kaul
 *
 *  License: Modified BSD Software License Agreement
 *
 *  $Id$
 */

/** \file
 *
 * Waypoint following controller
 */

#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Twist.h>
#include <math.h>
#include "nlgl.h"
#include "pixhwk_ctrl_node/PixHawk_SteeringControl.h"
#include "pixhwk_ctrl_node/PixHawk_ThrottleControl.h"
#include "pixhwk_ctrl_node/PixHawk_Goal.h"
#include <iostream>
#include <fstream>
#include <can/e2o_status.h>
#define PI 3.14159265

using namespace std;
using namespace pixhwk_ctrl_node;

static const int m_size = 501;
static const int home = (m_size+1)/2;

//---------------------------Steering------------------------------------------
double my_position[2]; //center of occupancy grid. not considering z
double my_goal[2]; //goal from occupancy grid. not considering z
double my_heading;

//---------------------------- Throttle------------------------------------------
double my_velocity[2];
double curr_vel=0;
double desired_vel=15;//in km/hr
double prev_error_throttle=0;
double sum_error=0;
double max_acceleration=2;
double last_time=0;
double curr_accel=0;
double des_accel=0;
double prev_vel=-1;
double prev_throttle=0.8;
double prev_accel_error=0.0;
double steering_ratio=1.0;

void velocityCallback(const can::e2o_status::ConstPtr& e2oinfo)
{
  // curr_vel = velocity->data;
  curr_vel =(int) (e2oinfo->velocity);
  cout<<curr_vel<<'\n';
}

double start_time=0.0;
int flag=0;

//---------------Speed Controller-----------------------------------//
double throttleCalc(double curVel)
{
  double kp=0.2; //0.20;//0.05 prev value
  double kd=0.0; // Works Fine with kd,ki=0
  double ki=0.0;

  double throttle;
  double error = desired_vel - curVel; // Calculate error
  
  // PID converting error to throttle values
  throttle = (error)*kp + (error-prev_error_throttle)*kd + sum_error*ki; 
  
  // prev_error_throttle=error;
  // sum_error=sum_error+error;

  // Timer start to calculate the acceleration when it reaches current velocity
  if(curVel==desired_vel) 
  {
    flag=1;
    start_time=ros::Time::now().toSec();
  }
  // When the current velocity is 1 greater than des calc accel. If accel less than a value release throttle
  // else apply brakes.
  if(curVel==(desired_vel+1)  && flag==1)
  {
    curr_accel=1/(ros::Time::now().toSec()-start_time);
    flag==0;
    // cout<<"One";
    if(curr_accel>2)
      throttle=-1.0;
    else
      throttle=0.0;
  }
  else if(throttle<0) // If th < 0 and des vel=0 slam brakes else make sure its not greater than -1.0
  {
    // cout<<"Two";
    if(desired_vel==0)
      throttle=-1.0;
    if(fabs(throttle)>1.0)
      throttle=-1.0;
  }
  else if(throttle>0) // set it between 0.2 and 0.8
  {
    // cout<<"Three";
    if(throttle < 0.2 )
      throttle = 0.2;
    else if (throttle > 0.8)
      throttle = 0.8;
  }
  // ROS_INFO("Throttle :%f",throttle);
  return throttle;
}
//-------------- Desired velocity pub from planner callback ----------//
void desVelCallback(const std_msgs::Float64::ConstPtr& des_vel_msg)
{
  desired_vel = des_vel_msg->data;
  cout<<desired_vel<<'\n';
}

//------------ Heading callback--------------//
void headingCallback(const std_msgs::Float64ConstPtr& heading_msg)
{
  my_heading = heading_msg->data;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "pixhwk_ctrl_node");
  ros::NodeHandle n;
  if(argc < 1)
  {
    cout<<"1) Steering ratio (0.5)\n"; // Hyper Parameter
    exit(1);
   } 
  steering_ratio = atof(argv[1]);
  ros::NodeHandle private_nh("~");
  ros::TransportHints noDelay = ros::TransportHints().tcpNoDelay(true);
  ros::Publisher pub_steering;
  ros::Publisher pub_throttle;
  
  pub_steering = n.advertise<pixhwk_ctrl_node::PixHawk_SteeringControl>("/PixHawk_SteeringControl", 1);
  pub_throttle = n.advertise<pixhwk_ctrl_node::PixHawk_ThrottleControl>("/PixHawk_ThrottleControl", 1);
  
  ros::Subscriber heading_sub = n.subscribe("/car_heading",1,headingCallback);
  // ros::Subscriber goal_sub = n.subscribe<pixhwk_ctrl_node::PixHawk_Goal>("/PixHawk_Goal", 1, goalCallback);
  ros::Subscriber curr_velocity = n.subscribe<can::e2o_status>("/can_node/e2o_info", 1, velocityCallback);
  ros::Subscriber des_vel_sub = n.subscribe<std_msgs::Float64>("/desired_velocity",1,desVelCallback);
  
  PixHawk_SteeringControl pixHwkSteer;
  PixHawk_ThrottleControl pixHwkThrottle;

  my_position[0] = home;              //default = 51
  my_position[1] = home;              //default = 51
  
  // ifstream iFile;
  // double Input_x,Input_y;
  // iFile.open("src/obstaclematrix/src/trajectory_ompl.txt");
  // if ((iFile >> Input_x) && (iFile >> Input_y))
  // {
  //   my_position[0] = Input_x;
  //   my_position[1] = Input_y;
  // }
  // iFile.close();
  
  int loop_rate = 3;
  ros::Rate r(loop_rate);
  double steering_command;
  int loop_count = 0;
  int first_throttle = 1;
  
  double curr_time = ros::Time::now().toSec();
  double last_time=curr_time;
  int read_vel;

  while(ros::ok())
  {
    ros::spinOnce();

    double x,y,theta,velocity_cmd_ompl,steering_cmd_ompl,duration_ompl;
    ifstream iFile;
    //------------------- Reading Controls based file ----- Not used----//
    // ifstream controlFile;
    // controlFile.open("/home/nuc1/iiitd//swarath/control.txt");
    // int ctr_ompl=0;
    // while((controlFile >> x) && (controlFile >> y) && (controlFile >> theta) && (controlFile >> velocity_cmd_ompl) && (controlFile >> steering_cmd_ompl) && (controlFile >> duration_ompl))
    // {
    //   ctr_ompl++;
    //   if(ctr_ompl==2)
    //   {
    //     cout<<"Goal : "<<x<<' '<<y<<'\n';
    //     steering_command =  -1*(steering_cmd_ompl*180/PI)*steering_ratio/40;
    //     cout<<"steering command: "<<steering_command*40<<'\n';
    //     ctr_ompl=0;
    //     break;
    //   }
    // }
    //------------------ Reading the Traj and calculating throttle and steering-----//
    // /home/pratique/Swarath_Repo/swarath/trajectory_ompl.txt
    iFile.open("/home/pratique/Swarath_Repo/swarath/trajectory_ompl.txt");                           // change file path here to run this code on nuc ( /home/nuc1/iiitd/swarath/trajectory_ompl.txt )

    int ctr = 0;
    while((iFile >> x) && (iFile >> y))
      {
        ctr++;
        if(ctr == 3) // Read the third point
        {
          cout<<"Goal : "<<x<<' '<<y<<'\n';
          // Calculate Normalized Steering and multiply by steering ratio.
          steering_command = ((90-(rad2deg(atan2(y - my_position[1], x - my_position[0]))))*steering_ratio)/40;
          // cout<<"Steering Ratio : "<<steering_ratio<<endl;
          cout<<"original steering "<<90-rad2deg(atan2(y - my_position[1], x - my_position[0]))<<'\n';
          // ROS_INFO("goal = %lf,%lf",my_goal[0],my_goal[1]);
          // ROS_INFO("waypoint = %lf,%lf",x,y);  
          cout<<"converted steering : "<< steering_command;
          ctr = 0;
          break;
        }
      }


    // steering_command = 1.0 * nlgl(my_position, my_heading, my_goal);      
    //-- Check to keep the steering commands within the limits----//
    if (my_position[1] != 0 && my_position[2] != 0)
    {
      if(steering_command <= -1) 
      {
        steering_command = -1;
      }
      else if(steering_command >= 1)
      {
        steering_command = 1;
      }          
    }

    double throttle_command = throttleCalc(curr_vel); // Calc throttle

    pixHwkThrottle.throttle = throttle_command;
    pixHwkSteer.steering = steering_command;        

    pub_steering.publish(pixHwkSteer);
    pub_throttle.publish(pixHwkThrottle);
    
    r.sleep();
  }

  return 0;
}
