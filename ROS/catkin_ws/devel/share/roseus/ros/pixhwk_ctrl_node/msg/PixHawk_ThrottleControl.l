;; Auto-generated. Do not edit!


(when (boundp 'pixhwk_ctrl_node::PixHawk_ThrottleControl)
  (if (not (find-package "PIXHWK_CTRL_NODE"))
    (make-package "PIXHWK_CTRL_NODE"))
  (shadow 'PixHawk_ThrottleControl (find-package "PIXHWK_CTRL_NODE")))
(unless (find-package "PIXHWK_CTRL_NODE::PIXHAWK_THROTTLECONTROL")
  (make-package "PIXHWK_CTRL_NODE::PIXHAWK_THROTTLECONTROL"))

(in-package "ROS")
;;//! \htmlinclude PixHawk_ThrottleControl.msg.html


(defclass pixhwk_ctrl_node::PixHawk_ThrottleControl
  :super ros::object
  :slots (_throttle ))

(defmethod pixhwk_ctrl_node::PixHawk_ThrottleControl
  (:init
   (&key
    ((:throttle __throttle) 0.0)
    )
   (send-super :init)
   (setq _throttle (float __throttle))
   self)
  (:throttle
   (&optional __throttle)
   (if __throttle (setq _throttle __throttle)) _throttle)
  (:serialization-length
   ()
   (+
    ;; float64 _throttle
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _throttle
       (sys::poke _throttle (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _throttle
     (setq _throttle (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get pixhwk_ctrl_node::PixHawk_ThrottleControl :md5sum-) "a0e4c91f838bf9ac9a81509ea028ea0b")
(setf (get pixhwk_ctrl_node::PixHawk_ThrottleControl :datatype-) "pixhwk_ctrl_node/PixHawk_ThrottleControl")
(setf (get pixhwk_ctrl_node::PixHawk_ThrottleControl :definition-)
      "float64 throttle

")



(provide :pixhwk_ctrl_node/PixHawk_ThrottleControl "a0e4c91f838bf9ac9a81509ea028ea0b")


