using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using SocketIOClient;
//
//roslaunch rosbridge_server rosbridge_websocket.launch
//source devel/setup.bash
//roscore
//roslaunch unity_simulation_scene unity_simulation_scene.launch
//rosrun carsim listener
//catkin_make to build project
//could not process inbound connection - remove \n from message string
//make install in ompl-1.3.1-Source/demos
namespace ROSBridgeLib{
	namespace geometry_msgs{
		namespace SocketIOClient{
			public class sendToROS : MonoBehaviour {
				private ROSBridgeWebSocketConnection ros = null;
				public GameObject maincar;
				// Use this for initialization
				void Start () {
					//ros = new ROSBridgeWebSocketConnection ("ws://localhost", 11311);
					ros = new ROSBridgeWebSocketConnection ("ws://localhost", 9090);

					// Add subscribers and publishers (if any)
					ros.AddPublisher (typeof(sendROS));
					ros.AddSubscriber (typeof(Receiver));
					ros.AddServiceResponse(typeof(CarServiceResponse));
					//ros.AddPublisher (typeof(BallControlPublisher));
					//ros.CallService();
					// Fire up the subscriber(s) and publisher(s)
					ros.Connect ();

					
				}
				
				void OnApplicationQuit() {
					if(ros!=null) {
						ros.Disconnect ();
					}
				}
				float time=0f;
				public bool sendFlag=false;
				// Update is called once per frame in Unity
				void Update () {
					if(Receiver.action!=null){
						//Debug.Log("asdasd---"+Receiver.action);
						maincar.GetComponent<moveplayer2> ().readMovement1 (Receiver.action);
						Receiver.action=null;
						fetchData.sentflag = 0; 
					}
					if(sendFlag){
						//5/12/2019
						//Debug.Log (GetComponent<fetchData>().dataSent);
						ros.Publish("/VehicleInfo",new VehicleInfo(GetComponent<fetchData>().dataSent));
						Debug.Log("sending...");
						sendFlag=false;
					}
					ros.Render ();
				}
			}
		}
	}
}