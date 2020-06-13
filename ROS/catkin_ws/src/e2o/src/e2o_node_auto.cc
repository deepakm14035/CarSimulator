#include <ros/ros.h>
#include "e2o.h"
#include "../../can/include/can/can_driver.h"
#include <signal.h>
#include "pixhwk_ctrl_node/PixHawk_ThrottleControl.h"
#include "pixhwk_ctrl_node/PixHawk_SteeringControl.h"
#include <sensor_msgs/Joy.h>
#include <can/e2o_status.h>
#include "std_msgs/String.h"


using namespace e2o_driver;

int input_throttle;
int input_brakes;
int input_velocity;

int set_throttle;
int set_brakes;

int throttle = 0;
int brakes = 0;
float steering = 0;
int steering_direc = 0;
int delta;
char turn_direction = 's';                  // 'l'=left, 'r'=right, 's'=straight

int flag = 1;

void set_bit(unsigned char &x, int bit, int pos)
{
  x ^= (-bit ^ x) & (1 << pos);
}

void e2o_callback(const can::e2o_statusConstPtr& input)
{
  // if((input->velocity± < input_velocity - delta )||(input->velocity > input_velocity + delta ))
  // {
  //   std::cout<<"---------out range----------------2\n";
  //   throttle = set_throttle;
  //   brakes = set_brakes;
  // }
  // else
  // {
  //   std::cout<<"----------in range---------------1\n";
  //   throttle = input_throttle;
  //   brakes = set_brakes;

  // }
}


void throttle_callback(const pixhwk_ctrl_node::PixHawk_ThrottleControl::ConstPtr& pixhwk_msg)
{
  std::cout<<"Pixhawk throttle : "<<pixhwk_msg->throttle<<" \n";
  if(pixhwk_msg->throttle >= 0)
  {
    //set throttle
    throttle = (int)(pixhwk_msg->throttle * 100);
    if(throttle>=80)
      throttle =80;
    //unset brake
    brakes = 0;
  }
  else
  {
    //unset throttlec
    throttle = 0;
    brakes = (int)(pixhwk_msg->throttle * 100*-1);
    //set brake
    // brakes = (int)pixhwk_msg->throttle * 100*-1;
    // brakes = throttle*100*-1;
  }
}

void joy_callback(const sensor_msgs::JoyConstPtr& input)
{
  
  // std::cout<<"---------------joystick message---------------\n";
  
  // steering = input->axes[0];
  // std::cout<<"original steering : "<<steering<<" \n";
    
  // // left is positive
  // // right is negative
  // // mapping (-1,1) to (0,40)
  
  // if(steering < 0)
  // {
  //   steering_direc = 1;// right
  //   steering = -1 * steering;
  // }
  // else
  // {
  //   steering_direc = 0;//left
  // }
  // steering = steering*40;
  // std::cout<<"converted steering : "<<steering<<" \n";
  // if(steering_direc==0)
  // std::cout<<"direction : "<<"left"<<" \n";
  // else
  // std::cout<<"direction : "<<"right"<<" \n";


  // // set_brakes = input->axes[2];
  // // std::cout<<"original brakes : "<<set_brakes<<" \n";  
  // // //mapping (-1,1) to (0,1)
  // // set_brakes = ((set_brakes + 1.0) / 2.0)*100;
  // // std::cout<<"converted brakes : "<<brakes<<" \n";

  // // set_throttle = input->axes[1];
  // // std::cout<<"original throttle : "<<set_throttle<<" \n";
  // // //mapping (-1,1) to (0,1)
  // // set_throttle = ((set_throttle + 1.0) / 2.0)*100;
  // // std::cout<<"converted set_throttle : "<<set_throttle<<" \n";


  // std::cout<<"-----------------------------------------------\n";

}

void indicator_light_callback(const std_msgs::String turn)
{
  if (turn.data == "left")
  {
    turn_direction = 'l'; 
  }
  else if (turn.data == "right")
  {
    turn_direction = 'r';
  }
  else if (turn.data == "straight" || turn.data =="")
  {
    turn_direction = 's';
  }
}

void steering_callback(const pixhwk_ctrl_node::PixHawk_SteeringControl::ConstPtr& pixhwk_msg)
{
    steering = pixhwk_msg->steering;
  if(steering < -1)
  {
    steering = -1.0;
    // std::cout<<"Invalid Steering values from pixhawk : "<< pixhwk_msg->steering<<"\n";
  }
  else if(steering > 1)
  {
    steering = 1.0;
  }
  
  {
    std::cout<<"original steering : "<<steering<<" \n";
    
    // left is positive
    // right is negative
    // mapping (-1,1) to (0,40)
    
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
  }
}

int main(int argc, char** argv)
{
  // input_throttle = atof(argv[1]);
  // input_brakes = atof(argv[2]);
  // input_velocity = atof(argv[3]);
  // delta = atof(argv[4]);

  ros::init(argc, argv, "e2o_node_auto", ros::init_options::NoSigintHandler);
  ros::NodeHandle node;
  ros::NodeHandle private_nh("~");

  ros::Subscriber steering_sub = private_nh.subscribe<pixhwk_ctrl_node::PixHawk_SteeringControl>("/PixHawk_SteeringControl", 2000, steering_callback);
  ros::Subscriber throttle_sub = private_nh.subscribe<pixhwk_ctrl_node::PixHawk_ThrottleControl>("/PixHawk_ThrottleControl", 2000, throttle_callback);
  ros::Subscriber indicator_light_sub = private_nh.subscribe<std_msgs::String>("/waypoint_turn_direction", 100, indicator_light_callback); //std_msgs::String>("/waypoint_turn_direction",1);                  //
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
  while(ros::ok())
  {
    ros::spinOnce();

    char buf[CL_CFSZ]; // max length 
    
    //-------------------------------------------------------------------------    
    //FIXED
    frame.can_id = 0x76D;//accessories control command meesage
    frame.data[7] = (unsigned char)0x28;//wiper and horn valid
    can_driver::CANObject::sendMsgOverCAN(&frame);
    skk_sprint_canframe(buf , (struct canfd_frame*) &frame, 0, CAN_MAX_DLEN);
    std::cout<<"access   : " << buf << std::endl;
    memset(frame.data,0,8);
    
    //-------------------------------------------------------------------------    
    //AUTONOMOUS
    frame.can_id = 0x770;//accelerating command
    frame.data[7] = (unsigned char)throttle;
    frame.data[7]=frame.data[7]<<1;
    set_bit(frame.data[7], 1, 0);
    can_driver::CANObject::sendMsgOverCAN(&frame);
    skk_sprint_canframe(buf , (struct canfd_frame*) &frame, 0, CAN_MAX_DLEN);
    std::cout<<"throttle : "<< buf << "     "<<"throttle  value : "<<throttle<<std::endl;
    memset(frame.data,0,8);

    //-------------------------------------------------------------------------
    //FIXED
    frame.can_id = 0x772;//breaking command
    frame.data[7] = (unsigned char)brakes;
    frame.data[7]=frame.data[7]<<1;
    set_bit(frame.data[7], 1, 0);
    can_driver::CANObject::sendMsgOverCAN(&frame);
    skk_sprint_canframe(buf , (struct canfd_frame*) &frame, 0, CAN_MAX_DLEN);
    std::cout<<"breaks   : "<< buf << "     "<<"breaks  value : "<<brakes<<std::endl;
    memset(frame.data,0,8);

    //-------------------------------------------------------------------------
    //AUTONOMOUS
    frame.can_id = 0x774;//steering command
    frame.data[6] = (unsigned char)0xC0;
    frame.data[7] = (unsigned char)steering<<2;
    if(steering_direc == 1)//left
    {
      set_bit(frame.data[7], 1, 0);
      set_bit(frame.data[7], 0, 1);
    }
    else//right
    {
      set_bit(frame.data[7], 0, 0);
      set_bit(frame.data[7], 1, 1);
    }
    can_driver::CANObject::sendMsgOverCAN(&frame);
    skk_sprint_canframe(buf , (struct canfd_frame*) &frame, 0, CAN_MAX_DLEN);
    std::cout<<"steering : "<< buf << "     "<<"steering  value : "<<steering<<std::endl;
    if(steering_direc==0)
    std::cout<<"direction : "<<"left"<<" \n";
    else
    std::cout<<"direction : "<<"right"<<" \n";
    memset(frame.data,0,8);

    //-------------------------------------------------------------------------
    //FIXED
    frame.can_id = 0x776;//lamp, indicator,etc
    frame.data[6] = (unsigned char)0x20;                //why is break light valid?
    frame.data[7] = (unsigned char)0x09;
    std::cout<<"******************* direction = "<< turn_direction <<std::endl;
    if(turn_direction == 'r')//right
    {
      frame.data[7] = (unsigned char)0x19;
    }
    else if(turn_direction == 'l')//left
    {
      frame.data[7] = (unsigned char)0x89;
    }
    else if(turn_direction == 's')//straight
    {
      frame.data[7] = (unsigned char)0x09;
    }
    can_driver::CANObject::sendMsgOverCAN(&frame);
    skk_sprint_canframe(buf , (struct canfd_frame*) &frame, 0, CAN_MAX_DLEN);
    std::cout<<"indicator/lamp     : " << buf << std::endl;
    memset(frame.data,0,8);

    //-------------------------------------------------------------------------
    //FIXED
    frame.can_id = 0x778;//driving mode control messagef
    frame.data[6] = (unsigned char)0x24;//drive gear
    can_driver::CANObject::sendMsgOverCAN(&frame); 
    skk_sprint_canframe(buf , (struct canfd_frame*) &frame, 0, CAN_MAX_DLEN);
    std::cout<<"drive    : " << buf << std::endl;
    memset(frame.data,0,8);
    
    std::cout<<"--------------------------------------------------\n";
    r.sleep();
  }
  return 0;
}

E2ODriver::E2ODriver(ros::NodeHandle node, ros::NodeHandle private_nh)
{

}