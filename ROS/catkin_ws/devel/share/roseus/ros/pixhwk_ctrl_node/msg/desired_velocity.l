;; Auto-generated. Do not edit!


(when (boundp 'pixhwk_ctrl_node::desired_velocity)
  (if (not (find-package "PIXHWK_CTRL_NODE"))
    (make-package "PIXHWK_CTRL_NODE"))
  (shadow 'desired_velocity (find-package "PIXHWK_CTRL_NODE")))
(unless (find-package "PIXHWK_CTRL_NODE::DESIRED_VELOCITY")
  (make-package "PIXHWK_CTRL_NODE::DESIRED_VELOCITY"))

(in-package "ROS")
;;//! \htmlinclude desired_velocity.msg.html


(defclass pixhwk_ctrl_node::desired_velocity
  :super ros::object
  :slots (_velocity _priority _node_id ))

(defmethod pixhwk_ctrl_node::desired_velocity
  (:init
   (&key
    ((:velocity __velocity) 0.0)
    ((:priority __priority) 0)
    ((:node_id __node_id) 0)
    )
   (send-super :init)
   (setq _velocity (float __velocity))
   (setq _priority (round __priority))
   (setq _node_id (round __node_id))
   self)
  (:velocity
   (&optional __velocity)
   (if __velocity (setq _velocity __velocity)) _velocity)
  (:priority
   (&optional __priority)
   (if __priority (setq _priority __priority)) _priority)
  (:node_id
   (&optional __node_id)
   (if __node_id (setq _node_id __node_id)) _node_id)
  (:serialization-length
   ()
   (+
    ;; float64 _velocity
    8
    ;; int8 _priority
    1
    ;; int8 _node_id
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _velocity
       (sys::poke _velocity (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; int8 _priority
       (write-byte _priority s)
     ;; int8 _node_id
       (write-byte _node_id s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _velocity
     (setq _velocity (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; int8 _priority
     (setq _priority (sys::peek buf ptr- :char)) (incf ptr- 1)
     (if (> _priority 127) (setq _priority (- _priority 256)))
   ;; int8 _node_id
     (setq _node_id (sys::peek buf ptr- :char)) (incf ptr- 1)
     (if (> _node_id 127) (setq _node_id (- _node_id 256)))
   ;;
   self)
  )

(setf (get pixhwk_ctrl_node::desired_velocity :md5sum-) "e42d3ef493b1319a15d2180a4aa66278")
(setf (get pixhwk_ctrl_node::desired_velocity :datatype-) "pixhwk_ctrl_node/desired_velocity")
(setf (get pixhwk_ctrl_node::desired_velocity :definition-)
      "float64 velocity
				# 100 = edge_case(must be handled first);
				# 99 - 51 = less than edge, more than traffic;	
				# 49 - 2 = less than traffic, more than traj.
int8 priority
int8 node_id
")



(provide :pixhwk_ctrl_node/desired_velocity "e42d3ef493b1319a15d2180a4aa66278")


