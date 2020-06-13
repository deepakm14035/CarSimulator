
(cl:in-package :asdf)

(defsystem "pixhwk_ctrl_node-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
               :std_msgs-msg
)
  :components ((:file "_package")
    (:file "Lanes" :depends-on ("_package_Lanes"))
    (:file "_package_Lanes" :depends-on ("_package"))
    (:file "OmplStatus" :depends-on ("_package_OmplStatus"))
    (:file "_package_OmplStatus" :depends-on ("_package"))
    (:file "PixHawk_Goal" :depends-on ("_package_PixHawk_Goal"))
    (:file "_package_PixHawk_Goal" :depends-on ("_package"))
    (:file "PixHawk_SteeringControl" :depends-on ("_package_PixHawk_SteeringControl"))
    (:file "_package_PixHawk_SteeringControl" :depends-on ("_package"))
    (:file "PixHawk_ThrottleControl" :depends-on ("_package_PixHawk_ThrottleControl"))
    (:file "_package_PixHawk_ThrottleControl" :depends-on ("_package"))
    (:file "desired_velocity" :depends-on ("_package_desired_velocity"))
    (:file "_package_desired_velocity" :depends-on ("_package"))
    (:file "traffic_light" :depends-on ("_package_traffic_light"))
    (:file "_package_traffic_light" :depends-on ("_package"))
  ))