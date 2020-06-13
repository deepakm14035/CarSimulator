﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using SimpleJSON;

namespace ROSBridgeLib{
	namespace geometry_msgs{
		public class RealsenseImageSubscriber : ROSBridgeSubscriber {
			static GameObject ball;
			
			// These two are important
			public new static string GetMessageTopic() {
				return "/path/to/pose/topic";
			}
			
			public new static string GetMessageType() {
				return "std_msgs/PoseMsg";
			}
			
			// Important function (I think, converting json to PoseMsg)
			public new static ROSBridgeMsg ParseMessage(JSONNode msg) {
				return new PoseMsg (msg);
			}
			
			// This function should fire on each ros message
			public new static void CallBack(ROSBridgeMsg msg) {
				
				// Update ball position, or whatever
				//ball.x = msg.x; // Check msg definition in rosbridgelib
				//ball.y = msg.y;
				//ball.z = msg.z;
			}
		}
	}
}