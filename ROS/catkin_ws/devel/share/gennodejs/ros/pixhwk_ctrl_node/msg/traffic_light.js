// Auto-generated. Do not edit!

// (in-package pixhwk_ctrl_node.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let std_msgs = _finder('std_msgs');

//-----------------------------------------------------------

class traffic_light {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.header = null;
      this.dist_light = null;
      this.color_light = null;
    }
    else {
      if (initObj.hasOwnProperty('header')) {
        this.header = initObj.header
      }
      else {
        this.header = new std_msgs.msg.Header();
      }
      if (initObj.hasOwnProperty('dist_light')) {
        this.dist_light = initObj.dist_light
      }
      else {
        this.dist_light = 0.0;
      }
      if (initObj.hasOwnProperty('color_light')) {
        this.color_light = initObj.color_light
      }
      else {
        this.color_light = '';
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type traffic_light
    // Serialize message field [header]
    bufferOffset = std_msgs.msg.Header.serialize(obj.header, buffer, bufferOffset);
    // Serialize message field [dist_light]
    bufferOffset = _serializer.float64(obj.dist_light, buffer, bufferOffset);
    // Serialize message field [color_light]
    bufferOffset = _serializer.string(obj.color_light, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type traffic_light
    let len;
    let data = new traffic_light(null);
    // Deserialize message field [header]
    data.header = std_msgs.msg.Header.deserialize(buffer, bufferOffset);
    // Deserialize message field [dist_light]
    data.dist_light = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [color_light]
    data.color_light = _deserializer.string(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += std_msgs.msg.Header.getMessageSize(object.header);
    length += object.color_light.length;
    return length + 12;
  }

  static datatype() {
    // Returns string type for a message object
    return 'pixhwk_ctrl_node/traffic_light';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'f8df0c506b0f4a93a3c764f8dfe78ca5';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    Header header
    float64 dist_light
    string color_light
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
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new traffic_light(null);
    if (msg.header !== undefined) {
      resolved.header = std_msgs.msg.Header.Resolve(msg.header)
    }
    else {
      resolved.header = new std_msgs.msg.Header()
    }

    if (msg.dist_light !== undefined) {
      resolved.dist_light = msg.dist_light;
    }
    else {
      resolved.dist_light = 0.0
    }

    if (msg.color_light !== undefined) {
      resolved.color_light = msg.color_light;
    }
    else {
      resolved.color_light = ''
    }

    return resolved;
    }
};

module.exports = traffic_light;
