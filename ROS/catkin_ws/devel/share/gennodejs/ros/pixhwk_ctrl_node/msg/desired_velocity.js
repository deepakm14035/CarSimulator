// Auto-generated. Do not edit!

// (in-package pixhwk_ctrl_node.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class desired_velocity {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.velocity = null;
      this.priority = null;
      this.node_id = null;
    }
    else {
      if (initObj.hasOwnProperty('velocity')) {
        this.velocity = initObj.velocity
      }
      else {
        this.velocity = 0.0;
      }
      if (initObj.hasOwnProperty('priority')) {
        this.priority = initObj.priority
      }
      else {
        this.priority = 0;
      }
      if (initObj.hasOwnProperty('node_id')) {
        this.node_id = initObj.node_id
      }
      else {
        this.node_id = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type desired_velocity
    // Serialize message field [velocity]
    bufferOffset = _serializer.float64(obj.velocity, buffer, bufferOffset);
    // Serialize message field [priority]
    bufferOffset = _serializer.int8(obj.priority, buffer, bufferOffset);
    // Serialize message field [node_id]
    bufferOffset = _serializer.int8(obj.node_id, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type desired_velocity
    let len;
    let data = new desired_velocity(null);
    // Deserialize message field [velocity]
    data.velocity = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [priority]
    data.priority = _deserializer.int8(buffer, bufferOffset);
    // Deserialize message field [node_id]
    data.node_id = _deserializer.int8(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 10;
  }

  static datatype() {
    // Returns string type for a message object
    return 'pixhwk_ctrl_node/desired_velocity';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'e42d3ef493b1319a15d2180a4aa66278';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    float64 velocity
    				# 100 = edge_case(must be handled first);
    				# 99 - 51 = less than edge, more than traffic;	
    				# 49 - 2 = less than traffic, more than traj.
    int8 priority
    int8 node_id
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new desired_velocity(null);
    if (msg.velocity !== undefined) {
      resolved.velocity = msg.velocity;
    }
    else {
      resolved.velocity = 0.0
    }

    if (msg.priority !== undefined) {
      resolved.priority = msg.priority;
    }
    else {
      resolved.priority = 0
    }

    if (msg.node_id !== undefined) {
      resolved.node_id = msg.node_id;
    }
    else {
      resolved.node_id = 0
    }

    return resolved;
    }
};

module.exports = desired_velocity;
