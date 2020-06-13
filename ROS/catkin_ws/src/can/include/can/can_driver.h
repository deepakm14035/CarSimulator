/* -*- mode: C++ -*- */
/*
 *  Copyright (C) 2012 Austin Robot Technology, Jack O'Quin
 * 
 *  License: Modified BSD Software License Agreement
 *
 *  $Id$
 */

/** \file
 *
 *  ROS driver interface for the Velodyne 3D LIDARs
 */

#ifndef _CAN_DRIVER_H
#define _CAN_DRIVER_H 1

#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <libgen.h>
#include <time.h>
#include <errno.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <net/if.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/can/error.h>

#include "terminal.h"
#include "lib.h"

#include <string>
#include <ros/ros.h>
#include <diagnostic_updater/diagnostic_updater.h>
#include <diagnostic_updater/publisher.h>

namespace can_driver
{

class CANObject
{
public:
  static int can_socket;
  static std::string can_name;
  static struct sockaddr_can can_addr;
  CANObject(ros::NodeHandle node, ros::NodeHandle private_nh) {}
  ~CANObject() {}

  bool poll();
  static void sendMsgOverCAN(struct can_frame*);
  static void sendMsgOverCAN_FD(struct canfd_frame*);
  void processCANMessage(struct canfd_frame*);
  static void initCAN();
};

int CANObject::can_socket = -1;
std::string CANObject::can_name = "can0";
struct sockaddr_can CANObject::can_addr;


} // namespace can_driver
int idx2dindex(int , int );
void skk_sprint_canframe(char* , struct canfd_frame *, int , int );

#endif // _CAN_DRIVER_H
