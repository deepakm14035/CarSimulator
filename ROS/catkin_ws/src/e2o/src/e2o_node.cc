/*
 *  Copyright (C) 2015 Indraprastha Institute of Information Technology, Sanjit Kaul
 * 
 *  License: Modified BSD Software License Agreement
 *
 *  $Id$
 */

/** \file
 *
 *  ROS driver node for Mahindra e2o
 */

#include <ros/ros.h>
#include "e2o.h"
#include "../../can/include/can/can_driver.h"
//#include "can_driver.h"
#include <signal.h>
#include <sensor_msgs/Joy.h>
using namespace e2o_driver;


/*void mySigintHandler(int sig) {
  RadarDriver::print_map();
  ros::shutdown();
}*/

const char *my_itoa_buf(char *buf, size_t len, int num)
{
  static char loc_buf[sizeof(int) * 8]; /* not thread safe */

  if (!buf)
  {
    buf = loc_buf;
    len = sizeof(loc_buf);
  }

  if (snprintf(buf, len, "%d", num) == -1)
    return ""; /* or whatever */

  return buf;
}



float acceleration = 0.0;
float brakes = 0.0;
float steering = 0.0;
float steering_direc = 0;

// void clear()
// {
//   for(int i=0; i<8; i++)
//     {
//       frame.data[i]= (unsigned char)0x00;
//     }
// }

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
    
  brakes = input->axes[2];
  std::cout<<"original brakes : "<<brakes<<" \n";
  
  //mapping (-1,1) to (0,1)
  brakes = ((brakes + 1.0) / 2.0)*100;
  std::cout<<"converted brakes : "<<brakes<<" \n";

  acceleration = input->axes[1];
  std::cout<<"original acceleration : "<<acceleration<<" \n";
  //mapping (-1,1) to (0,1)
  acceleration = ((acceleration + 1.0) / 2.0)*100;
  std::cout<<"converted acceleration : "<<acceleration<<" \n";

  std::cout<<"-----------------------------------------------\n";
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "e2o_node", ros::init_options::NoSigintHandler);
  ros::NodeHandle node;
  ros::NodeHandle private_nh("~");

  //subscribe to joystick
  ros::Subscriber sub = private_nh.subscribe<sensor_msgs::Joy> ("/joy", 1, joy_callback);

  // start the driver
  e2o_driver::E2ODriver dvr(node, private_nh);
  //struct canfd_frame frame;
  struct can_frame frame;
  memset(frame.data,0,8);
  frame.can_dlc = 8;

/*
  std::cout << "Frame id is " << frame.can_id << std::endl;
  frame.can_id = 0;
  frame.can_id = 0x07 | (0x6D << 8);
  std::cout << "Frame id is " << frame.can_id << std::endl;*/

  //frame.len = 8;
  //frame.flags = 0;
  //frame.data[8] = '\0';

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
    frame.data[7] = (unsigned char)acceleration;
    frame.data[7]=frame.data[7]<<1;
    frame.data[7] ^= (-1 ^ frame.data[7]) & (1 << 0);
    can_driver::CANObject::sendMsgOverCAN(&frame);
    skk_sprint_canframe(buf , (struct canfd_frame*) &frame, 0, CAN_MAX_DLEN);
    std::cout<<"throttle : "<< buf << "     "<<"throttle  value : "<<acceleration<<std::endl;;
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
    std::cout<<"breaks   : "<< buf << "     "<<"breaks  value : "<<brakes<<std::endl;;
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
    std::cout<<"steering : " << buf << std::endl;
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
  //signal(SIGINT, mySigintHandler);



    // for(int i=0; i<8; i++)
    // {
    //   // std::bitset<8> x(frame.data[i]);
    //   std::cout<<frame.data[i]<<std::endl;
    // }