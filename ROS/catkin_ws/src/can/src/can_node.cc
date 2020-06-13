/*
 *  Copyright (C) 2015 Indraprastha Institute of Information Technology, Sanjit Kaul
 * 
 *  License: Modified BSD Software License Agreement
 *
 *  $Id$
 */

/** \file
 *
 *  CAN Node for talking to RADAR and e2o
 */

#include <ros/ros.h>
#include "can_driver.h"
#include <signal.h>
#include <can/e2o_status.h>
#include <iostream>
#include <iomanip>

using namespace can_driver;
can::e2o_status e2o_message;

ros::Publisher e2o_broadcaster;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "can_node", ros::init_options::NoSigintHandler);
  ros::NodeHandle node;
  ros::NodeHandle private_nh("~");

  // start the driver
  can_driver::CANObject dvr(node, private_nh);
  can_driver::CANObject::initCAN();

  e2o_broadcaster = private_nh.advertise<can::e2o_status>( "e2o_info", 10);

  ros::Rate loop_rate(10);

  while(ros::ok() && dvr.poll()) {
    ros::spinOnce();
  }
  return 0;
}