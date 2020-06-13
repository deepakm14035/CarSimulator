;; Auto-generated. Do not edit!


(when (boundp 'pixhwk_ctrl_node::traffic_light)
  (if (not (find-package "PIXHWK_CTRL_NODE"))
    (make-package "PIXHWK_CTRL_NODE"))
  (shadow 'traffic_light (find-package "PIXHWK_CTRL_NODE")))
(unless (find-package "PIXHWK_CTRL_NODE::TRAFFIC_LIGHT")
  (make-package "PIXHWK_CTRL_NODE::TRAFFIC_LIGHT"))

(in-package "ROS")
;;//! \htmlinclude traffic_light.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass pixhwk_ctrl_node::traffic_light
  :super ros::object
  :slots (_header _dist_light _color_light ))

(defmethod pixhwk_ctrl_node::traffic_light
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:dist_light __dist_light) 0.0)
    ((:color_light __color_light) "")
    )
   (send-super :init)
   (setq _header __header)
   (setq _dist_light (float __dist_light))
   (setq _color_light (string __color_light))
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:dist_light
   (&optional __dist_light)
   (if __dist_light (setq _dist_light __dist_light)) _dist_light)
  (:color_light
   (&optional __color_light)
   (if __color_light (setq _color_light __color_light)) _color_light)
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; float64 _dist_light
    8
    ;; string _color_light
    4 (length _color_light)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; float64 _dist_light
       (sys::poke _dist_light (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; string _color_light
       (write-long (length _color_light) s) (princ _color_light s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; float64 _dist_light
     (setq _dist_light (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; string _color_light
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _color_light (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(setf (get pixhwk_ctrl_node::traffic_light :md5sum-) "f8df0c506b0f4a93a3c764f8dfe78ca5")
(setf (get pixhwk_ctrl_node::traffic_light :datatype-) "pixhwk_ctrl_node/traffic_light")
(setf (get pixhwk_ctrl_node::traffic_light :definition-)
      "Header header
float64 dist_light
string color_light
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



(provide :pixhwk_ctrl_node/traffic_light "f8df0c506b0f4a93a3c764f8dfe78ca5")


