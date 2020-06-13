using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using SimpleJSON;

public class VehicleInfo : ROSBridgeMsg {
	string data;

	public VehicleInfo(string d){
		data=d;
	}

	public VehicleInfo(JSONNode msg) {
		data = msg["data"];
	}
	
	public static string getMessageType() {
		return "std_msgs/String";
	}
	
	
	public override string ToString() {
		return "String [action=" + data +"]";
	}
	
	public override string ToYAMLString() {
		return "{\"data\" : \"" + data + "\"}";
	}

}
