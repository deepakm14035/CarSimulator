#include <ros/ros.h>
#include "e2o.h"
#include "../../can/include/can/can_driver.h"
#include <signal.h>
#include <sensor_msgs/Joy.h>
#include <can/e2o_status.h>

using namespace e2o_driver;

int desired_vel = 0;
int velocity = 0;
int throttle = 0;
int brakes = 0;
float steering = 0;
int steering_direc = 0;


void e2o_callback(const can::e2o_statusConstPtr& input)
{
  if(input->velocity < desired_vel)
  {
    std::cout<<"-------------------------1\n";
    throttle = 30;
    brakes = 0;
  }
  else if(input->velocity > desired_vel)
  {
    std::cout<<"-------------------------2\n";
    throttle = 0;
    brakes = 50;
  }
  else
  {
    std::cout<<"-------------------------3\n";
    throttle = 0;
    brakes = 0;
  }
}

void joy_callback(const sensor_msgs::JoyConstPtr& input)
{
  std::cout<<"---------------joystick message---------------\n";
  
  steering = input->axes[0];
  std::cout<<"original steering : "<<steering<<" \n";
    /*
  left is positive
  right is negative
  mapping (-1,1) to (0,40)
  */
  if(steering < 0)
  {
    steering_direc = 1;// right
    steering = -1 * steering;
  }
  else
  {
    steering_direc = 0;//left
  }
  steering = steering*40;
  std::cout<<"converted steering : "<<steering<<" \n";
  if(steering_direc==0)
  std::cout<<"direction : "<<"left"<<" \n";
  else
  std::cout<<"direction : "<<"right"<<" \n";
  std::cout<<"-----------------------------------------------\n";
}

int main(int argc, char** argv)
{
  if (argc < 1)
  {
    std::cout << "Enter the following arguments ..\n"
    "1) Desired velocity\n";
    return -1;
  }
  
  desired_vel = atoi(argv[1]);

  ros::init(argc, argv, "e2o_node", ros::init_options::NoSigintHandler);
  ros::NodeHandle node;
  ros::NodeHandle private_nh("~");

  //subscribe to joystick
  ros::Subscriber sub = private_nh.subscribe<sensor_msgs::Joy> ("/joy", 1, joy_callback);
  ros::Subscriber sub_e2o = private_nh.subscribe<can::e2o_status> ("/can_node/e2o_info", 1, e2o_callback);

  // start the driver
  e2o_driver::E2ODriver dvr(node, private_nh);
  //struct canfd_frame frame;
  struct can_frame frame;
  memset(frame.data,0,8);
  frame.can_dlc = 8;

  // loop until shut down or end of file
  ros::Rate r(100);
  while(ros::ok()) {
    ros::spinOnce();

    char buf[CL_CFSZ]; /* max length */
    
    frame.can_id = 0x76D;//accessories control command meesage
    frame.data[7] = (unsigned char)0x28;//wiper and horn valid
    can_driver::CANObject::sendMsgOverCAN(&frame);
    skk_sprint_canframe(buf , (struct canfd_frame*) &frame, 0, CAN_MAX_DLEN);
    std::cout<<"access   : " << buf << std::endl;
    for(int i=0; i<8; i++)
    {
      frame.data[i]= (unsigned char)0x00;
    }
    
    //-------------------------------------------------------------------------    
    frame.can_id = 0x770;//accelerating command
    frame.data[7] = (unsigned char)throttle;
    frame.data[7]=frame.data[7]<<1;
    frame.data[7] ^= (-1 ^ frame.data[7]) & (1 << 0);
    can_driver::CANObject::sendMsgOverCAN(&frame);
    skk_sprint_canframe(buf , (struct canfd_frame*) &frame, 0, CAN_MAX_DLEN);
    std::cout<<"throttle : "<< buf << "     "<<"throttle  value : "<<throttle<<std::endl;;
    for(int i=0; i<8; i++)
    {
      frame.data[i]= (unsigned char)0x00;
    }
    //-------------------------------------------------------------------------

    frame.can_id = 0x772;//breaking command
    frame.data[7] = (unsigned char)brakes;
    frame.data[7]=frame.data[7]<<1;
    frame.data[7] ^= (-1 ^ frame.data[7]) & (1 << 0);
    can_driver::CANObject::sendMsgOverCAN(&frame);
    skk_sprint_canframe(buf , (struct canfd_frame*) &frame, 0, CAN_MAX_DLEN);
    std::cout<<"breaks   : "<< buf << "     "<<"brakes  value : "<<brakes<<std::endl;;
    for(int i=0; i<8; i++)
    {
      frame.data[i]= (unsigned char)0x00;
    }

    //-------------------------------------------------------------------------
    
    frame.can_id = 0x774;//steering command
    frame.data[7] = (unsigned char)steering<<2;
    if(steering_direc == 1)//left
    {
      frame.data[7] ^= (-1 ^ frame.data[7]) & (1 << 0);
      frame.data[7] ^= (-0 ^ frame.data[7]) & (1 << 1);
    }
    else//right
    {
      frame.data[7] ^= (-0 ^ frame.data[7]) & (1 << 0);
      frame.data[7] ^= (-1 ^ frame.data[7]) & (1 << 1);
    }
    frame.data[6] = (unsigned char)0xC0;
    
    can_driver::CANObject::sendMsgOverCAN(&frame);
    skk_sprint_canframe(buf , (struct canfd_frame*) &frame, 0, CAN_MAX_DLEN);
    std::cout<<"steering : " << buf <<"  steering value : "<<steering<<std::endl;;

    for(int i=0; i<8; i++)
    {
      frame.data[i]= (unsigned char)0x00;
    }
    //-------------------------------------------------------------------------

    frame.can_id = 0x776;//lamp
    frame.data[6] = (unsigned char)0x20;
    frame.data[7] = (unsigned char)0x09;
    can_driver::CANObject::sendMsgOverCAN(&frame);
    skk_sprint_canframe(buf , (struct canfd_frame*) &frame, 0, CAN_MAX_DLEN);
    std::cout<<"lamp     : " << buf << std::endl;
    for(int i=0; i<8; i++)
    {
      frame.data[i]= (unsigned char)0x00;
    }
    //-------------------------------------------------------------------------

    frame.can_id = 0x778;//driving mode control messagef
    frame.data[6] ^= (-1 ^ frame.data[6]) & (1 << 2);
    frame.data[6] ^= (-1 ^ frame.data[6]) & (1 << 5);
    can_driver::CANObject::sendMsgOverCAN(&frame); 
    // frame.data[6] = (unsigned char)0x24;
    skk_sprint_canframe(buf , (struct canfd_frame*) &frame, 0, CAN_MAX_DLEN);
    std::cout<<"drive    : " << buf << std::endl;
    for(int i=0; i<8; i++)
    {
      frame.data[i]= (unsigned char)0x00;
    }
    //---------------------------------------------------------------------


    std::cout<<"--------------------------------------------------\n";

    r.sleep();
  }
  return 0;
}

E2ODriver::E2ODriver(ros::NodeHandle node, ros::NodeHandle private_nh)
{

}