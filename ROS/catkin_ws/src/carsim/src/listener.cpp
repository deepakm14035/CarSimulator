#include<ros/ros.h>
#include<std_msgs/String.h>
#include<string>
#include<vector>
#include<math.h>
#include <iostream>
#include <fstream>
using namespace std;

ros::Publisher pub;
std::string data="";
int m_size=51;
int Matrix[51][51]={0};
static int m_size_for_center;			//defualt value=101;				//matriz size...value is odd as it is easy to find median.
int home_x, home_y;
int goal_x,goal_y;
int lidar_x, lidar_y;

int min_distance_for_goal = 2; 											//minimum threshold of goal length
int max_distance_goal = 50; 											//max goal length

bool writeToFile=true;
const string ompl_script = "/home/deepak/swarath/Ompl.sh";
const string goal_file = "/home/deepak/swarath/goal.txt";
double curr_vel=0;
double steering_command;
double my_position[2];
double steering_ratio=1.0;
double curr_accel=0;
double start_time=0.0;
double prev_error_throttle=0;
double sum_error=0;
double max_acceleration=2;
double last_time=0;
double heading=0;
int flag=0;
double desired_vel=15;
double desired_accel=0;

#define PI 3.14159265

double rad2deg(double ang)
{
  return(ang*180/PI);
}

void writeToMatrix(){
	FILE *f=fopen("/home/deepak/swarath/matrix.txt","r+");
	char c=fgetc(f);
	int i=0,j=0;
	bool isFirstLine=true;
	while(c!=EOF){
		if(c!=' ')
		std::cout << c <<' ';//<< "["<<i<<","<<j<<"]";
		if(isFirstLine && c=='\n')
			isFirstLine=false;
		else if(!isFirstLine&&(c=='0'||c=='1')){
			if(c=='0')Matrix[i][j++]=0;
			else Matrix[i][j++]=1;
		}else if(!isFirstLine && c=='\n')
			i++;
		//std::cout <<',';
		if(j>=m_size)j=0;
		c=fgetc(f);
	}
	
}

void printOccupancyMap(std::vector<std::vector<std::string> > map){
	std::cout << "writing to file\n";
	FILE *f = fopen("/home/deepak/swarath/matrix.txt","w");
	if(writeToFile){
		//std::cout << "length-\n";
		fprintf(f,"%lu %lu\n",map.size(),map.size());
	}
	std::cout << "occupancy map - \n";
	for(int i=0;i<map.size();i++){
		for(int j=0;j<map.at(i).size();j++){
			//std::cout << map.at(i).at(j) << " ";
			if(writeToFile)
				fprintf(f,"%c ",map.at(i).at(j).c_str()[0]);
		}
		if(writeToFile && i!=map.size()-1)
			fprintf(f,"\n");
		//std::cout << "\n";
	}
	fclose(f);
}

int get_goal_quadrant(double heading)										// calculates the quadrant in which the goal is based on heading it is gets
{
	int goal_quadrant = 0;

	if(heading>0 && heading<=90)
		goal_quadrant=0;		
	else if(heading>90 && heading<=180)
		goal_quadrant=1;
	else if(heading<0 && heading>=-90)						
		goal_quadrant=2;
	else if(heading<-90 && heading>=-180)
		goal_quadrant=3;

	return goal_quadrant;
}

int get_local_quadrant(double heading)										//same as get_goal_guadrant now, was different function initially
{
	int local_quadrant = 0;

	if(heading>0 && heading<=90)
		local_quadrant=0;		
	else if(heading>90 && heading<=180)
		local_quadrant=1;
	else if(heading<0 && heading>=-90)
		local_quadrant=2;
	else if(heading<-90 && heading>=-180)
		local_quadrant=3;

	return local_quadrant;
}



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

void get_goal()
{
	// cout<<"---------------heading : "<<heading<<"\n";

	/*****************************************************************************************************************************
	******************************************************************************************************************************						  
								  				 			[FRONT]
																(0)
													[home_x, m_size]
																|
																|
																|
																|
																|
																|																 _
 _																|																|R|
|L|																|																|I|
|E|		(90)	[m_size, home_y]-------------------------[home_x, home_y]----------------------------[0, home_y]	(-90) 		|G|
|F|																|																|H|
|T|																|																|T|
																|
																|
																|
																|
																|
																|
																|
														[home_x, 0]
																(180,-180)
	*******************************************************************************************************************************
	******************************************************************************************************************************/
	double min_heading_diff = 10000;					//some arbitary large values 				
	double final_local_heading = 0.0;
	double local_heading;
	double local_distance;
	int a,b,q,r;							//loop variables (ignore)
	double max_dist = 0;
	
	for(a = 0; a < m_size; ++a)									   
	{	
		for (b = 0; b < m_size; ++b)				//select goal only in first two quadrants
		{
			local_heading = rad2deg(atan2(a - home_x, b - home_y));			// CALCULATING HEADING AND DIST FOR EACH POINT IN OUR LOCAL COORDINATE SYSTEM
			local_distance = sqrt((a - home_x)*(a - home_x) + (b - home_y)*(b - home_y));
			// if(local_distance > max_dist)
			// 	max_dist = local_distance;
			
			if(Matrix[a][b]==0)							//if cell is free
			{
				int goal_padding = 8;//changed from 8							//this padding accounts for the padding around the free cell
				int flag_goal = 0;						//This flag is appropriated to check if a point in the matrix is obstacle or not
				for(q= -goal_padding; q <= goal_padding; q++)
           			for(r= -goal_padding; r <= goal_padding; r++)
               			{
               				if(q+a >=0 && q+a < m_size-1 && r+a>=0 && r+a< m_size-1)					//limiting the "x plus pad_around_goal" value our coordinate space i.e. withing the range of 0 to 100
                    		{
                    			if(q+b >=0 && q+b < m_size-1 && r+b>=0 && r+b< m_size-1)				//limiting the "y plus pad_around_goal" value our coordinate space i.e. withing the range of 0 to 100
                    			{
                    				if( Matrix[q+a][r+b]==1 )					
                    				{
                    					flag_goal=1;							// this indicates that there is an obstacle at this point i.e. aroun Matrix[a][b]
							break;
                    				}
                    			}
                    		}
                    	}

				if(!flag_goal&&a>2 && a < m_size-3 && b > 2 && b < m_size-3){//std::cout<<"----("<<a<<","<<b<<")"<< local_heading<<" "<<heading<<"\n";												//if no obstacle is found 
					if(fabs(local_heading - heading)<= min_heading_diff&&b>20)	// fabs calculates the absolute value 	
					{	
						if(local_distance >= min_distance_for_goal && local_distance <= max_distance_goal)	// checking if the local_distance is within our specified constraints 
						{	
							//if((get_goal_quadrant(heading)==0&&get_goal_quadrant(heading)==2)|| (get_local_quadrant(local_heading)==0&& get_local_quadrant(local_heading)==2))	//checking if the both the calculated heading and the desired heading is in same quadrant	
							//{
								min_heading_diff = fabs(local_heading - heading);		// taking each point in the matrix and finding..
								goal_x = a;												// ..the one that alligns with our desired heading..
								goal_y = b;												// ..by reducing the error b/w desired heading and..
								final_local_heading = local_heading;					// ..local heading of the point
							//}
						}
					}
				}
			}
		}
	}
	// cout<<"max local distance"<<max_dist<< endl;
	cout<<"---------------local heading : "<<final_local_heading<<"\n";
	// if(min_heading_diff > max_dev)
	// {
	// 	cout<<"++++++MIN heading : "<<min_heading_diff<<"\n";
	// 	//if cannot find heading just move forward
	// 	goal_x = home_x;
	// 	goal_y = home_y + 5;
	// }
}

void setGoalPath(){
	get_goal();	
	//ROS_INFO("Home(x,y): %d %d",home_x,home_y);
	//ROS_INFO("Goal(x,y): %d %d",goal_x,goal_y);
	
	// for visualising home postion, lidar postion and goal position on rviz
	//23/11/19
	//v[home_x*m_size + home_y] = 10;
	//v[(lidar_x)*m_size + (lidar_y)] = 30;
	//v[goal_x*m_size + goal_y] = 90;
	
	
	//creating the text file with goal and home coordinates
	FILE *g = fopen(goal_file.c_str(), "w");								//enter path to goal file here
	if (g == NULL)	
	{
	    printf("Error opening goal file!\n");
	    exit(1);
	}
	fprintf(g, "%d %d\n", home_x,home_y);
	fprintf(g, "%d %d", goal_x,goal_y);
	fclose(g);
	cout<<"Goal :"<<goal_x<<' '<<goal_y<<'\n';

}

bool sendAction(){
	ifstream iFile;
	//std::cout << "\nsending action\n";
	iFile.open("/home/deepak/swarath/trajectory_ompl.txt");                           // change file path here to run this code on nuc ( /home/nuc1/iiitd/swarath/trajectory_ompl.txt )
	//std::cout << "finished reading file\n";
	double steering_command,x,y;
	int ctr = 0;
    while((iFile >> x) && (iFile >> y))
      {
        ctr++;
        if(ctr == 5) // Read the third point
        {
          std::cout<<"Goal : "<<x<<' '<<y<<'\n'<<"current position : "<<home_x <<" "<<home_y<<" " << atan2((x-home_x),y-home_y)<<"\n";
          // Calculate Normalized Steering and multiply by steering ratio.
          steering_command = (((rad2deg(atan2((x - home_x), y - home_y))))*steering_ratio)/40;
          // cout<<"Steering Ratio : "<<steering_ratio<<endl;
          //std::cout<<"original steering "<<90-rad2deg(atan2(y - my_position[1], x - my_position[0]))<<'\n';
          // ROS_INFO("goal = %lf,%lf",my_goal[0],my_goal[1]);
          // ROS_INFO("waypoint = %lf,%lf",x,y);  
          std::cout<<"converted steering : "<< steering_command;
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

//    pixHwkThrottle.throttle = throttle_command;
//    pixHwkSteer.steering = steering_command;        

//    pub_steering.publish(pixHwkSteer);
//    pub_throttle.publish(pixHwkThrottle);
	std_msgs::String dataToSend;
	std::stringstream ss;
	ss << steering_command << "," << throttle_command;
	std::cout<<"\nsteering-"<<steering_command << ", throttle-"<<throttle_command<<"\n";
	dataToSend.data = ss.str();
	pub.publish(dataToSend);
	std::cout << "data sent!\n";
}

void chatterCallback(const std_msgs::String::ConstPtr& msg){
	//std::cout << "message received- " << msg->data.c_str() << std::endl;
	std::string message=msg->data.c_str();
	std::vector<std::vector<std::string> > occupancy_map;
	std::string s;
	int num=0;
	bool isFirst=true;
	int rowCt=0;
	int gridSize=0;
	//std::cout << "\n" << message.size();
	std::vector<std::string> vMapRow;
	for(int i=0;i<message.size();i++){
		if(message.at(i) != ',' && message.at(i) != '.'){
			//std::cout << message.at(i);
			if(message.at(i)=='3'){
				home_x=rowCt;
				home_y=num;
				//std::cout << home_x << "," << home_y << "\n";
				message[i]='0';
			}
			s += message.at(i);

		}else{
			//std::cout << message.at(i);
			if(message.at(i)=='3'){
				home_x=rowCt+1;
				home_y=num+1;
				my_position[0]=home_x;
				my_position[1]=home_y;
				std::cout << home_x << "," << home_y << "\n";
				message[i]='0';
			}
			//std::cout << s;
			if(!isFirst){
				vMapRow.push_back(s);
				s="";
			}
			num++;
			if(isFirst){
				num=0;
				//s="";
				std::cout << "\n\n\nmessage number - " << s << "--------------------\n";
				s="";
				isFirst=false;
			}
		
			if(message.at(i)=='.'){
				rowCt++;
				occupancy_map.push_back(vMapRow);
				vMapRow.clear();
				gridSize=num;
				num=0;
				//std::cout << "\n";
				if(rowCt==gridSize)break;
			}
		}
		//std::cout << num;
	}
	m_size=occupancy_map.size();
	m_size_for_center = m_size;
	std::cout << "occ mmap saved\n";
	printOccupancyMap(occupancy_map);
	std::cout << "write to matrix\n";
	writeToMatrix();
	std::cout << "set goal path\n";
	setGoalPath();
	std::cout << "computing path\n";
	system(ompl_script.c_str());
	std::cout << "finished computing path";
	sendAction();
	/*
	//std::cout << occupancy_map.at(0).at(0);
	int nEgoPos=0;
	for(int i=0;i<4;i++){
		if(occupancy_map.at(i).at(0).at(0)=='3'){
			nEgoPos=i;
			break;
		}
	}
	bool bChangeDirection=false;
	int direction=-1;
	std::cout << "\n ego pos - " << nEgoPos << "\n";
	for(int i=1;i<8;i++){
		//std::cout << occupancy_map.at(nEgoPos).at(i).at(0) << "\n";
		if(occupancy_map.at(nEgoPos).at(i).at(0)!='0'){
			if(i<6){
				for(int row=nEgoPos-1;row>=0;row--){
					int closestBlockedPos=0;
					for(int col=1;col<i;col++){
						if(occupancy_map.at(row).at(col).at(0)!='0'){
							closestBlockedPos=col;break;
						}
					}if(closestBlockedPos>3){
						direction=row;
						break;
					}
				}
				if(direction!=-1) break;
				for(int row=nEgoPos+1;row<4;row++){
					int closestBlockedPos=0;
					for(int col=1;col<10;col++){
						if(occupancy_map.at(row).at(col).at(0)!='0'){
							closestBlockedPos=col;break;
						}
					}if(closestBlockedPos>3){
						direction=row;
						break;
					}
				}
				if(direction!=-1) break;
			}
		}
	}
	//std::cout << "direction - " << direction << "\n";
	if(direction==-1)
		direction=3;
	else if(direction<nEgoPos) direction=1;
	else direction=5;
	if(direction<3)
		std::cout << "direction - left\n";
	else if(direction==3)
		std::cout << "direction - forward\n";
	else
		std::cout << "direction - right\n";
	std::stringstream ss;
	ss << direction;
	std_msgs::String dataToSend;
	dataToSend.data=ss.str();
	pub.publish(dataToSend);
	ros::spinOnce();
	data=ss.str();
	//loop_rate.sleep();
	*/
}
//ros::Rate loop_rate(30);
int main(int argc, char ** argv){
	ros::init(argc,argv,"AlgoNode");
	ros::NodeHandle n;
	ros::Subscriber sub=n.subscribe("VehicleInfo",1000,chatterCallback);
	pub=n.advertise<std_msgs::String>("ActionInfo",1000);
	ros::Rate loop_rate(30);
	ros::spin();
	/*while(ros::ok()){
		//std::cout << "data-" << data;
		if(!data.empty()){
			std::cout << "sending data\n";
			std_msgs::String dataToSend;
			dataToSend.data=data;
			pub.publish(dataToSend);
			ros::spinOnce();
			loop_rate.sleep();
			data="";
		}
	}*/
	return 0;
}
