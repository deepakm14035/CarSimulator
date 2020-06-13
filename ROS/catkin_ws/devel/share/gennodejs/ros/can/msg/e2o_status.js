// Auto-generated. Do not edit!

// (in-package can.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let std_msgs = _finder('std_msgs');

//-----------------------------------------------------------

class e2o_status {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.velocity = null;
      this.throttle = null;
      this.brake = null;
      this.steering = null;
      this.steering_direc = null;
      this.header = null;
    }
    else {
      if (initObj.hasOwnProperty('velocity')) {
        this.velocity = initObj.velocity
      }
      else {
        this.velocity = 0;
      }
      if (initObj.hasOwnProperty('throttle')) {
        this.throttle = initObj.throttle
      }
      else {
        this.throttle = 0;
      }
      if (initObj.hasOwnProperty('brake')) {
        this.brake = initObj.brake
      }
      else {
        this.brake = 0;
      }
      if (initObj.hasOwnProperty('steering')) {
        this.steering = initObj.steering
      }
      else {
        this.steering = 0;
      }
      if (initObj.hasOwnProperty('steering_direc')) {
        this.steering_direc = initObj.steering_direc
      }
      else {
        this.steering_direc = 0;
      }
      if (initObj.hasOwnProperty('header')) {
        this.header = initObj.header
      }
      else {
        this.header = new std_msgs.msg.Header();
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type e2o_status
    // Serialize message field [velocity]
    bufferOffset = _serializer.int32(obj.velocity, buffer, bufferOffset);
    // Serialize message field [throttle]
    bufferOffset = _serializer.int32(obj.throttle, buffer, bufferOffset);
    // Serialize message field [brake]
    bufferOffset = _serializer.int32(obj.brake, buffer, bufferOffset);
    // Serialize message field [steering]
    bufferOffset = _serializer.int32(obj.steering, buffer, bufferOffset);
    // Serialize message field [steering_direc]
    bufferOffset = _serializer.int32(obj.steering_direc, buffer, bufferOffset);
    // Serialize message field [header]
    bufferOffset = std_msgs.msg.Header.serialize(obj.header, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type e2o_status
    let len;
    let data = new e2o_status(null);
    // Deserialize message field [velocity]
    data.velocity = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [throttle]
    data.throttle = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [brake]
    data.brake = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [steering]
    data.steering = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [steering_direc]
    data.steering_direc = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [header]
    data.header = std_msgs.msg.Header.deserialize(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += std_msgs.msg.Header.getMessageSize(object.header);
    return length + 20;
  }

  static datatype() {
    // Returns string type for a message object
    return 'can/e2o_status';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '94b6373a082ac03fa17612f6941ccfa8';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    int32 velocity
    int32 throttle
    int32 brake
    int32 steering
    int32 steering_direc
    std_msgs/Header header
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
    const resolved = new e2o_status(null);
    if (msg.velocity !== undefined) {
      resolved.velocity = msg.velocity;
    }
    else {
      resolved.velocity = 0
    }

    if (msg.throttle !== undefined) {
      resolved.throttle = msg.throttle;
    }
    else {
      resolved.throttle = 0
    }

    if (msg.brake !== undefined) {
      resolved.brake = msg.brake;
    }
    else {
      resolved.brake = 0
    }

    if (msg.steering !== undefined) {
      resolved.steering = msg.steering;
    }
    else {
      resolved.steering = 0
    }

    if (msg.steering_direc !== undefined) {
      resolved.steering_direc = msg.steering_direc;
    }
    else {
      resolved.steering_direc = 0
    }

    if (msg.header !== undefined) {
      resolved.header = std_msgs.msg.Header.Resolve(msg.header)
    }
    else {
      resolved.header = new std_msgs.msg.Header()
    }

    return resolved;
    }
};

module.exports = e2o_status;
