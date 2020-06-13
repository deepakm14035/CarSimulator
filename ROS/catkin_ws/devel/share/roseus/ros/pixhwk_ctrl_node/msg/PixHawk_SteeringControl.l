;; Auto-generated. Do not edit!


(when (boundp 'pixhwk_ctrl_node::PixHawk_SteeringControl)
  (if (not (find-package "PIXHWK_CTRL_NODE"))
    (make-package "PIXHWK_CTRL_NODE"))
  (shadow 'PixHawk_SteeringControl (find-package "PIXHWK_CTRL_NODE")))
(unless (find-package "PIXHWK_CTRL_NODE::PIXHAWK_STEERINGCONTROL")
  (make-package "PIXHWK_CTRL_NODE::PIXHAWK_STEERINGCONTROL"))

(in-package "ROS")
;;//! \htmlinclude PixHawk_SteeringControl.msg.html


(defclass pixhwk_ctrl_node::PixHawk_SteeringControl
  :super ros::object
  :slots (_steering ))

(defmethod pixhwk_ctrl_node::PixHawk_SteeringControl
  (:init
   (&key
    ((:steering __steering) 0.0)
    )
   (send-super :init)
   (setq _steering (float __steering))
   self)
  (:steering
   (&optional __steering)
   (if __steering (setq _steering __steering)) _steering)
  (:serialization-length
   ()
   (+
    ;; float64 _steering
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _steering
       (sys::poke _steering (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _steering
     (setq _steering (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get pixhwk_ctrl_node::PixHawk_SteeringControl :md5sum-) "13be5889908f58ce029441890e49203c")
(setf (get pixhwk_ctrl_node::PixHawk_SteeringControl :datatype-) "pixhwk_ctrl_node/PixHawk_SteeringControl")
(setf (get pixhwk_ctrl_node::PixHawk_SteeringControl :definition-)
      "float64 steering

")



(provide :pixhwk_ctrl_node/PixHawk_SteeringControl "13be5889908f58ce029441890e49203c")


