using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using SimpleJSON;
namespace ROSBridgeLib{
	public class Action1 : ROSBridgeMsg {
		public string data;
		
		public Action1(string d){
			Debug.Log("action1 page1");
			data=d;
		}

		public Action1(JSONNode msg) {
			Debug.Log("action1 page");
			data = msg["data"];
		}

		public static string getMessageType() {
			return "std_msgs/String";
		}

		public string GetData(){
			return data;
		}

		public override string ToString() {
			Debug.Log("tostring");
			return "String [data=" + data +"]";
		}
		
		public override string ToYAMLString() {
			Debug.Log("yamlstring");
			return "{\"data\": " + data + "}";
		}
		
		
	}
}
