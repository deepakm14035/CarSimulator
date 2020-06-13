; Auto-generated. Do not edit!


(cl:in-package pixhwk_ctrl_node-msg)


;//! \htmlinclude PixHawk_ThrottleControl.msg.html

(cl:defclass <PixHawk_ThrottleControl> (roslisp-msg-protocol:ros-message)
  ((throttle
    :reader throttle
    :initarg :throttle
    :type cl:float
    :initform 0.0))
)

(cl:defclass PixHawk_ThrottleControl (<PixHawk_ThrottleControl>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <PixHawk_ThrottleControl>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'PixHawk_ThrottleControl)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name pixhwk_ctrl_node-msg:<PixHawk_ThrottleControl> is deprecated: use pixhwk_ctrl_node-msg:PixHawk_ThrottleControl instead.")))

(cl:ensure-generic-function 'throttle-val :lambda-list '(m))
(cl:defmethod throttle-val ((m <PixHawk_ThrottleControl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader pixhwk_ctrl_node-msg:throttle-val is deprecated.  Use pixhwk_ctrl_node-msg:throttle instead.")
  (throttle m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <PixHawk_ThrottleControl>) ostream)
  "Serializes a message object of type '<PixHawk_ThrottleControl>"
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'throttle))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <PixHawk_ThrottleControl>) istream)
  "Deserializes a message object of type '<PixHawk_ThrottleControl>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'throttle) (roslisp-utils:decode-double-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<PixHawk_ThrottleControl>)))
  "Returns string type for a message object of type '<PixHawk_ThrottleControl>"
  "pixhwk_ctrl_node/PixHawk_ThrottleControl")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'PixHawk_ThrottleControl)))
  "Returns string type for a message object of type 'PixHawk_ThrottleControl"
  "pixhwk_ctrl_node/PixHawk_ThrottleControl")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<PixHawk_ThrottleControl>)))
  "Returns md5sum for a message object of type '<PixHawk_ThrottleControl>"
  "a0e4c91f838bf9ac9a81509ea028ea0b")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'PixHawk_ThrottleControl)))
  "Returns md5sum for a message object of type 'PixHawk_ThrottleControl"
  "a0e4c91f838bf9ac9a81509ea028ea0b")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<PixHawk_ThrottleControl>)))
  "Returns full string definition for message of type '<PixHawk_ThrottleControl>"
  (cl:format cl:nil "float64 throttle~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'PixHawk_ThrottleControl)))
  "Returns full string definition for message of type 'PixHawk_ThrottleControl"
  (cl:format cl:nil "float64 throttle~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <PixHawk_ThrottleControl>))
  (cl:+ 0
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <PixHawk_ThrottleControl>))
  "Converts a ROS message object to a list"
  (cl:list 'PixHawk_ThrottleControl
    (cl:cons ':throttle (throttle msg))
))
