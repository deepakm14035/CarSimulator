#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/PoseStamped.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <math.h>
#include "pixhwk_ctrl_node/PixHawk_ThrottleControl.h"
#include <can/e2o_status.h>
using namespace std;

double param_th[]={6.283077460472147102e+00,1.629848675285511206e+01,6.990654879874271810e-01,4.693173467879619059e-16
	,3.158030223183717622e-01};
double param_brake[]={5.61927532 , 0.62389385 , 0.43734694};
double velocity=0;
double throttle=0;
double brake=0;
double steering=0;
double steering_direc=1;
int flag1=0,flag2=0;
double startTime=0;
double prevThrottle=0;
double curAccel=0;
double prevAccel=0;
double v0;
double offsetTime=0;

void get_velocity(const pixhwk_ctrl_node::PixHawk_ThrottleControl::ConstPtr& Throttle_value)
{
	double curTime = ros::Time::now().toSec();
	throttle = (Throttle_value->throttle);
	if(throttle>0)
	{
		brake=0;
		flag2=0;
		if(prevThrottle!=throttle)
		{
			curTime = startTime = ros::Time::now().toSec();
			offsetTime = pow(velocity/(param_th[0]/(1+exp(param_th[1]*(param_th[4]-throttle))) + param_th[3]),1/param_th[2]);
		}

		// double curTime = ros::Time::now().toSec();
		double t = curTime - startTime;
		double accel=param_th[0]/(1+exp(param_th[1]*(param_th[4]-throttle))) + param_th[3];
		velocity = accel*pow(t + offsetTime,param_th[2]);
		// curAccel = param_th[2]*accel*pow(t + offsetTime,param_th[2]-1);

		// cout<<"Current velocity :"<<velocity<<"Offset:"<<offsetTime;
	}
	else if(throttle<=0)
	{
		brake=-throttle;
		throttle=0;
		flag1=0;
		if(flag2==0)
		{
			v0=velocity;
			startTime=curTime;
			flag2=1;
		}
		double decel = (param_brake[0]*pow(brake,param_brake[1]) + param_brake[2]) * (curTime - startTime);
		velocity= v0 - decel;
		if(velocity<=0)
			velocity=0;


	}
	prevThrottle = throttle;

	ROS_INFO("Current Velocity : %f",velocity);

}


int main(int argc, char **argv)
{

	ros::init(argc, argv, "test_controller");
	ros::NodeHandle nh;
	ros::NodeHandle private_nh("~");
	
	// ros::Publisher pubPosition = nh.advertise<geometry_msgs::PoseStamped>("/trajectory", 1);
	ros::Publisher pub = nh.advertise<can::e2o_status>("/can_node/e2o_info", 1);
	ros::Subscriber goal_sub = nh.subscribe<pixhwk_ctrl_node::PixHawk_ThrottleControl>("/PixHawk_ThrottleControl", 1, get_velocity);

	ros::Rate r(2);
	can::e2o_status status;
	while(ros::ok())
	{
		ros::spinOnce();
		status.throttle=throttle;
		status.velocity=velocity;
		status.brake=brake;
		status.steering=steering;
		status.steering_direc=steering_direc;

		pub.publish(status);

		r.sleep();

	}
	return 0;	

}