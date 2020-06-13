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
#include "pixhwk_ctrl_node/PixHawk_SteeringControl.h"
#include "pixhwk_ctrl_node/PixHawk_ThrottleControl.h"
#include "pixhwk_ctrl_node/PixHawk_Goal.h"
#include <iostream>
#include <fstream>
#include <can/e2o_status.h>
// #include <turtlesim/turtleX/teleport_absolute.h>

using namespace std;
using namespace pixhwk_ctrl_node;

//////////////////////////////////////

double p[]={0,0,0};
double dp[]={0.1,0.1,0.1};

ros::Publisher pubThrottle;
PixHawk_ThrottleControl pixHwkThrottle;
ros::Publisher pub_info;
can::e2o_status status;
double curTime=0;
//////////////////////////////////////
double myVelocity[2];
double myPosition[3];
double myPosition_lidar[3];
double Combined_Position[6];
double myGoal[3];
static double myHeadingCompass, myHeading = 0;
double velVector;
double homeLocation[2]= {0, 0};
double InputX,InputY;
static int gotGPSFix = 0;
int useLidar = 1;  //set to 1 if using the Lidar, else set it to 0  CHANGE THE NLGL.CPP file too
double carVelocity=0;


double desiredVel=20;//in km/hr
double preverrorThrottle=0;
double sumerror_pid=0;
double maxAcceleration=2;
double turtvel=0;

double lasttime=0;
double curAccel=0;
double desAccel=0;
double prevVel=-1;
double prevAccelError=0.0;


void velocityCallback(const can::e2o_status::ConstPtr& e2oinfo)
{
  // currvel = velocity->data;
  carVelocity = (e2oinfo->velocity);
  // cout<<currvel<<'\n';
}

double startTime_throttle=0.0;
int flag=0;

double throttleCalc(double curVel)
{

  double throttle;
  double error = desiredVel-curVel;
  
  throttle=(error)*p[0] + (error-preverrorThrottle)*p[1] + sumerror_pid*p[2];
  
  preverrorThrottle=error;
  sumerror_pid += error;

  // if(fabs(throttle)>1)
  //   if(throttle>0)
  //     throttle=1.0;
  //   else
  //     throttle=-1.0;
  if(curVel==desiredVel)
  {
    flag=1;
    startTime_throttle=curTime;
  }
  if(curVel==(desiredVel+1)  && flag==1)
  {
    curAccel=1/(curTime-startTime_throttle);
    flag==0;
    // cout<<"One";
    if(curAccel>2)
      throttle=-1.0;
    else
      throttle=0.0;
  }
  else if(throttle<0)
  {
    // cout<<"Two";
    if(fabs(throttle)>1.0)
      throttle=-1.0;
  }
  else if(throttle>0)
  {
    // cout<<"Three";
    if(throttle < 0.2 )
      throttle = 0.2;
    else if (throttle > 0.8)
      throttle = 0.8;
  }
  // ROS_INFO("Throttle :%f",throttle);
  // cout<<"Throttle:"<<throttle<<' ';
  return throttle;
}


double param_th[]={6.283077460472147102e+00,1.629848675285511206e+01,6.990654879874271810e-01,4.693173467879619059e-16
  ,3.158030223183717622e-01};
double param_brake[]={5.61927532 , 0.62389385 , 0.43734694};
double velocity=0;
double throttle=0;
double brake=0;
double steering=0;
double steering_direc=1;
int flag1=0,flag2=0;
double startTime_velocity=0;
double prevThrottle=0;
double prevAccel=0;
double v0;
double offsetTime=0;


// void get_velocity(const pixhwk_ctrl_node::PixHawk_ThrottleControl::ConstPtr& Throttle_value)
void get_velocity(double throttle)
{
  // double curTime = ros::Time::now().toSec();
  curTime +=0.2;
  // throttle = (Throttle_value->throttle);
  // cout<<"curTime :"<<curTime<<' ';
  if(throttle>0)
  {
    brake=0;
    flag2=0;
    if(prevThrottle!=throttle)
    {
      startTime_velocity = curTime;
      offsetTime = pow(velocity/(param_th[0]/(1+exp(param_th[1]*(param_th[4]-throttle))) + param_th[3]),1/param_th[2]);
      // cout<<"OffsetTime:"<<offsetTime<<' ';
    }

    // double curTime = ros::Time::now().toSec();
    double t = curTime - startTime_velocity;
    double accel=param_th[0]/(1+exp(param_th[1]*(param_th[4]-throttle))) + param_th[3];
    velocity = accel*pow(t + offsetTime,param_th[2]);
    // cout<<"Velocity :"<<velocity;
    // curAccel = param_th[2]*accel*pow(t + offsetTime,param_th[2]-1);

    // cout<<"Current velocity :"<<velocity;
  }
  else if(throttle<=0)
  {
    brake=-throttle;
    throttle=0;
    flag1=0;
    if(flag2==0)
    {
      v0=velocity;
      startTime_velocity=curTime;
      flag2=1;
    }
    double decel = (param_brake[0]*pow(brake,param_brake[1]) + param_brake[2]) * (curTime - startTime_velocity);
    velocity= v0 - decel;
    if(velocity<=0)
      velocity=0;
    // cout<<"velocity :"<<velocity;

  }
  prevThrottle = throttle;


  // ROS_INFO("Current Velocity : %f",velocity);
  // status.velocity = velocity;
  // pub_info.publish(status);
  // cout<<'\n';

}

double calcError()
{
  double sumError=0;
  // double error_start_time = ros::Time::now().toSec();
  // double tempTime= ros::Time::now().toSec();
  curTime = 0;
  preverrorThrottle=0;
  sumerror_pid=0;
  velocity=0;
  startTime_velocity=0;
  prevThrottle=0;
  offsetTime=0; flag1=0; flag2=0;
  while (curTime <=20)
  {
    double throttle = throttleCalc(velocity);
    // pixHwkThrottle.throttle = throttle;
    // pubThrottle.publish(pixHwkThrottle);
    get_velocity(throttle);
    sumError +=pow((desiredVel- velocity),2);
    // tempTime= ros::Time::now().toSec();
  }

  return sumError;

}
int main(int argc, char** argv)
{
  ros::init(argc, argv, "pid_tuner");
  ros::NodeHandle n;
  ros::NodeHandle private_nh("~");
  ros::TransportHints noDelay = ros::TransportHints().tcpNoDelay(true);
    
  // pubThrottle = n.advertise<pixhwk_ctrl_node::PixHawk_ThrottleControl>("/PixHawk_ThrottleControl", 1);
  
  // ros::Subscriber curr_velocity = n.subscribe<can::e2o_status>("/can_node/e2o_info", 1, velocityCallback);
  // pub_info = n.advertise<can::e2o_status>("/can_node/e2o_info", 1);
  // ros::Subscriber goal_sub = n.subscribe<pixhwk_ctrl_node::PixHawk_ThrottleControl>("/PixHawk_ThrottleControl", 1, get_velocity);
 
  

  int loopRate = 1;
  ros::Rate r(loopRate);
  
  double besterror = calcError();
  cout<<"Best Error :"<<besterror<<"\n";
  double threshold = 0.001;
  double error;
  while(ros::ok() && (dp[0]+dp[1]+dp[2])>threshold)
  {
    for(int i =0;i<3;i++)
    {
      p[i]+=dp[i];
      error=calcError();
      cout<<"Error :"<<error<<'\n';
      if(error < besterror)
      {
        besterror = error;
        dp[i] *=1.1;
      }
      else
      {
        p[i] -=2*dp[i];
        error=calcError();
        if(error < besterror)
        {
          besterror = error;
          dp[i] *=1.05;
        }
        else
        {
          p[i] +=dp[i];
          dp[i] *=0.95;
        }
      }
    }
    for(int j=0; j<3; j++)
      cout<<p[j]<<' '<<dp[j]<<"  ";
    cout<<'\n';
  }

  for(int i=0; i<3; i++)
    cout<<p[i]<<' ';

  return 0;
}
