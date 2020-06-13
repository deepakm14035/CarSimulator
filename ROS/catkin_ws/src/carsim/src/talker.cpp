#include<ros/ros.h>
#include<std_msgs/String.h>
//#include<sensor_msgs/Joy.h>
#include<sstream>

int main(int argc, char **argv){
	ros::init(argc,argv,"ActionInfoNode");
	ros::NodeHandle n;
	ros::Publisher chatter_pub=n.advertise<std_msgs::String>("ActionInfo",1000);
	ros::Rate loop_rate(2);
	int count=0;
	while(ros::ok()){
		//sensor_msgs::Joy msg({1,0,1},{0,5,4,3},{2,3,1,6});
		std_msgs::String msg;
		std::stringstream ss;
		ss << "hello!" << count;
		msg.data = ss.str();
		//msg.axes={2,1,4,3,5};
		std::cout<<msg<<std::endl;
		chatter_pub.publish(msg);
		ros::spinOnce();
		loop_rate.sleep();
		++count;
		if(count>5)count=0;
	}
	return 0;
}

