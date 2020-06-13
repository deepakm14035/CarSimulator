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

class OmplStatus {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.OmplSolStatus = null;
      this.actualmsg = null;
      this.stamp = null;
    }
    else {
      if (initObj.hasOwnProperty('OmplSolStatus')) {
        this.OmplSolStatus = initObj.OmplSolStatus
      }
      else {
        this.OmplSolStatus = 0;
      }
      if (initObj.hasOwnProperty('actualmsg')) {
        this.actualmsg = initObj.actualmsg
      }
      else {
        this.actualmsg = '';
      }
      if (initObj.hasOwnProperty('stamp')) {
        this.stamp = initObj.stamp
      }
      else {
        this.stamp = {secs: 0, nsecs: 0};
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type OmplStatus
    // Serialize message field [OmplSolStatus]
    bufferOffset = _serializer.int8(obj.OmplSolStatus, buffer, bufferOffset);
    // Serialize message field [actualmsg]
    bufferOffset = _serializer.string(obj.actualmsg, buffer, bufferOffset);
    // Serialize message field [stamp]
    bufferOffset = _serializer.time(obj.stamp, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type OmplStatus
    let len;
    let data = new OmplStatus(null);
    // Deserialize message field [OmplSolStatus]
    data.OmplSolStatus = _deserializer.int8(buffer, bufferOffset);
    // Deserialize message field [actualmsg]
    data.actualmsg = _deserializer.string(buffer, bufferOffset);
    // Deserialize message field [stamp]
    data.stamp = _deserializer.time(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += object.actualmsg.length;
    return length + 13;
  }

  static datatype() {
    // Returns string type for a message object
    return 'pixhwk_ctrl_node/OmplStatus';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '5e50e9584b597fecb0c7c001901c2ff9';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    int8 OmplSolStatus
    string actualmsg
    time stamp
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new OmplStatus(null);
    if (msg.OmplSolStatus !== undefined) {
      resolved.OmplSolStatus = msg.OmplSolStatus;
    }
    else {
      resolved.OmplSolStatus = 0
    }

    if (msg.actualmsg !== undefined) {
      resolved.actualmsg = msg.actualmsg;
    }
    else {
      resolved.actualmsg = ''
    }

    if (msg.stamp !== undefined) {
      resolved.stamp = msg.stamp;
    }
    else {
      resolved.stamp = {secs: 0, nsecs: 0}
    }

    return resolved;
    }
};

module.exports = OmplStatus;
