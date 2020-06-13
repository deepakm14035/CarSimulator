using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using SimpleJSON;

namespace ROSBridgeLib{
	public class sendROS: ROSBridgePublisher {
		
		// The following three functions are important
		public static string GetMessageTopic() {
			return "/VehicleInfo";
		}
		
		public static string GetMessageType() {
			return "std_msgs/String";
		}
		
		public static string ToYAMLString(VehicleInfo msg) {
			return msg.ToYAMLString();
		}
		
		public new static ROSBridgeMsg ParseMessage(JSONNode msg) {
			return new VehicleInfo(msg);
		}    
	}
}