;; Auto-generated. Do not edit!


(when (boundp 'pixhwk_ctrl_node::OmplStatus)
  (if (not (find-package "PIXHWK_CTRL_NODE"))
    (make-package "PIXHWK_CTRL_NODE"))
  (shadow 'OmplStatus (find-package "PIXHWK_CTRL_NODE")))
(unless (find-package "PIXHWK_CTRL_NODE::OMPLSTATUS")
  (make-package "PIXHWK_CTRL_NODE::OMPLSTATUS"))

(in-package "ROS")
;;//! \htmlinclude OmplStatus.msg.html


(defclass pixhwk_ctrl_node::OmplStatus
  :super ros::object
  :slots (_OmplSolStatus _actualmsg _stamp ))

(defmethod pixhwk_ctrl_node::OmplStatus
  (:init
   (&key
    ((:OmplSolStatus __OmplSolStatus) 0)
    ((:actualmsg __actualmsg) "")
    ((:stamp __stamp) (instance ros::time :init))
    )
   (send-super :init)
   (setq _OmplSolStatus (round __OmplSolStatus))
   (setq _actualmsg (string __actualmsg))
   (setq _stamp __stamp)
   self)
  (:OmplSolStatus
   (&optional __OmplSolStatus)
   (if __OmplSolStatus (setq _OmplSolStatus __OmplSolStatus)) _OmplSolStatus)
  (:actualmsg
   (&optional __actualmsg)
   (if __actualmsg (setq _actualmsg __actualmsg)) _actualmsg)
  (:stamp
   (&optional __stamp)
   (if __stamp (setq _stamp __stamp)) _stamp)
  (:serialization-length
   ()
   (+
    ;; int8 _OmplSolStatus
    1
    ;; string _actualmsg
    4 (length _actualmsg)
    ;; time _stamp
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int8 _OmplSolStatus
       (write-byte _OmplSolStatus s)
     ;; string _actualmsg
       (write-long (length _actualmsg) s) (princ _actualmsg s)
     ;; time _stamp
       (write-long (send _stamp :sec) s) (write-long (send _stamp :nsec) s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int8 _OmplSolStatus
     (setq _OmplSolStatus (sys::peek buf ptr- :char)) (incf ptr- 1)
     (if (> _OmplSolStatus 127) (setq _OmplSolStatus (- _OmplSolStatus 256)))
   ;; string _actualmsg
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _actualmsg (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; time _stamp
     (send _stamp :sec (sys::peek buf ptr- :integer)) (incf ptr- 4)  (send _stamp :nsec (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get pixhwk_ctrl_node::OmplStatus :md5sum-) "5e50e9584b597fecb0c7c001901c2ff9")
(setf (get pixhwk_ctrl_node::OmplStatus :datatype-) "pixhwk_ctrl_node/OmplStatus")
(setf (get pixhwk_ctrl_node::OmplStatus :definition-)
      "int8 OmplSolStatus
string actualmsg
time stamp

")



(provide :pixhwk_ctrl_node/OmplStatus "5e50e9584b597fecb0c7c001901c2ff9")


