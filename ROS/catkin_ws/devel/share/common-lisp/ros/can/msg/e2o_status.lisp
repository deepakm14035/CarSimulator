; Auto-generated. Do not edit!


(cl:in-package can-msg)


;//! \htmlinclude e2o_status.msg.html

(cl:defclass <e2o_status> (roslisp-msg-protocol:ros-message)
  ((velocity
    :reader velocity
    :initarg :velocity
    :type cl:integer
    :initform 0)
   (throttle
    :reader throttle
    :initarg :throttle
    :type cl:integer
    :initform 0)
   (brake
    :reader brake
    :initarg :brake
    :type cl:integer
    :initform 0)
   (steering
    :reader steering
    :initarg :steering
    :type cl:integer
    :initform 0)
   (steering_direc
    :reader steering_direc
    :initarg :steering_direc
    :type cl:integer
    :initform 0)
   (header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header)))
)

(cl:defclass e2o_status (<e2o_status>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <e2o_status>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'e2o_status)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name can-msg:<e2o_status> is deprecated: use can-msg:e2o_status instead.")))

(cl:ensure-generic-function 'velocity-val :lambda-list '(m))
(cl:defmethod velocity-val ((m <e2o_status>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:velocity-val is deprecated.  Use can-msg:velocity instead.")
  (velocity m))

(cl:ensure-generic-function 'throttle-val :lambda-list '(m))
(cl:defmethod throttle-val ((m <e2o_status>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:throttle-val is deprecated.  Use can-msg:throttle instead.")
  (throttle m))

(cl:ensure-generic-function 'brake-val :lambda-list '(m))
(cl:defmethod brake-val ((m <e2o_status>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:brake-val is deprecated.  Use can-msg:brake instead.")
  (brake m))

(cl:ensure-generic-function 'steering-val :lambda-list '(m))
(cl:defmethod steering-val ((m <e2o_status>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:steering-val is deprecated.  Use can-msg:steering instead.")
  (steering m))

(cl:ensure-generic-function 'steering_direc-val :lambda-list '(m))
(cl:defmethod steering_direc-val ((m <e2o_status>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:steering_direc-val is deprecated.  Use can-msg:steering_direc instead.")
  (steering_direc m))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <e2o_status>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can-msg:header-val is deprecated.  Use can-msg:header instead.")
  (header m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <e2o_status>) ostream)
  "Serializes a message object of type '<e2o_status>"
  (cl:let* ((signed (cl:slot-value msg 'velocity)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'throttle)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'brake)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'steering)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'steering_direc)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <e2o_status>) istream)
  "Deserializes a message object of type '<e2o_status>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'velocity) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'throttle) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'brake) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'steering) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'steering_direc) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<e2o_status>)))
  "Returns string type for a message object of type '<e2o_status>"
  "can/e2o_status")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'e2o_status)))
  "Returns string type for a message object of type 'e2o_status"
  "can/e2o_status")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<e2o_status>)))
  "Returns md5sum for a message object of type '<e2o_status>"
  "94b6373a082ac03fa17612f6941ccfa8")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'e2o_status)))
  "Returns md5sum for a message object of type 'e2o_status"
  "94b6373a082ac03fa17612f6941ccfa8")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<e2o_status>)))
  "Returns full string definition for message of type '<e2o_status>"
  (cl:format cl:nil "int32 velocity~%int32 throttle~%int32 brake~%int32 steering~%int32 steering_direc~%std_msgs/Header header~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'e2o_status)))
  "Returns full string definition for message of type 'e2o_status"
  (cl:format cl:nil "int32 velocity~%int32 throttle~%int32 brake~%int32 steering~%int32 steering_direc~%std_msgs/Header header~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <e2o_status>))
  (cl:+ 0
     4
     4
     4
     4
     4
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <e2o_status>))
  "Converts a ROS message object to a list"
  (cl:list 'e2o_status
    (cl:cons ':velocity (velocity msg))
    (cl:cons ':throttle (throttle msg))
    (cl:cons ':brake (brake msg))
    (cl:cons ':steering (steering msg))
    (cl:cons ':steering_direc (steering_direc msg))
    (cl:cons ':header (header msg))
))
