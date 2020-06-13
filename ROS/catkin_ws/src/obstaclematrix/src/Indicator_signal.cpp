#include "ros/ros.h"
#include "std_msgs/String.h"
#include "math.h"
#include <sensor_msgs/NavSatFix.h>
#include "nlgl.h"
#include "nlgl.cpp"
//deepak
//#include <can/e2o_status.h>
#include <vector>
//#include <pixhwk_ctrl_node/traffic_light.h>
#include <std_msgs/Float64.h>
//#include "pixhwk_ctrl_node/PixHawk_SteeringControl.h"
//#include <pixhwk_ctrl_node/desired_velocity.h>

using namespace std;

//----------------------------------------------------- Publisher -------------------------------------------------------
ros::Publisher waypoint_turn_pub;
ros::Publisher pub_des_vel;

//-------------------------------------------Variables for steering_callback()-------------------------------------------
double steering_val;
// double steering_stack[3];

//----------------------------------------------Variables for detect_turn()----------------------------------------------
int turn_count = 0;                                //to keep track of number of turn
float some_factor = 0;

//------------------------------------------Variables for detect_turn_apriori()------------------------------------------
// vector<double> waypt_turn_point[2];
// vector<std::string> waypt_turn_direction;

//-----------------------------------------Variables for goal_heading_callback()-----------------------------------------
double heading = 0;               //global variable for Heading angle that we get from /car_heading topic

//-----------------------------------------Variables for e2o_callback()-----------------------------------------
double curr_vel = 1, prev_vel = 0;
double curr_time, prev_time;
double acc = 0;

//-------------------------------------Variables for get_waypoints)()----------------------------------------------------
vector<double> waypoints[2]; // waypoints[0] contains x coordinates and waypoints[1] contains y coordinates
int waypoints_count = 0;
std::string waypoints_file;

//---------------------------------------------Variables for gps_callback()----------------------------------------------
bool need_to_read_waypoint_file = true;
bool is_indicator_on = false;
bool is_first_instance = true;
int curr_goal_index = 0;
int navigation_status = 0;
int bearing_update_distance = 2;                      //value in meters
double distance_to_upcomming_turn;
float switch_on_indicator_distance = 18.0;              //this variable holds distance cap for which a indicator should  blinking
double gps_fix_error = 5.0;             //distance error in gps value.
double prev_position[2] = {0,0};
double current_goal[2] = {0,0};
double initial_distance_for_ramping, initial_velocity_for_ramping;
std_msgs::String des_vel_msg;


//------------------------------------------------------Arbitary---------------------------------------------------------
double max_vel = 10;// Max velocity that the car can travel
double min_vel = 5;


void get_waypoints(string waypoints_file)
{
    string line;
    double prev[2], current[2];
    int i, flag = 0, count = 0, distance_interval = 9;  //distance_interval is the interval at which we are reading waypoints from file(default = 7)
    ifstream infile_waypoints(waypoints_file.c_str());

    if(!infile_waypoints.is_open())
    {
      cout<<"[Error] Unable to open waypoint file"<<endl;
      exit(1);
    }
    
    else
  {
      // cout<<"------------ GPS waypoints ------------------\n";
      while (std::getline(infile_waypoints, line))
      {
          std::istringstream iss(line);
        double lat,lon, x1, y1, d;

        if (!(iss >> lat >> lon)) 
        {  
          cout<<"[ERROR] not reading points"<<endl; break;                                    // error
        } 
        
        //adding gps coordinates to a waypoints vector which are at an interval of "distance_interval"
        if (flag == 0)
        {
          prev[0] = lat;
          prev[1] = lon;
          waypoints[0].push_back(prev[0]);
          waypoints[1].push_back(prev[1]);
          count++;
          flag = 1;
      }

        current[0] = lat;
        current[1] = lon;
        d = distance(prev[0], prev[1], current[0], current[1]);
        
        if(d >= distance_interval)
        {
          waypoints[0].push_back(current[0]);
          waypoints[1].push_back(current[1]);
          count++;
        prev[0] = current[0];
          prev[1] = current[1];
        }
        waypoints_count = count;
    }
    infile_waypoints.close();
  }

}


/*
Ramping the velocity;
Calculating the value of velocity after each iteration in order to linearly decrease/increase the velocity
## remove this when PID is implemented in pixhwkctrlNode
*/

double velocity_ramp(double desired_velocity, double Const_velocity, double Const_distance, double current_velocity, double distance_to_upcomming_turn)
{
  double vel_diff, vel_diff_dash, dist_diff, final_speed;
  vel_diff = curr_vel - desired_velocity;
  dist_diff = Const_distance - distance_to_upcomming_turn;
  vel_diff_dash = (int)(vel_diff*fabs(dist_diff))/(int)Const_distance;
  final_speed = (int)Const_velocity - vel_diff_dash;
  return final_speed;
}


std::string detect_turn(int index)
{
  double waypoint_turn_angle_threshold = 20.0;                      //define waypoint_turn_angle_threshold in degrees// if bearing is greater than this threshold a turn is detected.
  double delta, max_angle = 0, dist_from_last_wp = 0;
  float angle_1, angle_2;
  double x1, y1, x2, y2, x3, y3, px1, py1;
  float tollerance = 0.4;                           //error tollerance in distance ; range = [0,1]
  std::string direction;
  
  x1 = waypoints[0].at(index-1);
  y1 = waypoints[1].at(index-1);
  x2 = waypoints[0].at(index);
  y2 = waypoints[1].at(index);
  x3 = waypoints[0].at(index+1);
  y3 = waypoints[1].at(index+1);

  angle_1 = rad2deg(bearing(x1, y1, x2, y2));           //angle_unwrap(rad2deg(bearing(x1, y1, x2, y2)));
  angle_2 = rad2deg(bearing(x2, y2, x3, y3));
    
  delta = angle_2 - angle_1;
  // cout<<"angle difference = "<<delta<<endl;

  /**************************************************************
    SIGN CONVENTION
    if delta is positive, RIGHT turn is detected
    ELSE if delta is negative, LEFT turn is detected
  **************************************************************/

   if ((fabs(delta) >= waypoint_turn_angle_threshold) && (fabs(delta) <180))
      {
          turn_count++;
          some_factor = fabs(delta)/90;
          std::cout<< "\t\tturning factor = " << some_factor <<std::endl;
        // dist_from_last_wp = distance(px1, py1, x2, y2);
          cout<<"\n\n-------TURN DETECTED------\n\n"<<endl;
        if( delta < 0 )
          {
            direction = "left";
            return direction;
       //     setprecision(14);
        // fprintf(mf, "prev_lat= %lf ,prev_lon= %lf,lat= %lf ,lon= %lf, dist_from_last_wp= %lf,  dir= left %d,   delta(%lf) = angle_1(%lf) - angle_2(%lf) \n",x1, y1, x2, y2, dist_from_last_wp, turn_count-1, delta, angle_1, angle_2);
          }
      
          else if( delta > 0 )
          {
            direction = "right";
            return direction;
       //     setprecision(14);
        // fprintf(mf, "prev_lat= %lf ,prev_lon= %lf,lat= %lf ,lon= %lf, dist_from_last_wp= %lf,  dir= right %d,  delta(%lf) = angle_1(%lf) - angle_2(%lf) \n",x1, y1, x2, y2, dist_from_last_wp, turn_count-1, delta, angle_1, angle_2);
          }
      }

    else
    {
      some_factor = 0;
    }
}


//void steering_callback(const pixhwk_ctrl_node::PixHawk_SteeringControl msg)
//{
                // ofstream mf;
                // mf.open("/home/pratique/steeringwithindist.txt", ios::out | ios::app);
                // if(!mf.is_open())
                //  {
                //    cout<<"couldn't open max_angle file"<<endl;
                //    exit(1);
                //  }

  //steering_val = msg.steering;
  // if(distance_to_upcomming_turn < 10.0 && relevant_turn_index < turn_count-1 )           //default val of distance to upcomming turn <0.5
  //   {
  //     mf<<"steering = "<<msg.steering<<" turn = "<<relevant_turn_index<<'\n';
  //   }

// mf.close();
//}
//deepak
//void e2o_callback(const can::e2o_status::ConstPtr& e2o_info)
//{
//  curr_vel =((int) (e2o_info->velocity) );
  // std::cout<<"callback_curr_vel"<<curr_vel<<std::endl;
  // curr_time = ros::Time::now().toSec();
  // acc = (curr_vel - prev_vel)/(curr_time - prev_time);
  // prev_time=curr_time;
  // prev_vel = curr_vel;  
//}


void goal_heading_callback(const std_msgs::Float64::ConstPtr& heading_msg)  // reads heading_msg and assigns it to a global variable named heading 
{                                     
  heading = heading_msg->data;                      // HEADINING IS SIMPLY AN ANGLE IN 0 TO 360 DEGREES.
}


void gps_callback(const sensor_msgs::NavSatFix::ConstPtr& gps_msg)
{
  int i; 
  double current_position[2] = {0,0};
  double desired_velocity;
  //des_vel_msg.node_id = 5;

  std_msgs::String temp;
  std:stringstream ss;
  std::string turn_direction;
                
                // ofstream mf;
                // mf.open("/home/pratique/steering_val.txt", ios::out | ios::app);
                // if(!mf.is_open())
                //  {
                //    cout<<"couldn't open file"<<endl;
                //    exit(1);
                //  }

  if(gps_msg->status.status == 0)
  {
    if(need_to_read_waypoint_file)
        {
            prev_position[0] = gps_msg->latitude;
            prev_position[1] = gps_msg->longitude;
            get_waypoints(waypoints_file);
            need_to_read_waypoint_file = false;
            return;
        }

        current_position[0] = gps_msg->latitude;
        current_position[1] = gps_msg->longitude;
        // cout<<"curr pos  || lat = "<<setprecision(11)<<current_position[0]<<" lon = "<<setprecision(11)<<current_position[1]<<endl;
        current_goal[0] = waypoints[0][curr_goal_index];
        current_goal[1] = waypoints[1][curr_goal_index];
        // cout<<"curr GOAL || lat = "<<setprecision(11)<<current_goal[0]<<" lon = "<<setprecision(11)<<current_goal[1]<<endl;
        
        
        if(navigation_status == 1)
          {
            for(i = curr_goal_index; i < waypoints_count-1; ++i)
            {
              double distance_to_waypoint = distance(current_position[0], current_position[1], waypoints[0].at(i), waypoints[1].at(i));
              // cout<<"distance_to_waypoint = "<<distance_to_waypoint<<endl;
              if(distance_to_waypoint <= switch_on_indicator_distance && curr_goal_index>0)
                {
                  if(is_first_instance)
                  {
                    initial_distance_for_ramping = distance_to_waypoint;
                    initial_velocity_for_ramping = curr_vel ;
                    /*
                    setting flag to zero as above values have been initialized
                    this flag will be reset to 1 once the current waypoint has been reached
                    */
                    is_first_instance = false;                        
                  }

                  turn_direction = detect_turn(i);
                  if(turn_direction == "left" || turn_direction == "right")
                    {
                        ss.str("");
                        ss << turn_direction;
                        distance_to_upcomming_turn = distance(current_position[0], current_position[1], waypoints[0].at(i), waypoints[1].at(i));
                        desired_velocity = max_vel - some_factor*(max_vel - min_vel);
                        std::cout<< "desired_velocity = " << desired_velocity <<std::endl;
                        is_indicator_on = true;
                        break;
                    }

                  else
                    {
                      ss.str("");
                      ss << "straight";
                      is_indicator_on = false;
                    }

                }

                else
                {
                  // ignoring out of reach waypoints that are far away(distance )
                  break;
                }

            }

            //publishing turn 
            temp.data = ss.str();
            waypoint_turn_pub.publish(temp);

            /*
              if distance of turn waypoint and current location is less than Switch_on_indicator_distance (also considering GPS fix error)
            */
            if(distance_to_upcomming_turn <= switch_on_indicator_distance && distance_to_upcomming_turn > gps_fix_error /*&&  desired_velocity < curr_vel*/)
            {
              double Const_distance = distance_to_upcomming_turn - gps_fix_error;
              //des_vel_msg.velocity = velocity_ramp(desired_velocity, initial_velocity_for_ramping, initial_distance_for_ramping, curr_vel, Const_distance);
              //std::cout<<" ramped velocity = " <<des_vel_msg.velocity <<"\n distace-gps_fix = "<< distance_to_upcomming_turn <<std::endl;
              //des_vel_msg.priority = 20;
            }

            else
            {
              //des_vel_msg.velocity = max_vel;
              //des_vel_msg.priority = 0;
            }

            // if(distance_to_upcomming_turn <= switch_on_indicator_distance && distance_to_upcomming_turn >0)
            // {
            //   if(is_indicator_on)
            //   {
            //     mf<<"wayt_pos = "<< waypoints[0].at(i)<<" " <<waypoints[1].at(i) <<"   steering = "<<steering_val<<'\n';
            //   }
            // }
            //publish reduced velocity
            //pub_des_vel.publish(des_vel_msg);

            //incrementing waypoint index if needed
            double distance_to_goal = distance(current_position[0], current_position[1], current_goal[0], current_goal[1]);
            if( distance_to_goal <= gps_fix_error) // reached waypoint
                {
                  if(curr_goal_index>=0 && curr_goal_index < waypoints_count)// switch to next waypoint
                    {
                      curr_goal_index++;
                      // mf<<'\n';
                      is_first_instance = true;                       //next waypoint, reset flag to reinitialize
                      if(curr_goal_index == waypoints_count) // reached final waypoint
                        {
                          cout<<"REACHED DESTINATION\n";
                          navigation_status = 2;// navigation ended
                          
                        }
                    }
                }
          }


          //check if distance is greater than min distance only then update previous position`
        double dist = distance(prev_position[0], prev_position[1], current_position[0], current_position[1]);
        // cout<<"distance from last bearing calculation : "<<dist<<"\n";
            if(dist > bearing_update_distance)                                      //check for updating the previous position for calculating new bearing
            {
              prev_position[0] = current_position[0];
              prev_position[1] = current_position[1];

              if(navigation_status == 0)
              {
                // cout<<"STARTED NAVIGATION";
                navigation_status = 1; // navigation started!
              }
            }
  }

  else
  {
    cout<<"------------NO GPS FIX\n";
  }
  
  // mf.close();
}




int main(int argc, char** argv)
{
	ros::init(argc, argv, "indicator_signal");
	ros::NodeHandle n;
	ros::TransportHints noDelay = ros::TransportHints().tcpNoDelay(true);
 
	if (argc < 4)
	{
		cout << "1) name of waypoints file \n"
         << "2) velocity upper bound \n"
         << "3) Velocity lower bound (if upper bound > 8;recommended lower bound = 25 percent of upper bound) \n" ;
        return -1;
	}

  waypoints_file =  string(argv[1]);
  max_vel = atoi(argv[2]);
  min_vel = atoi(argv[3]);

  // curr_time = last_time = ros::Time::now().toSec();

  //********************************** Setting up publishers ****************************************
  waypoint_turn_pub = n.advertise<std_msgs::String>("/waypoint_turn_direction",1);									//
  //pub_des_vel = n.advertise<pixhwk_ctrl_node::desired_velocity>("/set_desired_velocity", 1);
  
  //********************************** Setting up subscribers ****************************************
  //deepak
  //ros::Subscriber vel_sub = n.subscribe<can::e2o_status>("/can_node/e2o_info", 1, e2o_callback, noDelay);
  ros::Subscriber gps_sub = n.subscribe<sensor_msgs::NavSatFix>("/mavros/global_position/global", 1000, gps_callback, noDelay);
  ros::Subscriber heading_sub = n.subscribe("/car_heading",1,goal_heading_callback);				
  //ros::Subscriber steering_sub = n.subscribe<pixhwk_ctrl_node::PixHawk_SteeringControl>("/PixHawk_SteeringControl",10,steering_callback);
  //ros::Subscriber traffic_light_sub= n.subscribe<pixhwk_ctrl_node::traffic_light>("/traffic_light_data", 1, traffic_light_callback); 
  
  while (ros::ok())
  {
  	ros::spin();
  }

  return 0;
}
