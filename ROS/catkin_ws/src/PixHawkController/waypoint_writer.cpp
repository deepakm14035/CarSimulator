#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Pose.h>
#include <math.h>
#include <sensor_msgs/NavSatFix.h>
#include "pixhwk_ctrl_node/PixHawk_Goal.h"
#include <iostream>
#include <fstream>
#include "nlgl.h"
#include "nlgl.cpp"
#include <can/e2o_status.h>
#include <vector>
#include <Eigen/Dense>
#include <Eigen/LU>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

double current_position[2] = {0,0};
double prev_position[2] = {0,0};
double min_dist_store = 5;

int flag = 0; // flag for initializing first time in gps_callback 

/*
Stores the waypoints in a file
*/
void gps_callback(const sensor_msgs::NavSatFix::ConstPtr& gps_msg) // use nav_msgs::Odometry for local
{
    if (gps_msg->status.status == 0)
    {
        if(flag == 0)
        {
            prev_position[0] = gps_msg->latitude;
            prev_position[1] = gps_msg->longitude;
            flag = 1;
            return;
        }

        current_position[0] = gps_msg->latitude;
        current_position[1] = gps_msg->longitude;

        double dist = distance(prev_position[0], prev_position[1], current_position[0], current_position[1]);
        cout<<dist<<endl;
        // update the previous position
        
        //check if distance is greater than min distance only then write the waypoint        
        if(dist > min_dist_store)
        {
            prev_position[0] = current_position[0];
            prev_position[1] = current_position[1];
            
            cout<<"in min_dist_store"<<endl;
            // ADD FEATURE: take the file name as command line arg
            string gps_file_name = "testing_separate_planner.asc";
            ofstream out_gps;

            out_gps.open (gps_file_name.c_str (), ios::out | ios::app);
            if (!out_gps) 
            {
            cout << "Unable to open file : " << gps_file_name << "\n";
            throw std::runtime_error ("Fatal error ");
            }
            cout << "Opened file : " << gps_file_name << "\n";
            out_gps <<setprecision(10)<< gps_msg->latitude << " "<<setprecision(10)<< gps_msg->longitude << "\n";
            //ROS_INFO("lat : %.6f lon: %.6f",points[0],points[1]);
            out_gps.close();
        }
    }
    else
    {
        cout<<"------------NO GPS FIX\n";
    }
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "waypoint_writer");
    if (argc < 2)
    {
        cout << "Enter the min distance for storing waypoint (in meters) (5)\n"
                "\n";
        return -1;
    }
    min_dist_store = atof(argv[1]);

    ros::NodeHandle n;
    ros::NodeHandle private_nh("~");
    ros::TransportHints noDelay = ros::TransportHints().tcpNoDelay(true);

    ros::Subscriber gps_sub = n.subscribe<sensor_msgs::NavSatFix>("/mavros/global_position/global", 1000, gps_callback, noDelay);

    int loopRate = 5;
    ros::Rate r(loopRate);

    while(ros::ok())
    {
        ros::spinOnce();
    }
return 0;
}
