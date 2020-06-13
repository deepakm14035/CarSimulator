; Auto-generated. Do not edit!


(cl:in-package pixhwk_ctrl_node-msg)


;//! \htmlinclude traffic_light.msg.html

(cl:defclass <traffic_light> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (dist_light
    :reader dist_light
    :initarg :dist_light
    :type cl:float
    :initform 0.0)
   (color_light
    :reader color_light
    :initarg :color_light
    :type cl:string
    :initform ""))
)

(cl:defclass traffic_light (<traffic_light>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <traffic_light>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'traffic_light)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name pixhwk_ctrl_node-msg:<traffic_light> is deprecated: use pixhwk_ctrl_node-msg:traffic_light instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <traffic_light>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader pixhwk_ctrl_node-msg:header-val is deprecated.  Use pixhwk_ctrl_node-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'dist_light-val :lambda-list '(m))
(cl:defmethod dist_light-val ((m <traffic_light>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader pixhwk_ctrl_node-msg:dist_light-val is deprecated.  Use pixhwk_ctrl_node-msg:dist_light instead.")
  (dist_light m))

(cl:ensure-generic-function 'color_light-val :lambda-list '(m))
(cl:defmethod color_light-val ((m <traffic_light>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader pixhwk_ctrl_node-msg:color_light-val is deprecated.  Use pixhwk_ctrl_node-msg:color_light instead.")
  (color_light m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <traffic_light>) ostream)
  "Serializes a message object of type '<traffic_light>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'dist_light))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'color_light))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'color_light))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <traffic_light>) istream)
  "Deserializes a message object of type '<traffic_light>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'dist_light) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'color_light) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'color_light) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<traffic_light>)))
  "Returns string type for a message object of type '<traffic_light>"
  "pixhwk_ctrl_node/traffic_light")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'traffic_light)))
  "Returns string type for a message object of type 'traffic_light"
  "pixhwk_ctrl_node/traffic_light")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<traffic_light>)))
  "Returns md5sum for a message object of type '<traffic_light>"
  "f8df0c506b0f4a93a3c764f8dfe78ca5")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'traffic_light)))
  "Returns md5sum for a message object of type 'traffic_light"
  "f8df0c506b0f4a93a3c764f8dfe78ca5")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<traffic_light>)))
  "Returns full string definition for message of type '<traffic_light>"
  (cl:format cl:nil "Header header~%float64 dist_light~%string color_light~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'traffic_light)))
  "Returns full string definition for message of type 'traffic_light"
  (cl:format cl:nil "Header header~%float64 dist_light~%string color_light~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <traffic_light>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     8
     4 (cl:length (cl:slot-value msg 'color_light))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <traffic_light>))
  "Converts a ROS message object to a list"
  (cl:list 'traffic_light
    (cl:cons ':header (header msg))
    (cl:cons ':dist_light (dist_light msg))
    (cl:cons ':color_light (color_light msg))
))
