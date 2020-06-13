;; Auto-generated. Do not edit!


(when (boundp 'pixhwk_ctrl_node::Lanes)
  (if (not (find-package "PIXHWK_CTRL_NODE"))
    (make-package "PIXHWK_CTRL_NODE"))
  (shadow 'Lanes (find-package "PIXHWK_CTRL_NODE")))
(unless (find-package "PIXHWK_CTRL_NODE::LANES")
  (make-package "PIXHWK_CTRL_NODE::LANES"))

(in-package "ROS")
;;//! \htmlinclude Lanes.msg.html
(if (not (find-package "GEOMETRY_MSGS"))
  (ros::roseus-add-msgs "geometry_msgs"))
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass pixhwk_ctrl_node::Lanes
  :super ros::object
  :slots (_header _lane_left_points _lane_right_points ))

(defmethod pixhwk_ctrl_node::Lanes
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:lane_left_points __lane_left_points) (let (r) (dotimes (i 0) (push (instance geometry_msgs::Point32 :init) r)) r))
    ((:lane_right_points __lane_right_points) (let (r) (dotimes (i 0) (push (instance geometry_msgs::Point32 :init) r)) r))
    )
   (send-super :init)
   (setq _header __header)
   (setq _lane_left_points __lane_left_points)
   (setq _lane_right_points __lane_right_points)
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:lane_left_points
   (&rest __lane_left_points)
   (if (keywordp (car __lane_left_points))
       (send* _lane_left_points __lane_left_points)
     (progn
       (if __lane_left_points (setq _lane_left_points (car __lane_left_points)))
       _lane_left_points)))
  (:lane_right_points
   (&rest __lane_right_points)
   (if (keywordp (car __lane_right_points))
       (send* _lane_right_points __lane_right_points)
     (progn
       (if __lane_right_points (setq _lane_right_points (car __lane_right_points)))
       _lane_right_points)))
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; geometry_msgs/Point32[] _lane_left_points
    (apply #'+ (send-all _lane_left_points :serialization-length)) 4
    ;; geometry_msgs/Point32[] _lane_right_points
    (apply #'+ (send-all _lane_right_points :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; geometry_msgs/Point32[] _lane_left_points
     (write-long (length _lane_left_points) s)
     (dolist (elem _lane_left_points)
       (send elem :serialize s)
       )
     ;; geometry_msgs/Point32[] _lane_right_points
     (write-long (length _lane_right_points) s)
     (dolist (elem _lane_right_points)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; geometry_msgs/Point32[] _lane_left_points
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _lane_left_points (let (r) (dotimes (i n) (push (instance geometry_msgs::Point32 :init) r)) r))
     (dolist (elem- _lane_left_points)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;; geometry_msgs/Point32[] _lane_right_points
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _lane_right_points (let (r) (dotimes (i n) (push (instance geometry_msgs::Point32 :init) r)) r))
     (dolist (elem- _lane_right_points)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get pixhwk_ctrl_node::Lanes :md5sum-) "1c83d511d158b95dd22c4a5f998f865d")
(setf (get pixhwk_ctrl_node::Lanes :datatype-) "pixhwk_ctrl_node/Lanes")
(setf (get pixhwk_ctrl_node::Lanes :definition-)
      "std_msgs/Header header
geometry_msgs/Point32[] lane_left_points
geometry_msgs/Point32[] lane_right_points

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

================================================================================
MSG: geometry_msgs/Point32
# This contains the position of a point in free space(with 32 bits of precision).
# It is recommeded to use Point wherever possible instead of Point32.  
# 
# This recommendation is to promote interoperability.  
#
# This message is designed to take up less space when sending
# lots of points at once, as in the case of a PointCloud.  

float32 x
float32 y
float32 z
")



(provide :pixhwk_ctrl_node/Lanes "1c83d511d158b95dd22c4a5f998f865d")


