; Auto-generated. Do not edit!


(cl:in-package pixhwk_ctrl_node-msg)


;//! \htmlinclude desired_velocity.msg.html

(cl:defclass <desired_velocity> (roslisp-msg-protocol:ros-message)
  ((velocity
    :reader velocity
    :initarg :velocity
    :type cl:float
    :initform 0.0)
   (priority
    :reader priority
    :initarg :priority
    :type cl:fixnum
    :initform 0)
   (node_id
    :reader node_id
    :initarg :node_id
    :type cl:fixnum
    :initform 0))
)

(cl:defclass desired_velocity (<desired_velocity>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <desired_velocity>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'desired_velocity)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name pixhwk_ctrl_node-msg:<desired_velocity> is deprecated: use pixhwk_ctrl_node-msg:desired_velocity instead.")))

(cl:ensure-generic-function 'velocity-val :lambda-list '(m))
(cl:defmethod velocity-val ((m <desired_velocity>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader pixhwk_ctrl_node-msg:velocity-val is deprecated.  Use pixhwk_ctrl_node-msg:velocity instead.")
  (velocity m))

(cl:ensure-generic-function 'priority-val :lambda-list '(m))
(cl:defmethod priority-val ((m <desired_velocity>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader pixhwk_ctrl_node-msg:priority-val is deprecated.  Use pixhwk_ctrl_node-msg:priority instead.")
  (priority m))

(cl:ensure-generic-function 'node_id-val :lambda-list '(m))
(cl:defmethod node_id-val ((m <desired_velocity>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader pixhwk_ctrl_node-msg:node_id-val is deprecated.  Use pixhwk_ctrl_node-msg:node_id instead.")
  (node_id m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <desired_velocity>) ostream)
  "Serializes a message object of type '<desired_velocity>"
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'velocity))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let* ((signed (cl:slot-value msg 'priority)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 256) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'node_id)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 256) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <desired_velocity>) istream)
  "Deserializes a message object of type '<desired_velocity>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'velocity) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'priority) (cl:if (cl:< unsigned 128) unsigned (cl:- unsigned 256))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'node_id) (cl:if (cl:< unsigned 128) unsigned (cl:- unsigned 256))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<desired_velocity>)))
  "Returns string type for a message object of type '<desired_velocity>"
  "pixhwk_ctrl_node/desired_velocity")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'desired_velocity)))
  "Returns string type for a message object of type 'desired_velocity"
  "pixhwk_ctrl_node/desired_velocity")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<desired_velocity>)))
  "Returns md5sum for a message object of type '<desired_velocity>"
  "e42d3ef493b1319a15d2180a4aa66278")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'desired_velocity)))
  "Returns md5sum for a message object of type 'desired_velocity"
  "e42d3ef493b1319a15d2180a4aa66278")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<desired_velocity>)))
  "Returns full string definition for message of type '<desired_velocity>"
  (cl:format cl:nil "float64 velocity~%				# 100 = edge_case(must be handled first);~%				# 99 - 51 = less than edge, more than traffic;	~%				# 49 - 2 = less than traffic, more than traj.~%int8 priority~%int8 node_id~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'desired_velocity)))
  "Returns full string definition for message of type 'desired_velocity"
  (cl:format cl:nil "float64 velocity~%				# 100 = edge_case(must be handled first);~%				# 99 - 51 = less than edge, more than traffic;	~%				# 49 - 2 = less than traffic, more than traj.~%int8 priority~%int8 node_id~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <desired_velocity>))
  (cl:+ 0
     8
     1
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <desired_velocity>))
  "Converts a ROS message object to a list"
  (cl:list 'desired_velocity
    (cl:cons ':velocity (velocity msg))
    (cl:cons ':priority (priority msg))
    (cl:cons ':node_id (node_id msg))
))
