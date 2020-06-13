;; Auto-generated. Do not edit!


(when (boundp 'pixhwk_ctrl_node::PixHawk_Goal)
  (if (not (find-package "PIXHWK_CTRL_NODE"))
    (make-package "PIXHWK_CTRL_NODE"))
  (shadow 'PixHawk_Goal (find-package "PIXHWK_CTRL_NODE")))
(unless (find-package "PIXHWK_CTRL_NODE::PIXHAWK_GOAL")
  (make-package "PIXHWK_CTRL_NODE::PIXHAWK_GOAL"))

(in-package "ROS")
;;//! \htmlinclude PixHawk_Goal.msg.html


(defclass pixhwk_ctrl_node::PixHawk_Goal
  :super ros::object
  :slots (_Lat _Lon ))

(defmethod pixhwk_ctrl_node::PixHawk_Goal
  (:init
   (&key
    ((:Lat __Lat) 0.0)
    ((:Lon __Lon) 0.0)
    )
   (send-super :init)
   (setq _Lat (float __Lat))
   (setq _Lon (float __Lon))
   self)
  (:Lat
   (&optional __Lat)
   (if __Lat (setq _Lat __Lat)) _Lat)
  (:Lon
   (&optional __Lon)
   (if __Lon (setq _Lon __Lon)) _Lon)
  (:serialization-length
   ()
   (+
    ;; float64 _Lat
    8
    ;; float64 _Lon
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _Lat
       (sys::poke _Lat (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _Lon
       (sys::poke _Lon (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _Lat
     (setq _Lat (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _Lon
     (setq _Lon (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get pixhwk_ctrl_node::PixHawk_Goal :md5sum-) "6481a43b47ce4303d5bdcb88c9414b02")
(setf (get pixhwk_ctrl_node::PixHawk_Goal :datatype-) "pixhwk_ctrl_node/PixHawk_Goal")
(setf (get pixhwk_ctrl_node::PixHawk_Goal :definition-)
      "float64 Lat
float64 Lon

")



(provide :pixhwk_ctrl_node/PixHawk_Goal "6481a43b47ce4303d5bdcb88c9414b02")


