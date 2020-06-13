using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using SimpleJSON;

namespace ROSBridgeLib{
	namespace geometry_msgs{
		public class BallControlPublisher: ROSBridgePublisher {
			
			// The following three functions are important
			public static string GetMessageTopic() {
				return "/VehicleInfo";
			}
			
			public static string GetMessageType() {
				return "std_msgs/TwistMsg";
			}
			
			public static string ToYAMLString(TwistMsg msg) {
				return msg.ToYAMLString();
			}
			
			public new static ROSBridgeMsg ParseMessage(JSONNode msg) {
				return new TwistMsg(msg);
			}    
		}
	}
}