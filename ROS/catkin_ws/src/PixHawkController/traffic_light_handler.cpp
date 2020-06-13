#include "ros/ros.h"
#include <std_msgs/String.h>
#include <std_msgs/Float64.h>
#include <pixhwk_ctrl_node/traffic_light.h>
#include <pixhwk_ctrl_node/desired_velocity.h>

static std_msgs::String traffic_light_color;
ros::Publisher pub_des_vel;

static double curr_velocity = 0;
// static double curr_angle = 0;
// static double coordinates_x = 0;
// static double coordinates_y = 0;
// static double coordinates_trafficlight_x = 0;
// static double coordinates_trafficlight_y = 0;

// calculates the stopping distance based on various params
inline double get_stopping_distance() 
{
    return 10;
}

// Distance set initially
double traffic_light_distance = 200;
double desired_vel = 0;
int previous_vel = -1000;

// TO BE CONVERTED TO PARAMETERS
// gap to maintain between the obstacle and the car
double safe_distance = 0;
double yellow_tolerence = 2;

// double distance(double x1,double y1,double x2,double y2)
// {
//     double dis = sqtr(pow((x1-x2),2) + pow((y1-y2),2));
//     return dis;
// }

int control() {
    pixhwk_ctrl_node::desired_velocity des_vel_msg;
    des_vel_msg.node_id = 10;
    if (traffic_light_color.data == "red") {
        // light far away so let normal trajectory movement take place
        if (get_stopping_distance() - safe_distance < traffic_light_distance) 
        {
            ROS_INFO("rMoving normally/ Maybe slowing down");
        }
        // stopping as the light is red
        else 
        {
            // stop the car
            ROS_INFO("rStopping the car");
            des_vel_msg.velocity = 0;
    
            des_vel_msg.priority = 50;
            // if(previous_vel != des_vel_msg.velocity)
            // {
                pub_des_vel.publish(des_vel_msg);
                // previous_vel = des_vel_msg.velocity;
            // }

            // else
            // {
            //     //Don't publish
            // }
        }
    }
    else if (traffic_light_color.data == "yellow") 
    {
        // far so keep going or slow down until another case recieved
        if (get_stopping_distance() - safe_distance < traffic_light_distance) 
        {
            ROS_INFO("yMoving normally/ Maybe slowing down");
        }
        // possible to stop safely, so stop
        // keeping a small tolerence in which we are not so close to the light that we wont be able to stop.
        else if (get_stopping_distance() - safe_distance < traffic_light_distance + yellow_tolerence) 
        {
            ROS_INFO("yStopping the car");
            des_vel_msg.velocity = 0;
    
            des_vel_msg.priority = 50;
            
            // if(previous_vel != des_vel_msg.velocity)
            // {
                pub_des_vel.publish(des_vel_msg);
            //     previous_vel = des_vel_msg.velocity;
            // }

            // else
            // {
            //     //Don't publish
            // }
        }
        // yellow light but already crossing so keep going
        else 
        {
            ROS_INFO("yMoving normally/ Maybe speed up");
        }
    }
    else if (traffic_light_color.data == "green") 
    {
        ROS_INFO("gMoving normally, found green light");
        des_vel_msg.velocity = 3;
        des_vel_msg.priority = 0;
        pub_des_vel.publish(des_vel_msg);
    }
    else 
    {
        ROS_INFO("Invalid Color Recieved");
    }
}



    // double d = distance(coordinates_trafficlight_x,coordinates_trafficlight_y,coordinates_x,coordinates_y);
    // if (command == 0)
    // {
    //     ROS_INFO("Didn't receive the command yet.")
    // }
    // else if (command == 1)
    // {
    //     double d = distance(coordinates_x,coordinates_y,coordinates_trafficlight_x,coordinates_trafficlight_y);
    //     ROS_INFO("Stop within ");
    //     ROS_INFO(d);
    // }
    // else if (command == 3)
    // {
    //     ROS_INFO("Get ready to move");
    // }
    // else if (command == 2)
    // {
    //     ROS_INFO("Keep moving normally");
    // }

// void velocity_callback(std_msgs::Float64 velocity) {
//     curr_velocity = velocity.data;
// }

// void angle_callback(const double::ConstPtr& angle)
// {
//     curr_angle = angle;
// }

// void coordinates_callback(const geometric_msgs::Point& coordinates)
// {
//     coordinates_x = coordinates->x;
//     coordinates_y = coordinates->y;
// }

// void coordinates_trafficlight_callback(const geometric_msgs::Point& coordinates_trafficlight)
// {
//     coordinates_trafficlight_x = coordinates_trafficlight->x;
//     coordinates_trafficlight_y = coordinates_trafficlight->y;
// }

// void dist_callback(const std_msgs::Float64::ConstPtr& obs) {
//     traffic_light_distance = obs->data;
//     // double deceleration=(-1*pow((curr_vel*5/18),2))/(2*dist_obst);
// }

// void light_color_callback(const std_msgs::String l_color) {
//     traffic_light_color.data = l_color.data;
// }

void traffic_light_callback(const pixhwk_ctrl_node::traffic_light msg) 
{
    traffic_light_distance = msg.dist_light;
    traffic_light_color.data = msg.color_light;
}

int main(int argc, char** argv)
{       
    ros::init(argc, argv, "traffic_light_handler");
    ros::NodeHandle nh;

    // traffic light related subscriptions
    ros::Subscriber light_color_sub = nh.subscribe<pixhwk_ctrl_node::traffic_light>("/traffic_light_data", 1, traffic_light_callback);
    // ros::Subscriber light_color_sub = nh.subscribe<std_msgs::String>("/traffic_light_color", 1, light_color_callback);
    // ros::Subscriber dist_sub = nh.subscribe<std_msgs::Float64>("/distObst", 1, dist_callback);
    // ros::Subscriber coordinates_trafficlight_sub = nh.subscibe<geometric_msgs::Point>("coordinates_trafficlight",1,coordinates_trafficlight_callback);
    
    // car information related callbacks
    // NOTE - as of now assuming that directly getting the distances from DummyObs.cpp
    // ros::Subscriber velocity_sub = nh.subscribe<std_msgs::Float64>("/velocity",1,velocity_callback);
    // ros::Subscriber theta_sub = nh.subscibe<double>("angle",1,angle_callback);
    // ros::Subscriber coordinates_sub = nh.subscibe<geometric_msgs::Point>("coordinates",1,coordinates_callback);

    // publisher to desired_velocity
    pub_des_vel = nh.advertise<pixhwk_ctrl_node::desired_velocity>("/set_desired_velocity", 1);
    
    

    ros::Rate loop_rate(5);

    while (ros::ok())
    {
        // ROS_INFO("%s", traffic_light_color.data.c_str());
        // ROS_INFO("%f", traffic_light_distance);
        control();
        

        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}