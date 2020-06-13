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
 *  e2o messaging
 */

#ifndef _E2O_DRIVER_H_
#define _E2O_DRIVER_H_ 1

#include <string>
#include <ros/ros.h>
#include <diagnostic_updater/diagnostic_updater.h>
#include <diagnostic_updater/publisher.h>

namespace e2o_driver
{

class E2ODriver
{
public:

  E2ODriver(ros::NodeHandle node,
                 ros::NodeHandle private_nh);
  ~E2ODriver() {}
private:

};

} // namespace radar_driver

#endif // _E2O_DRIVER_H_
