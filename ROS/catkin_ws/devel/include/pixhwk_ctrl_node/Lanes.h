// Generated by gencpp from file pixhwk_ctrl_node/Lanes.msg
// DO NOT EDIT!


#ifndef PIXHWK_CTRL_NODE_MESSAGE_LANES_H
#define PIXHWK_CTRL_NODE_MESSAGE_LANES_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <std_msgs/Header.h>
#include <geometry_msgs/Point32.h>
#include <geometry_msgs/Point32.h>

namespace pixhwk_ctrl_node
{
template <class ContainerAllocator>
struct Lanes_
{
  typedef Lanes_<ContainerAllocator> Type;

  Lanes_()
    : header()
    , lane_left_points()
    , lane_right_points()  {
    }
  Lanes_(const ContainerAllocator& _alloc)
    : header(_alloc)
    , lane_left_points(_alloc)
    , lane_right_points(_alloc)  {
  (void)_alloc;
    }



   typedef  ::std_msgs::Header_<ContainerAllocator>  _header_type;
  _header_type header;

   typedef std::vector< ::geometry_msgs::Point32_<ContainerAllocator> , typename ContainerAllocator::template rebind< ::geometry_msgs::Point32_<ContainerAllocator> >::other >  _lane_left_points_type;
  _lane_left_points_type lane_left_points;

   typedef std::vector< ::geometry_msgs::Point32_<ContainerAllocator> , typename ContainerAllocator::template rebind< ::geometry_msgs::Point32_<ContainerAllocator> >::other >  _lane_right_points_type;
  _lane_right_points_type lane_right_points;





  typedef boost::shared_ptr< ::pixhwk_ctrl_node::Lanes_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::pixhwk_ctrl_node::Lanes_<ContainerAllocator> const> ConstPtr;

}; // struct Lanes_

typedef ::pixhwk_ctrl_node::Lanes_<std::allocator<void> > Lanes;

typedef boost::shared_ptr< ::pixhwk_ctrl_node::Lanes > LanesPtr;
typedef boost::shared_ptr< ::pixhwk_ctrl_node::Lanes const> LanesConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::pixhwk_ctrl_node::Lanes_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::pixhwk_ctrl_node::Lanes_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace pixhwk_ctrl_node

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': True}
// {'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg'], 'pixhwk_ctrl_node': ['/home/deepak/catkin_ws/src/PixHawkController/msg'], 'geometry_msgs': ['/opt/ros/kinetic/share/geometry_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::pixhwk_ctrl_node::Lanes_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::pixhwk_ctrl_node::Lanes_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::pixhwk_ctrl_node::Lanes_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::pixhwk_ctrl_node::Lanes_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::pixhwk_ctrl_node::Lanes_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::pixhwk_ctrl_node::Lanes_<ContainerAllocator> const>
  : TrueType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::pixhwk_ctrl_node::Lanes_<ContainerAllocator> >
{
  static const char* value()
  {
    return "1c83d511d158b95dd22c4a5f998f865d";
  }

  static const char* value(const ::pixhwk_ctrl_node::Lanes_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x1c83d511d158b95dULL;
  static const uint64_t static_value2 = 0xd22c4a5f998f865dULL;
};

template<class ContainerAllocator>
struct DataType< ::pixhwk_ctrl_node::Lanes_<ContainerAllocator> >
{
  static const char* value()
  {
    return "pixhwk_ctrl_node/Lanes";
  }

  static const char* value(const ::pixhwk_ctrl_node::Lanes_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::pixhwk_ctrl_node::Lanes_<ContainerAllocator> >
{
  static const char* value()
  {
    return "std_msgs/Header header\n\
geometry_msgs/Point32[] lane_left_points\n\
geometry_msgs/Point32[] lane_right_points\n\
\n\
================================================================================\n\
MSG: std_msgs/Header\n\
# Standard metadata for higher-level stamped data types.\n\
# This is generally used to communicate timestamped data \n\
# in a particular coordinate frame.\n\
# \n\
# sequence ID: consecutively increasing ID \n\
uint32 seq\n\
#Two-integer timestamp that is expressed as:\n\
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')\n\
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')\n\
# time-handling sugar is provided by the client library\n\
time stamp\n\
#Frame this data is associated with\n\
# 0: no frame\n\
# 1: global frame\n\
string frame_id\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Point32\n\
# This contains the position of a point in free space(with 32 bits of precision).\n\
# It is recommeded to use Point wherever possible instead of Point32.  \n\
# \n\
# This recommendation is to promote interoperability.  \n\
#\n\
# This message is designed to take up less space when sending\n\
# lots of points at once, as in the case of a PointCloud.  \n\
\n\
float32 x\n\
float32 y\n\
float32 z\n\
";
  }

  static const char* value(const ::pixhwk_ctrl_node::Lanes_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::pixhwk_ctrl_node::Lanes_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.header);
      stream.next(m.lane_left_points);
      stream.next(m.lane_right_points);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct Lanes_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::pixhwk_ctrl_node::Lanes_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::pixhwk_ctrl_node::Lanes_<ContainerAllocator>& v)
  {
    s << indent << "header: ";
    s << std::endl;
    Printer< ::std_msgs::Header_<ContainerAllocator> >::stream(s, indent + "  ", v.header);
    s << indent << "lane_left_points[]" << std::endl;
    for (size_t i = 0; i < v.lane_left_points.size(); ++i)
    {
      s << indent << "  lane_left_points[" << i << "]: ";
      s << std::endl;
      s << indent;
      Printer< ::geometry_msgs::Point32_<ContainerAllocator> >::stream(s, indent + "    ", v.lane_left_points[i]);
    }
    s << indent << "lane_right_points[]" << std::endl;
    for (size_t i = 0; i < v.lane_right_points.size(); ++i)
    {
      s << indent << "  lane_right_points[" << i << "]: ";
      s << std::endl;
      s << indent;
      Printer< ::geometry_msgs::Point32_<ContainerAllocator> >::stream(s, indent + "    ", v.lane_right_points[i]);
    }
  }
};

} // namespace message_operations
} // namespace ros

#endif // PIXHWK_CTRL_NODE_MESSAGE_LANES_H