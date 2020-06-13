using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using SimpleJSON;
using ROSBridgeLib;

//subscriber
public class Receiver : ROSBridgeSubscriber {
	public static string action;
	
	// These two are important
	public new static string GetMessageTopic() {
		return "/ActionInfo";
	}
	
	public new static string GetMessageType() {
		return "std_msgs/String";
	}
	
	// Important function (I think, converting json to PoseMsg)
	public new static ROSBridgeMsg ParseMessage(JSONNode msg) {
		//Debug.Log("somethinggggg");
		return new Action1 (msg);
	}
	
	// This function should fire on each ros message
	public new static void CallBack(ROSBridgeMsg msg) {
		//Debug.Log("---recvd something");
		// Update ball position, or whatever
		action = ((Action1)msg).data; // Check msg definition in rosbridgelib
		//5/12/2019
		//Debug.Log("received - "+action);
	}
}
