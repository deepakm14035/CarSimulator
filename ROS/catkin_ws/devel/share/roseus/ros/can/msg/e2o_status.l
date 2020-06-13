;; Auto-generated. Do not edit!


(when (boundp 'can::e2o_status)
  (if (not (find-package "CAN"))
    (make-package "CAN"))
  (shadow 'e2o_status (find-package "CAN")))
(unless (find-package "CAN::E2O_STATUS")
  (make-package "CAN::E2O_STATUS"))

(in-package "ROS")
;;//! \htmlinclude e2o_status.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass can::e2o_status
  :super ros::object
  :slots (_velocity _throttle _brake _steering _steering_direc _header ))

(defmethod can::e2o_status
  (:init
   (&key
    ((:velocity __velocity) 0)
    ((:throttle __throttle) 0)
    ((:brake __brake) 0)
    ((:steering __steering) 0)
    ((:steering_direc __steering_direc) 0)
    ((:header __header) (instance std_msgs::Header :init))
    )
   (send-super :init)
   (setq _velocity (round __velocity))
   (setq _throttle (round __throttle))
   (setq _brake (round __brake))
   (setq _steering (round __steering))
   (setq _steering_direc (round __steering_direc))
   (setq _header __header)
   self)
  (:velocity
   (&optional __velocity)
   (if __velocity (setq _velocity __velocity)) _velocity)
  (:throttle
   (&optional __throttle)
   (if __throttle (setq _throttle __throttle)) _throttle)
  (:brake
   (&optional __brake)
   (if __brake (setq _brake __brake)) _brake)
  (:steering
   (&optional __steering)
   (if __steering (setq _steering __steering)) _steering)
  (:steering_direc
   (&optional __steering_direc)
   (if __steering_direc (setq _steering_direc __steering_direc)) _steering_direc)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:serialization-length
   ()
   (+
    ;; int32 _velocity
    4
    ;; int32 _throttle
    4
    ;; int32 _brake
    4
    ;; int32 _steering
    4
    ;; int32 _steering_direc
    4
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32 _velocity
       (write-long _velocity s)
     ;; int32 _throttle
       (write-long _throttle s)
     ;; int32 _brake
       (write-long _brake s)
     ;; int32 _steering
       (write-long _steering s)
     ;; int32 _steering_direc
       (write-long _steering_direc s)
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32 _velocity
     (setq _velocity (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _throttle
     (setq _throttle (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _brake
     (setq _brake (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _steering
     (setq _steering (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _steering_direc
     (setq _steering_direc (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;;
   self)
  )

(setf (get can::e2o_status :md5sum-) "94b6373a082ac03fa17612f6941ccfa8")
(setf (get can::e2o_status :datatype-) "can/e2o_status")
(setf (get can::e2o_status :definition-)
      "int32 velocity
int32 throttle
int32 brake
int32 steering
int32 steering_direc
std_msgs/Header header
================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
# 0: no frame
# 1: global frame
string frame_id

")



(provide :can/e2o_status "94b6373a082ac03fa17612f6941ccfa8")


