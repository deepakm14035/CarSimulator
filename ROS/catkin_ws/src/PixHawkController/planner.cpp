/*
SUBSCRIBERS TOPICS:
    /can_node/e2o_info
    /dist_obs
    /mavros/global_position/global

**************************************************************
PUBLISHERS TOPICS:
    /desired_velocity
    /car_heading
    /OmplStatus"


*/
#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Pose.h>
#include <math.h>
#include <sensor_msgs/NavSatFix.h>
#include "pixhwk_ctrl_node/PixHawk_Goal.h"
//#include <pixhwk_ctrl_node/desired_velocity.h>
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
#include "pixhwk_ctrl_node/OmplStatus.h"            //custom message to check status of OptimalPlanning.cpp. Also useful for debugging.

using namespace std;

ros::Publisher pub_des_vel; // Desired Velocity publisher subs - pixhawkctrlNode.cc
ros::Publisher pub_heading; // car heading pub subs - matrixnode.cpp
ros::Publisher pub_OmplStatus; // reads trajectory file and sets the OmplStatus accordingly

double max_vel = 15;// Max velocity that the car can travel
double min_vel = 5;
double min_dist_bearing; // min bearing update distance
double min_dist_goal;   // min distance to between waypoint and car to consider it as reached goal
double current_gps_bearng = 0.0; 
double goal_gps_bearng = 0.0; // bearing between goal and current pos
double current_position[2] = {0,0};
double prev_position[2] = {0,0};
double current_goal[2] = {0,0};
int flag = 0; // flag for initializing first time in gps_callback 
vector<double> waypoints[2]; // waypoints vector [0] contains x and [1] contains y coordinates
int curr_goal_index = 0;  // current goal index in the waypoints file
int navigation_status = 0; // not navigating

double curr_time; // to store the time at the event of calling e2o_callback 
double last_time; // last time e2o_callback was called
double set_distance = 0;  // distance between the obstacle and the car in order to stop. Upates every timestep
double dist_obst = 200; // Distance set initially
int mode=0; // (Yet to implement). 0 - contant vel mode. 1 - obstacle mode. 2 - turning mode. (0 & 2 are merged now)
int desired_velocity = max_vel; 
double dist_obs_car = 200;
double curr_vel=1; // Current vel from e2o callback
double deceleration=-0.3; // Initial deceleration for stopping
int smooth_size=5;  // Heading difference between car and goal smooth window size
vector<double> heading_vect;  // Heading vector storing prev heading differences
double sum_threshold=40; // Threshold to detect turn
double sum_delta = 0;
double ratio = 0;
int scale_factor = 2;
/*
Contain angle between 0 to 360
*/
double angle_unwrap(double theta)
{
    /*
    going from (-180,180) to (0,360)
    */
    if(theta < 0)
    {
        theta = 360 + theta;
    }
    return theta;
}

/*
Calculates heading diff between goal and car
*/
double heading_shift(double current_gps_bearng, double goal_gps_bearng)
{
    double delta = angle_unwrap(goal_gps_bearng) - angle_unwrap(current_gps_bearng);
    /*
    delta +ve is right 
    delta -ve is left
    */
    double heading_shift = delta;
    double sum=0;
    double avg_heading=0;
    heading_vect.push_back(heading_shift);
    for(int i=0;i<heading_vect.size();i++)
    { 
      sum = sum + heading_vect[i];
    }

    avg_heading = sum/heading_vect.size(); // Smoothes out heading
    if(heading_vect.size() == smooth_size)  
      heading_vect.erase(heading_vect.begin()); // Pops back first element
    cout<<"current bearing : "<< current_gps_bearng<<"\n";
    cout<<"goal bearing    : "<< goal_gps_bearng<<"\n";
    if(delta > 0)
        cout<<"right : "<<delta<<"\n";
    else
        cout<<"left  : "<<delta<<"\n";

    /*
    delta -ve is right
    delta +ve is left
    */

    return -1.0*avg_heading;
}

/*
Read Waypoints from the file
*/
void get_waypoints(string waypoints_file)
{
    std::string line;
    std::ifstream infile_waypoints(waypoints_file.c_str ());
    cout<<"------------ GPS waypoints ------------------\n";
    while (std::getline(infile_waypoints, line))
    {
        std::istringstream iss(line);
        float lat,lon;
        if (!(iss >> lat >> lon)) { break; } // error
        // add waypoints
        waypoints[0].push_back(lat);
        waypoints[1].push_back(lon);
        cout<<waypoints[0].size()<<" ";
        printf("%lf %lf " , lat, lon);
        cout<<"\n";
    }
    cout<<"------------------------------------------------\n";
}

/*
Calculates the desired velocity based on the curvature
*/
void set_velocity()
{
  std::string waypoint_xy;
  std::string skipped ("Skipped!");
  std::string nosol ("NoSolFound");
  pixhwk_ctrl_node::OmplStatus status;                            //value of step is either -1, 0 or 1


  if(navigation_status!=2)
  {
    ifstream traj_file;
    mode = 0;
    traj_file.open("/home/nuc1/iiitd/swarath/trajectory_ompl.txt");    //change to the actual directory on nuc[default : "/home/nuc1/iiitd/swarath/trajectory_ompl.txt"]
    double x1=51,y1=51,x2,y2,x3,y3; // To read the waypoints from file
    double prev_angle = 90, angle,delta; 
    int ctr=0;
    sum_delta = 0;
    ratio = 0;
                                                      //status =  1 means no solution found by OptimalPlanning.cpp
                                                  //status =  0 means solution found by OptimalPlanning.cpp
                                                  //status = -1 means time exceeded.
    while(getline (traj_file, waypoint_xy))                         //(traj_file >> x2) && (traj_file >> y2)
    {
      stringstream stream(waypoint_xy);
      status.actualmsg = stream.str();
      ctr++;
       // if(ctr == 2)
       // {
       //   angle = atan2((y2 - y1),(x2-x1));
       //   prev_angle = angle;
       //   x1 = x2; y1 = y2;
       // }
       

      if(waypoint_xy.compare(skipped) == 0)
        {
          status.OmplSolStatus = -1;           //set to -1 
          status.stamp = ros::Time::now();
          pub_OmplStatus.publish(status);

          desired_velocity = 0;
          cout<< "Skipped, thus SET_VEL" <<desired_velocity<<endl;

          break;
        }       


      else if(waypoint_xy.compare(nosol) == 0)
        {
           status.OmplSolStatus = 1;          //set to 1
           status.stamp = ros::Time::now();
           pub_OmplStatus.publish(status);

          desired_velocity = 0;
          cout<< "No solution found, thus SET_VEL" <<desired_velocity<<endl;

          break;
        }

      else if(  (stream >> x3) && (stream >> y3)  )
      {
        status.OmplSolStatus = 0;           //set to 0
        status.stamp = ros::Time::now();
        pub_OmplStatus.publish(status);        
       
        if(ctr > 2) // since we need three points 
        {
          // cout<<x3/scale_factor<<" "<<y3/scale_factor<<" "<<51/scale_factor<<" "<<51/scale_factor<<"\n";
          // cout<<"distance : "<<sqrt(pow(x3/scale_factor - 51/scale_factor,2) + pow(y3/scale_factor - 51/scale_factor,2))<<"\n";
          if(sqrt(pow(x3/scale_factor - 51/scale_factor,2) + pow(y3/scale_factor - 51/scale_factor,2))>10)
            continue;
          // double m1=0,m2=0;

          // x1 = 50;y1 = 50;
          // x2 = 50;y2 = 100;

          // m1 = (y2 - y1) / (x2-x1);

          // x2 = 50;y2 = 50;
          // x3 = 0;y3 = 50;

          // m2 = (y3 - y2) / (x3-x2);
          // double alpha = fabs(90-rad2deg(atan(m2-m1/(1+(m1*m2)))));
          double angle_1 = rad2deg(atan2(x2-x1, y2-y1));
          double angle_2 = rad2deg(atan2(x3-x2, y3-y2));

          // angle_1 = angle_unwrap(angle_1);
          // angle_2 = angle_unwrap(angle_2);

          double alpha = fabs(angle_1 - angle_2);

          cout<<x1<<","<<y1<<" ";
          cout<<x2<<","<<y2<<" ";
          cout<<x3<<","<<y3<<"\n";
          cout<<"angle_1 : "<<angle_1<<endl;
          cout<<"angle_2 : "<<angle_2<<endl;
          cout<<"alpha : "<<alpha<<endl;
          sum_delta += alpha;
        }
        x1 = x2;
        y1 = y2;
          
        x2 = x3/scale_factor;
        y2 = y3/scale_factor;
      }
      else
        break;
    }
  }
    // if(sum_delta >= 0)
    // {
    //   mode = 2;
    // }
    // else
    // {
    //   mode = 0;1
    // }
  
  int outstatus = (int)(status.OmplSolStatus);
  cout<< "OmplStatus check after exiting while = " <<outstatus<<endl;
  ratio =  sum_delta/sum_threshold;
  // if(mode==0) // No obstacles or curves detected,so drive at max velocity.
    // desired_velocity=max_vel;
  // if(mode==1) // Obstacle Detected - Cannot move further (NEVER USED YET)
  // {
  //   // dist_obs_car = distance(obstacle.position.x, obstacle.position.y, current_position[0], current_position[1]);
  //   dist_obs_car = (dist_obst - set_distance);
  //   if(dist_obs_car < 1)
  //     desired_velocity=0;
  //   else
  //   {
  //     double temp_vel=sqrt(-2*deceleration*(dist_obs_car-1)) * 18/5;
  //     if(temp_vel < curr_vel)
  //       desired_velocity=temp_vel;
  //   }
  // }
  // if(mode==2) // curve so limit the velocity if required
  // {

  if(ratio>1.0)                                                // seems like some kind of 'p' regulator for reducing speed near sharp turns 
    ratio=1.0;
  else if(ratio < 0)
    ratio = 0;
  
  if(status.OmplSolStatus == 0)
  {
    desired_velocity = max_vel - ((ratio)*fabs(max_vel-min_vel));
  }
  cout<<"+++++++++ subtract vel:"<<(ratio)*fabs(max_vel-min_vel)<<endl;
  cout<<"+++++++++ set vel:"<<desired_velocity<<endl;
  // }
}

/*
(Testing Purpose)
E2o callback to calc distance travel based on velocity
This function sets current velocity
*/
void e2o_callback(const can::e2o_status::ConstPtr& e2o_info)
{
  // currvel = velocity->data;
  curr_vel =(int) (e2o_info->velocity);
  curr_time = ros::Time::now().toSec();
  set_distance += (curr_time - last_time)*curr_vel*5/18;            //calculating distance and converting it from km to m
  last_time=curr_time;
  // cout<<""set_distance<<'\n';
  // cout<<currvel<<'\n';
}

/*
Calculates current bearing and publishes heading msg
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

        current_goal[0] = waypoints[0][curr_goal_index];
        current_goal[1] = waypoints[1][curr_goal_index];

        //check if distance is greater than min distance only then update heading
        double dist = distance(prev_position[0], prev_position[1], current_position[0], current_position[1]);
        cout<<"distance from last bearing calculation : "<<dist<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        if(dist > min_dist_bearing)
        {
          current_gps_bearng = rad2deg(bearing(prev_position[0], prev_position[1], current_position[0], current_position[1]));
          prev_position[0] = gps_msg->latitude;
          prev_position[1] = gps_msg->longitude;

          goal_gps_bearng = rad2deg(bearing(current_position[0], current_position[1], current_goal[0], current_goal[1]));
          string gps_file_name = "gps_data123.asc";
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
          if(navigation_status == 0)
          {
            cout<<"STARTED NAVIGATION";
            navigation_status = 1; // started navigating
          }
        }

        double distance_to_goal = distance(current_position[0], current_position[1], current_goal[0], current_goal[1]);

        if(navigation_status == 1)
        {
          std_msgs::Float64 heading_msg;
          cout<<"distance from waypoint : "<<distance_to_goal<<"\n";
          if( distance_to_goal >= min_dist_goal) // trying to reach waypoint
          {
            if(curr_goal_index < waypoints[0].size() && curr_goal_index != -1)
            {
              cout<<"index : "<<curr_goal_index<<" ";
              printf("%lf %lf " , current_position[0], current_position[1]);
              cout<<"\n"; 
            }
            heading_msg.data =  heading_shift(current_gps_bearng, goal_gps_bearng);
          }
          else // reached waypoint
          {
            if(curr_goal_index>=0 && curr_goal_index < waypoints[0].size())// switch to next waypoint
            {
              heading_msg.data =  heading_shift(current_gps_bearng, goal_gps_bearng);
              curr_goal_index++;
              if(curr_goal_index == waypoints[0].size()) // reached final waypoint
              {
                cout<<"REACHED DESTINATION\n";
                navigation_status = 0; // not navigating
                desired_velocity = 0;
                navigation_status = 2;
                //exit(0);
              }
            }
          }

          pub_heading.publish(heading_msg);
          set_velocity();
          cout<<"+++++++++ Max velocity :"<<max_vel<<endl;
          cout<<"+++++++++ Min velocity :"<<min_vel<<endl;
          cout<<"+++++++++ Angle Sum  :  "<<sum_delta<<endl;
          cout<<"+++++++++ Desired velocity :"<<desired_velocity<<endl;
          cout<<"+++++++++ Ratio:"<<ratio<<endl;
          cout<<"-----------------------------\n";            
      }
    }
    else
    {
        cout<<"------------NO GPS FIX\n";
    }
}   

/*
(Testing Purpose)
Gets obstacle distance from dummy node
*/
void dist_callback(const std_msgs::Float64::ConstPtr& obs)
{
    dist_obst = obs->data;
    set_distance=0;
    deceleration=(-1*pow((curr_vel*5/18),2))/(2*dist_obst);
}


int main(int argc, char** argv)
{
  ros::init(argc, argv, "planner");
  if (argc < 5)
  {
    cout << "Enter the following arguments ..\n"
            "1) waypoints file\n"
            "2) min distance for bearing update (in meters) (5)\n"
            "3) min distance for goal satisfaction (in meters) (10)\n"
            "4) Smoothing window size (1)\n"
            "5) Threshold angle for turn detection (60)\n"
            "6) Max speed (kmph) (15)\n"
            "7) Min speed (kmph) (5)\n"
            "8) scale factor (2)\n"
            "\n";
    return -1;
  }
  
  get_waypoints(string(argv[1])); 
  min_dist_bearing = atof(argv[2]);
  min_dist_goal = atof(argv[3]);
  smooth_size = atoi(argv[4]);
  sum_threshold = atoi(argv[5]);
  max_vel = atoi(argv[6]);
  min_vel = atoi(argv[7]);
  scale_factor = atoi(argv[8]);
 
  ros::NodeHandle n;
  ros::NodeHandle private_nh("~");
  ros::TransportHints noDelay = ros::TransportHints().tcpNoDelay(true);

  ros::Subscriber vel_sub = n.subscribe<can::e2o_status>("/can_node/e2o_info", 1, e2o_callback, noDelay);                           //currently not under use...was initially used to calculate set_distance.
  ros::Subscriber dist_sub = n.subscribe<std_msgs::Float64>("/dist_obst", 1, dist_callback, noDelay);                               //currently not under use...was initially used for dummy obstacle distance
  ros::Subscriber gps_sub = n.subscribe<sensor_msgs::NavSatFix>("/mavros/global_position/global", 1000, gps_callback, noDelay);

  pub_des_vel = n.advertise<std_msgs::Float64>("/set_desired_velocity", 1);
  pub_heading = n.advertise<std_msgs::Float64>("/car_heading", 1);
  pub_OmplStatus = n.advertise<pixhwk_ctrl_node::OmplStatus>("/OmplStatus", 5);
  //pub_steering = n.advertise<pixhwk_ctrl_node::PixHawk_SteeringControl>("/PixHawk_SteeringControl", 1);


  int loopRate = 5;
  ros::Rate r(loopRate);
  
  std_msgs::Float64 des_vel_msg;
  curr_time = last_time = ros::Time::now().toSec();
  
  while(ros::ok())
  {
    // cin>>desiredVelocity;

    //mode of operation of vehicle
    
    des_vel_msg.data = (float)desired_velocity;
    
    //des_vel_msg.priority = 1;
    pub_des_vel.publish(des_vel_msg);
    ros::spinOnce();
 }
  return 0;
}
