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

class PixHawk_Goal {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.Lat = null;
      this.Lon = null;
    }
    else {
      if (initObj.hasOwnProperty('Lat')) {
        this.Lat = initObj.Lat
      }
      else {
        this.Lat = 0.0;
      }
      if (initObj.hasOwnProperty('Lon')) {
        this.Lon = initObj.Lon
      }
      else {
        this.Lon = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type PixHawk_Goal
    // Serialize message field [Lat]
    bufferOffset = _serializer.float64(obj.Lat, buffer, bufferOffset);
    // Serialize message field [Lon]
    bufferOffset = _serializer.float64(obj.Lon, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type PixHawk_Goal
    let len;
    let data = new PixHawk_Goal(null);
    // Deserialize message field [Lat]
    data.Lat = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [Lon]
    data.Lon = _deserializer.float64(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 16;
  }

  static datatype() {
    // Returns string type for a message object
    return 'pixhwk_ctrl_node/PixHawk_Goal';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '6481a43b47ce4303d5bdcb88c9414b02';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    float64 Lat
    float64 Lon
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new PixHawk_Goal(null);
    if (msg.Lat !== undefined) {
      resolved.Lat = msg.Lat;
    }
    else {
      resolved.Lat = 0.0
    }

    if (msg.Lon !== undefined) {
      resolved.Lon = msg.Lon;
    }
    else {
      resolved.Lon = 0.0
    }

    return resolved;
    }
};

module.exports = PixHawk_Goal;
