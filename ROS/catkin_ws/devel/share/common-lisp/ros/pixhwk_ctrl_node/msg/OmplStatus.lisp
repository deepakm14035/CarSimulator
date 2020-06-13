; Auto-generated. Do not edit!


(cl:in-package pixhwk_ctrl_node-msg)


;//! \htmlinclude OmplStatus.msg.html

(cl:defclass <OmplStatus> (roslisp-msg-protocol:ros-message)
  ((OmplSolStatus
    :reader OmplSolStatus
    :initarg :OmplSolStatus
    :type cl:fixnum
    :initform 0)
   (actualmsg
    :reader actualmsg
    :initarg :actualmsg
    :type cl:string
    :initform "")
   (stamp
    :reader stamp
    :initarg :stamp
    :type cl:real
    :initform 0))
)

(cl:defclass OmplStatus (<OmplStatus>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <OmplStatus>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'OmplStatus)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name pixhwk_ctrl_node-msg:<OmplStatus> is deprecated: use pixhwk_ctrl_node-msg:OmplStatus instead.")))

(cl:ensure-generic-function 'OmplSolStatus-val :lambda-list '(m))
(cl:defmethod OmplSolStatus-val ((m <OmplStatus>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader pixhwk_ctrl_node-msg:OmplSolStatus-val is deprecated.  Use pixhwk_ctrl_node-msg:OmplSolStatus instead.")
  (OmplSolStatus m))

(cl:ensure-generic-function 'actualmsg-val :lambda-list '(m))
(cl:defmethod actualmsg-val ((m <OmplStatus>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader pixhwk_ctrl_node-msg:actualmsg-val is deprecated.  Use pixhwk_ctrl_node-msg:actualmsg instead.")
  (actualmsg m))

(cl:ensure-generic-function 'stamp-val :lambda-list '(m))
(cl:defmethod stamp-val ((m <OmplStatus>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader pixhwk_ctrl_node-msg:stamp-val is deprecated.  Use pixhwk_ctrl_node-msg:stamp instead.")
  (stamp m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <OmplStatus>) ostream)
  "Serializes a message object of type '<OmplStatus>"
  (cl:let* ((signed (cl:slot-value msg 'OmplSolStatus)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 256) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    )
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'actualmsg))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'actualmsg))
  (cl:let ((__sec (cl:floor (cl:slot-value msg 'stamp)))
        (__nsec (cl:round (cl:* 1e9 (cl:- (cl:slot-value msg 'stamp) (cl:floor (cl:slot-value msg 'stamp)))))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __sec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __sec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __sec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __sec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 0) __nsec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __nsec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __nsec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __nsec) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <OmplStatus>) istream)
  "Deserializes a message object of type '<OmplStatus>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'OmplSolStatus) (cl:if (cl:< unsigned 128) unsigned (cl:- unsigned 256))))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'actualmsg) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'actualmsg) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
    (cl:let ((__sec 0) (__nsec 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __sec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __sec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __sec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __sec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 0) __nsec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __nsec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __nsec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __nsec) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'stamp) (cl:+ (cl:coerce __sec 'cl:double-float) (cl:/ __nsec 1e9))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<OmplStatus>)))
  "Returns string type for a message object of type '<OmplStatus>"
  "pixhwk_ctrl_node/OmplStatus")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'OmplStatus)))
  "Returns string type for a message object of type 'OmplStatus"
  "pixhwk_ctrl_node/OmplStatus")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<OmplStatus>)))
  "Returns md5sum for a message object of type '<OmplStatus>"
  "5e50e9584b597fecb0c7c001901c2ff9")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'OmplStatus)))
  "Returns md5sum for a message object of type 'OmplStatus"
  "5e50e9584b597fecb0c7c001901c2ff9")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<OmplStatus>)))
  "Returns full string definition for message of type '<OmplStatus>"
  (cl:format cl:nil "int8 OmplSolStatus~%string actualmsg~%time stamp~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'OmplStatus)))
  "Returns full string definition for message of type 'OmplStatus"
  (cl:format cl:nil "int8 OmplSolStatus~%string actualmsg~%time stamp~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <OmplStatus>))
  (cl:+ 0
     1
     4 (cl:length (cl:slot-value msg 'actualmsg))
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <OmplStatus>))
  "Converts a ROS message object to a list"
  (cl:list 'OmplStatus
    (cl:cons ':OmplSolStatus (OmplSolStatus msg))
    (cl:cons ':actualmsg (actualmsg msg))
    (cl:cons ':stamp (stamp msg))
))
