
(cl:in-package :asdf)

(defsystem "can-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "e2o_status" :depends-on ("_package_e2o_status"))
    (:file "_package_e2o_status" :depends-on ("_package"))
  ))