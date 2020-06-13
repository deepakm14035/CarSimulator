// Generated by gencpp from file pixhwk_ctrl_node/PixHawk_ThrottleControl.msg
// DO NOT EDIT!


#ifndef PIXHWK_CTRL_NODE_MESSAGE_PIXHAWK_THROTTLECONTROL_H
#define PIXHWK_CTRL_NODE_MESSAGE_PIXHAWK_THROTTLECONTROL_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace pixhwk_ctrl_node
{
template <class ContainerAllocator>
struct PixHawk_ThrottleControl_
{
  typedef PixHawk_ThrottleControl_<ContainerAllocator> Type;

  PixHawk_ThrottleControl_()
    : throttle(0.0)  {
    }
  PixHawk_ThrottleControl_(const ContainerAllocator& _alloc)
    : throttle(0.0)  {
  (void)_alloc;
    }



   typedef double _throttle_type;
  _throttle_type throttle;





  typedef boost::shared_ptr< ::pixhwk_ctrl_node::PixHawk_ThrottleControl_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::pixhwk_ctrl_node::PixHawk_ThrottleControl_<ContainerAllocator> const> ConstPtr;

}; // struct PixHawk_ThrottleControl_

typedef ::pixhwk_ctrl_node::PixHawk_ThrottleControl_<std::allocator<void> > PixHawk_ThrottleControl;

typedef boost::shared_ptr< ::pixhwk_ctrl_node::PixHawk_ThrottleControl > PixHawk_ThrottleControlPtr;
typedef boost::shared_ptr< ::pixhwk_ctrl_node::PixHawk_ThrottleControl const> PixHawk_ThrottleControlConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::pixhwk_ctrl_node::PixHawk_ThrottleControl_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::pixhwk_ctrl_node::PixHawk_ThrottleControl_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace pixhwk_ctrl_node

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': True, 'IsMessage': True, 'HasHeader': False}
// {'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg'], 'pixhwk_ctrl_node': ['/home/deepak/catkin_ws/src/PixHawkController/msg'], 'geometry_msgs': ['/opt/ros/kinetic/share/geometry_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::pixhwk_ctrl_node::PixHawk_ThrottleControl_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::pixhwk_ctrl_node::PixHawk_ThrottleControl_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::pixhwk_ctrl_node::PixHawk_ThrottleControl_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::pixhwk_ctrl_node::PixHawk_ThrottleControl_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::pixhwk_ctrl_node::PixHawk_ThrottleControl_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::pixhwk_ctrl_node::PixHawk_ThrottleControl_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::pixhwk_ctrl_node::PixHawk_ThrottleControl_<ContainerAllocator> >
{
  static const char* value()
  {
    return "a0e4c91f838bf9ac9a81509ea028ea0b";
  }

  static const char* value(const ::pixhwk_ctrl_node::PixHawk_ThrottleControl_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xa0e4c91f838bf9acULL;
  static const uint64_t static_value2 = 0x9a81509ea028ea0bULL;
};

template<class ContainerAllocator>
struct DataType< ::pixhwk_ctrl_node::PixHawk_ThrottleControl_<ContainerAllocator> >
{
  static const char* value()
  {
    return "pixhwk_ctrl_node/PixHawk_ThrottleControl";
  }

  static const char* value(const ::pixhwk_ctrl_node::PixHawk_ThrottleControl_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::pixhwk_ctrl_node::PixHawk_ThrottleControl_<ContainerAllocator> >
{
  static const char* value()
  {
    return "float64 throttle\n\
";
  }

  static const char* value(const ::pixhwk_ctrl_node::PixHawk_ThrottleControl_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::pixhwk_ctrl_node::PixHawk_ThrottleControl_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.throttle);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct PixHawk_ThrottleControl_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::pixhwk_ctrl_node::PixHawk_ThrottleControl_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::pixhwk_ctrl_node::PixHawk_ThrottleControl_<ContainerAllocator>& v)
  {
    s << indent << "throttle: ";
    Printer<double>::stream(s, indent + "  ", v.throttle);
  }
};

} // namespace message_operations
} // namespace ros

#endif // PIXHWK_CTRL_NODE_MESSAGE_PIXHAWK_THROTTLECONTROL_H
