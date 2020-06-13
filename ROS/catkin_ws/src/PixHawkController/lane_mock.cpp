#include <ros/ros.h>
#include <math.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#define PI 3.14159265
#include "pcldefs.h"
#include <pcl_conversions/pcl_conversions.h>
#include <sensor_msgs/PointCloud2.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Quaternion.h>
#include <std_msgs/Header.h>
#include <pixhwk_ctrl_node/Lanes.h>

using namespace std;

ros::Publisher pub_left;
ros::Publisher pub_right;
ros::Publisher pub_lane;
int radius = 10;

int scan_no = 0;
vector<double> left_lane[3],right_lane[3], trajectory[6];

// double distance(double x1, double y1, double z1, double x2, double y2, double z2)
// {
//   return sqrt(pow(x1-x2,2) + pow(y1-y2,2) + pow(z1-z2,2));
// }

double distance(double x1, double y1,  double x2, double y2)
{
  return sqrt(pow(x1-x2,2) + pow(y1-y2,2));
}

double inverse_rot_trans(double &x, double &y, double &z, double x_trans, double y_trans, double z_trans, double angle_x , double angle_y, double angle_z)
{
  // angle_x *= -1.0;
  // angle_y *= -1.0;
  // angle_z *= -1.0;

  //reverse translation
  x = x - x_trans;
  y = y - y_trans;
  z = z - z_trans; 

  //reverse rotation
  double x_rotated, y_rotated, z_rotated;

  //rotation along x
  y_rotated = (y*cos(angle_x)) + (z*sin(angle_x));
  z_rotated = (y*-1.0*sin(angle_x)) + (z*cos(angle_x));
  x_rotated = x;

  //rotation along y
  z = z_rotated;
  x = x_rotated;
  y = y_rotated;
  x_rotated = (x*cos(angle_y))-(z*sin(angle_y));
  z_rotated = (x*sin(angle_y)) + (z*cos(angle_y)) ;
  y_rotated = y;

  //rotation along z
  x = x_rotated;
  y = y_rotated;
  z = z_rotated;

  x_rotated = (x*cos(angle_z)) + (y*sin(angle_z));
  y_rotated = (x*-1.0*sin(angle_z)) + (y*cos(angle_z));
  z_rotated = z;


  x = x_rotated;
  y = y_rotated;
  z = z_rotated;
}

void point_cloud_callback (const sensor_msgs::PointCloud2Ptr& in_cloud)
{
  cout<<"scan : "<<scan_no<<endl;

  pixhwk_ctrl_node::Lanes lane_msg;

  geometry_msgs::Point32 point2;
  pcl::PointXYZI point1;

  pcl::PointCloud<pcl::PointXYZI>::Ptr left_lane_cloud(new pcl::PointCloud<pcl::PointXYZI>);
  pcl::PointCloud<pcl::PointXYZI>::Ptr right_lane_cloud(new pcl::PointCloud<pcl::PointXYZI>);

 
  /*
  change logic of setting this position
  */
  //--------------------------
  double pos_x,pos_y,pos_z, angle_x, angle_y, angle_z;

  pos_x = trajectory[0][scan_no];
  pos_y = trajectory[1][scan_no];
  pos_z = trajectory[2][scan_no];
  angle_x = trajectory[3][scan_no] *PI/180 ;
  angle_y = trajectory[4][scan_no] *PI/180;
  angle_z = trajectory[5][scan_no] *PI/180;
  //---------------------------

  //left lane
  for(int i=0; i<left_lane[0].size(); i++)
  {
    double x,y,z;
    x = left_lane[0][i];
    y = left_lane[1][i];
    z = left_lane[2][i];
    // double dist = distance(pos_x, pos_y, pos_z, x, y, z);
    double dist = distance(pos_x, pos_y, x, y);
    
    if(dist <= radius)
    {
      inverse_rot_trans(x, y, z, pos_x, pos_y, pos_z, angle_x, angle_y, angle_z);
      // cout<<x<<" "<<y<<" "<<z<<"\n\n";
      //add to pointcloud for left
      point1.x = x;
      point1.y = y;
      point1.z = z;
      point1.intensity = 1;
      left_lane_cloud->points.push_back(point1);

      point2.x = x;
      point2.y = y;
      point2.z = z;
      lane_msg.lane_left_points.push_back(point2);
    }
  }

  //right lane
  for(int i=0; i<right_lane[0].size(); i++)
  {
    double x,y,z;
    x = right_lane[0][i];
    y = right_lane[1][i];
    z = right_lane[2][i];
    // double dist = distance(pos_x, pos_y, pos_z, x, y, z);
    double dist = distance(pos_x, pos_y, x, y);
    if(dist <= radius)
    {
      inverse_rot_trans(x, y, z, pos_x, pos_y, pos_z, angle_x, angle_y, angle_z);
      //add to pointcloud for right
      point1.x = x;
      point1.y = y;
      point1.z = z;
      point1.intensity = 1;
      right_lane_cloud->points.push_back(point1);

      point2.x = x;
      point2.y = y;
      point2.z = z;
      lane_msg.lane_right_points.push_back(point2);
    }
  }

  //------------------------------------------------------------------
  left_lane_cloud->width = left_lane_cloud->points.size(); 
  left_lane_cloud->height = 1;
  left_lane_cloud->is_dense = true;
  left_lane_cloud->header.seq = scan_no;
  left_lane_cloud->header.frame_id = in_cloud->header.frame_id;

  right_lane_cloud->width = right_lane_cloud->points.size(); 
  right_lane_cloud->height = 1;
  right_lane_cloud->is_dense = true;
  right_lane_cloud->header.seq = scan_no;
  right_lane_cloud->header.frame_id = in_cloud->header.frame_id;

  lane_msg.header.seq = scan_no;
  lane_msg.header.frame_id = in_cloud->header.frame_id;
  lane_msg.header.stamp = in_cloud->header.stamp;
  //-------------------------------------------------------------------
  pcl::PCLPointCloud2* output_left_lane_cloud = new pcl::PCLPointCloud2;
  sensor_msgs::PointCloud2 output_left_lane;
  //convert PCL1 to PCL2 type
  pcl::toPCLPointCloud2(*left_lane_cloud,*output_left_lane_cloud);
  // Convert to ROS data type
  pcl_conversions::fromPCL(*output_left_lane_cloud, output_left_lane);

  pcl::PCLPointCloud2* output_right_lane_cloud = new pcl::PCLPointCloud2;
  sensor_msgs::PointCloud2 output_right_lane;
  //convert PCL1 to PCL2 type
  pcl::toPCLPointCloud2(*right_lane_cloud,*output_right_lane_cloud);
  // Convert to ROS data type
  pcl_conversions::fromPCL(*output_right_lane_cloud, output_right_lane);

  pub_left.publish(output_left_lane);
  pub_right.publish(output_right_lane);
  pub_lane.publish(lane_msg);

  scan_no++;
}

int main (int argc, char *argv[])
{
  ros::init (argc, argv, "lane_mock");
  ros::NodeHandle nh;

  if (argc < 2) {
    cout << "Enter the following arguments ..\n"
            "1) left lane file (.asc)\n"
            "2) right lane file (.asc)\n"
            "3) trajectory file (.asc)\n"
            "3) radius\n"
            "\n";
    return -1;
  }

  double x=0, y=0, z=0, x_rot=0, y_rot=0, z_rot=0;

  char *left_lane_file_name = argv[1];
  char *right_lane_file_name = argv[2];
  char *traj_file_name = argv[3];
  radius = atoi(argv[4]);
  // out_traj_file = string(argv[3]);

  std::ifstream infile_left(left_lane_file_name);
  std::ifstream infile_right(right_lane_file_name);
  std::ifstream infile_traj(traj_file_name);

  std::string line;

  //read left lane
  while (std::getline(infile_left, line))
  {
    std::istringstream iss(line);
    if (!(iss >> x >> y >> z)) { break; } // error
    left_lane[0].push_back(x);
    left_lane[1].push_back(y);
    left_lane[2].push_back(z);
  }

  //read right lane
  while (std::getline(infile_right, line))
  {
    std::istringstream iss(line);
    if (!(iss >> x >> y >> z)) { break; } // error
    right_lane[0].push_back(x);
    right_lane[1].push_back(y);
    right_lane[2].push_back(z);
  }

  //read trajectory
  while (std::getline(infile_traj, line))
  {
    std::istringstream iss(line);
    if (!(iss >> x >> y >> z >> x_rot >> y_rot >> z_rot )) { break; } // error
    trajectory[0].push_back(x);
    trajectory[1].push_back(y);
    trajectory[2].push_back(z);
    trajectory[3].push_back(x_rot);
    trajectory[4].push_back(y_rot);       
    trajectory[5].push_back(z_rot);
  }
  
  ros::Subscriber subscriber_laser = nh.subscribe ("/velodyne_points", 1, point_cloud_callback);
  pub_left = nh.advertise<sensor_msgs::PointCloud2>("left_lane",1);
  pub_right = nh.advertise<sensor_msgs::PointCloud2>("right_lane",1);
  pub_lane = nh.advertise<pixhwk_ctrl_node::Lanes>("lanes",1);

  ros::Rate loop_rate(5);

  while (ros::ok())
  {
    ros::spinOnce();
  }

  return 0;
}
