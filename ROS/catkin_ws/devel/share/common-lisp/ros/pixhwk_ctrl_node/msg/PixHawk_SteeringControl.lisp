; Auto-generated. Do not edit!


(cl:in-package pixhwk_ctrl_node-msg)


;//! \htmlinclude PixHawk_SteeringControl.msg.html

(cl:defclass <PixHawk_SteeringControl> (roslisp-msg-protocol:ros-message)
  ((steering
    :reader steering
    :initarg :steering
    :type cl:float
    :initform 0.0))
)

(cl:defclass PixHawk_SteeringControl (<PixHawk_SteeringControl>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <PixHawk_SteeringControl>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'PixHawk_SteeringControl)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name pixhwk_ctrl_node-msg:<PixHawk_SteeringControl> is deprecated: use pixhwk_ctrl_node-msg:PixHawk_SteeringControl instead.")))

(cl:ensure-generic-function 'steering-val :lambda-list '(m))
(cl:defmethod steering-val ((m <PixHawk_SteeringControl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader pixhwk_ctrl_node-msg:steering-val is deprecated.  Use pixhwk_ctrl_node-msg:steering instead.")
  (steering m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <PixHawk_SteeringControl>) ostream)
  "Serializes a message object of type '<PixHawk_SteeringControl>"
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'steering))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <PixHawk_SteeringControl>) istream)
  "Deserializes a message object of type '<PixHawk_SteeringControl>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'steering) (roslisp-utils:decode-double-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<PixHawk_SteeringControl>)))
  "Returns string type for a message object of type '<PixHawk_SteeringControl>"
  "pixhwk_ctrl_node/PixHawk_SteeringControl")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'PixHawk_SteeringControl)))
  "Returns string type for a message object of type 'PixHawk_SteeringControl"
  "pixhwk_ctrl_node/PixHawk_SteeringControl")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<PixHawk_SteeringControl>)))
  "Returns md5sum for a message object of type '<PixHawk_SteeringControl>"
  "13be5889908f58ce029441890e49203c")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'PixHawk_SteeringControl)))
  "Returns md5sum for a message object of type 'PixHawk_SteeringControl"
  "13be5889908f58ce029441890e49203c")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<PixHawk_SteeringControl>)))
  "Returns full string definition for message of type '<PixHawk_SteeringControl>"
  (cl:format cl:nil "float64 steering~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'PixHawk_SteeringControl)))
  "Returns full string definition for message of type 'PixHawk_SteeringControl"
  (cl:format cl:nil "float64 steering~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <PixHawk_SteeringControl>))
  (cl:+ 0
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <PixHawk_SteeringControl>))
  "Converts a ROS message object to a list"
  (cl:list 'PixHawk_SteeringControl
    (cl:cons ':steering (steering msg))
))
