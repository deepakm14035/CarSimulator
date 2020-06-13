// Auto-generated. Do not edit!

// (in-package pixhwk_ctrl_node.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let geometry_msgs = _finder('geometry_msgs');
let std_msgs = _finder('std_msgs');

//-----------------------------------------------------------

class Lanes {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.header = null;
      this.lane_left_points = null;
      this.lane_right_points = null;
    }
    else {
      if (initObj.hasOwnProperty('header')) {
        this.header = initObj.header
      }
      else {
        this.header = new std_msgs.msg.Header();
      }
      if (initObj.hasOwnProperty('lane_left_points')) {
        this.lane_left_points = initObj.lane_left_points
      }
      else {
        this.lane_left_points = [];
      }
      if (initObj.hasOwnProperty('lane_right_points')) {
        this.lane_right_points = initObj.lane_right_points
      }
      else {
        this.lane_right_points = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type Lanes
    // Serialize message field [header]
    bufferOffset = std_msgs.msg.Header.serialize(obj.header, buffer, bufferOffset);
    // Serialize message field [lane_left_points]
    // Serialize the length for message field [lane_left_points]
    bufferOffset = _serializer.uint32(obj.lane_left_points.length, buffer, bufferOffset);
    obj.lane_left_points.forEach((val) => {
      bufferOffset = geometry_msgs.msg.Point32.serialize(val, buffer, bufferOffset);
    });
    // Serialize message field [lane_right_points]
    // Serialize the length for message field [lane_right_points]
    bufferOffset = _serializer.uint32(obj.lane_right_points.length, buffer, bufferOffset);
    obj.lane_right_points.forEach((val) => {
      bufferOffset = geometry_msgs.msg.Point32.serialize(val, buffer, bufferOffset);
    });
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type Lanes
    let len;
    let data = new Lanes(null);
    // Deserialize message field [header]
    data.header = std_msgs.msg.Header.deserialize(buffer, bufferOffset);
    // Deserialize message field [lane_left_points]
    // Deserialize array length for message field [lane_left_points]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.lane_left_points = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.lane_left_points[i] = geometry_msgs.msg.Point32.deserialize(buffer, bufferOffset)
    }
    // Deserialize message field [lane_right_points]
    // Deserialize array length for message field [lane_right_points]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.lane_right_points = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.lane_right_points[i] = geometry_msgs.msg.Point32.deserialize(buffer, bufferOffset)
    }
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += std_msgs.msg.Header.getMessageSize(object.header);
    length += 12 * object.lane_left_points.length;
    length += 12 * object.lane_right_points.length;
    return length + 8;
  }

  static datatype() {
    // Returns string type for a message object
    return 'pixhwk_ctrl_node/Lanes';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '1c83d511d158b95dd22c4a5f998f865d';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    std_msgs/Header header
    geometry_msgs/Point32[] lane_left_points
    geometry_msgs/Point32[] lane_right_points
    
    ================================================================================
    MSG: std_msgs/Header
    # Standard metadata for higher-level stamped data types.
    # This is generally used to communicate timestamped data 
    # in a particular coordinate frame.
    # 
    # sequence ID: consecutively increasing ID 
    uint32 seq
    #Two-integer timestamp that is expressed as:
    # * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
    # * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
    # time-handling sugar is provided by the client library
    time stamp
    #Frame this data is associated with
    # 0: no frame
    # 1: global frame
    string frame_id
    
    ================================================================================
    MSG: geometry_msgs/Point32
    # This contains the position of a point in free space(with 32 bits of precision).
    # It is recommeded to use Point wherever possible instead of Point32.  
    # 
    # This recommendation is to promote interoperability.  
    #
    # This message is designed to take up less space when sending
    # lots of points at once, as in the case of a PointCloud.  
    
    float32 x
    float32 y
    float32 z
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new Lanes(null);
    if (msg.header !== undefined) {
      resolved.header = std_msgs.msg.Header.Resolve(msg.header)
    }
    else {
      resolved.header = new std_msgs.msg.Header()
    }

    if (msg.lane_left_points !== undefined) {
      resolved.lane_left_points = new Array(msg.lane_left_points.length);
      for (let i = 0; i < resolved.lane_left_points.length; ++i) {
        resolved.lane_left_points[i] = geometry_msgs.msg.Point32.Resolve(msg.lane_left_points[i]);
      }
    }
    else {
      resolved.lane_left_points = []
    }

    if (msg.lane_right_points !== undefined) {
      resolved.lane_right_points = new Array(msg.lane_right_points.length);
      for (let i = 0; i < resolved.lane_right_points.length; ++i) {
        resolved.lane_right_points[i] = geometry_msgs.msg.Point32.Resolve(msg.lane_right_points[i]);
      }
    }
    else {
      resolved.lane_right_points = []
    }

    return resolved;
    }
};

module.exports = Lanes;
