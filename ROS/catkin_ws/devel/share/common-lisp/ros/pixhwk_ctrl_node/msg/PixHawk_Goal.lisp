; Auto-generated. Do not edit!


(cl:in-package pixhwk_ctrl_node-msg)


;//! \htmlinclude PixHawk_Goal.msg.html

(cl:defclass <PixHawk_Goal> (roslisp-msg-protocol:ros-message)
  ((Lat
    :reader Lat
    :initarg :Lat
    :type cl:float
    :initform 0.0)
   (Lon
    :reader Lon
    :initarg :Lon
    :type cl:float
    :initform 0.0))
)

(cl:defclass PixHawk_Goal (<PixHawk_Goal>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <PixHawk_Goal>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'PixHawk_Goal)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name pixhwk_ctrl_node-msg:<PixHawk_Goal> is deprecated: use pixhwk_ctrl_node-msg:PixHawk_Goal instead.")))

(cl:ensure-generic-function 'Lat-val :lambda-list '(m))
(cl:defmethod Lat-val ((m <PixHawk_Goal>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader pixhwk_ctrl_node-msg:Lat-val is deprecated.  Use pixhwk_ctrl_node-msg:Lat instead.")
  (Lat m))

(cl:ensure-generic-function 'Lon-val :lambda-list '(m))
(cl:defmethod Lon-val ((m <PixHawk_Goal>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader pixhwk_ctrl_node-msg:Lon-val is deprecated.  Use pixhwk_ctrl_node-msg:Lon instead.")
  (Lon m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <PixHawk_Goal>) ostream)
  "Serializes a message object of type '<PixHawk_Goal>"
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'Lat))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'Lon))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <PixHawk_Goal>) istream)
  "Deserializes a message object of type '<PixHawk_Goal>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'Lat) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'Lon) (roslisp-utils:decode-double-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<PixHawk_Goal>)))
  "Returns string type for a message object of type '<PixHawk_Goal>"
  "pixhwk_ctrl_node/PixHawk_Goal")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'PixHawk_Goal)))
  "Returns string type for a message object of type 'PixHawk_Goal"
  "pixhwk_ctrl_node/PixHawk_Goal")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<PixHawk_Goal>)))
  "Returns md5sum for a message object of type '<PixHawk_Goal>"
  "6481a43b47ce4303d5bdcb88c9414b02")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'PixHawk_Goal)))
  "Returns md5sum for a message object of type 'PixHawk_Goal"
  "6481a43b47ce4303d5bdcb88c9414b02")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<PixHawk_Goal>)))
  "Returns full string definition for message of type '<PixHawk_Goal>"
  (cl:format cl:nil "float64 Lat~%float64 Lon~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'PixHawk_Goal)))
  "Returns full string definition for message of type 'PixHawk_Goal"
  (cl:format cl:nil "float64 Lat~%float64 Lon~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <PixHawk_Goal>))
  (cl:+ 0
     8
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <PixHawk_Goal>))
  "Converts a ROS message object to a list"
  (cl:list 'PixHawk_Goal
    (cl:cons ':Lat (Lat msg))
    (cl:cons ':Lon (Lon msg))
))
