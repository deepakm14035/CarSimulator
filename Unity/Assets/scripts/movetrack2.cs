using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using System.Linq;
using UnityEngine.UI;
[RequireComponent(typeof (CarController))
public class movetrack2 : MonoBehaviour {
	public int index=1;
	public float speed=1.0f;
	float speed2=0.06f;
	RaycastHit hit,hit1;
	public GameObject track;
	public Transform[] tangents;
	GameObject[] go;				//list of all the cars
	int goflag=0;
	public int flag1=0;				// avoid other cars only if car is not close to a road boundary
	float hor,vert;
	public Text text;
	public GameObject stopper;
	float stoptime=0f;
	// Use this for initialization
	void Start () {

		//get list of waypoints
		Transform[] tangents1 = track.GetComponentsInChildren<Transform> ();//.OrderBy(go =>go.name).ToArray();
		tangents =new Transform[tangents1.Length-1];
		for (int i=1; i<tangents1.Length; i++) {
			tangents[i-1]=tangents1[i];
		}

		//if (speed > 1.3f)
		//	speed = 1.6f;


		//get waypoint closest to the car
		float dist = 1000f;
		for (int i=1; i<tangents.Length; i++) {
			float dis=Vector3.Distance(transform.position,tangents[i].position);
			if(dis<dist){
				dist=dis;
				index=i;
			}
		}
		goflag = 1;
	}
	
	// Update is called once per frame
	void FixedUpdate () {
		hor = 0f;
		vert = 0f;
		/*if(Vector3.Distance(stopper.transform.position,transform.position)<6f){
			stoptime+=Time.deltaTime;
			if(stoptime>2.8f){}
			else{
				gameObject.GetComponent<CarController> ().Move (0f,-0.02f,-0.02f,0f);
				return;
			}
		}*/
		Debug.DrawRay(transform.position, transform.forward*5, Color.green);
		Debug.DrawRay(transform.position, transform.right*0.8f, Color.red);
		Debug.DrawRay(transform.position, -transform.forward*2f, Color.red);

		//check left and right side if there is a road boundary and then turn
		flag1 = 0;
		if (Physics.Raycast (transform.position, transform.right, out hit, 0.5f)) {
			if ((hit.transform.parent.gameObject.name.Equals ("trackbounds")&&gameObject.tag.Equals("Player"))
			    ||(hit.transform.parent.gameObject.name.Equals ("trackbounds2")&&gameObject.tag.Equals("Player1"))) {
				hor-=0.1f;
				//transform.Rotate (Vector3.up * -15f * Time.deltaTime);
				flag1=1;
				//Debug.Log("something on right");
			}
		}if (Physics.Raycast (transform.position, -transform.right, out hit, 0.5f)) {
			if ((hit.transform.parent.gameObject.name.Equals ("trackbounds2")&&gameObject.tag.Equals("Player"))
			    ||(hit.transform.parent.gameObject.name.Equals ("trackbounds")&&gameObject.tag.Equals("Player1"))) {
				hor+=0.1f;
				//transform.Rotate (Vector3.up * 15f * Time.deltaTime);
				flag1=1;
				//Debug.Log("something on left");
			}
		}

		//check front direction for a road bounddary
		if (Physics.Raycast (transform.position, transform.forward, out hit, 3f)) {
			if (hit.transform.root.gameObject.name.Equals ("trackbounds")&&gameObject.tag.Equals("Player")) {
				hor+=0.1f;
				//transform.Rotate (Vector3.up * 15f * Time.deltaTime);
				flag1=1;
				//Debug.Log ("outertrack1");
			} else if (hit.transform.root.gameObject.name.Equals ("trackbounds2")&&gameObject.tag.Equals("Player")) {
				hor-=0.1f;
				//transform.Rotate (Vector3.up * -15f * Time.deltaTime);
				//Debug.Log ("innertrack1");
				flag1=1;
			}else if (hit.transform.root.gameObject.name.Equals ("trackbounds")&&gameObject.tag.Equals("Player1")) {
				hor-=0.1f;
				//transform.Rotate (Vector3.up * -15f * Time.deltaTime);
				flag1=1;
				//Debug.Log ("outertrack2");
			} else if (hit.transform.root.gameObject.name.Equals ("trackbounds2")&&gameObject.tag.Equals("Player1")) {
				hor+=0.1f;
				//transform.Rotate (Vector3.up * 15f * Time.deltaTime);
				flag1=1;
				//Debug.Log ("innertrack2");
			}
		}
		//initialization of variables
		if (goflag == 1) {
			GameObject[] go1 = GameObject.FindGameObjectsWithTag ("Player");
			GameObject[] go2 = GameObject.FindGameObjectsWithTag ("Player1");
			go = new GameObject[go1.Length + go2.Length];
			for (int i=0; i<go1.Length; i++) {
				go [i] = go1 [i];
			}
			for (int i=0; i<go2.Length; i++) {
				go [i + go1.Length] = go2 [i];
			}
			goflag=2;
		}

		Vector3 sum=new Vector3(0f,0f,0f);
		int count = 0;
		//check if car is close to a road boundary
		//flag1 is 1 if it is close to a road boundary
		if (flag1 == 0) {
			for (int i=0; i<go.Length; i++) {
				float disst = Vector3.Distance (go [i].transform.position, transform.position);
				//if a car is close to another vehicle, move this car in direction away from vehicle
				if (disst < 1.2f&&go[i].name!=gameObject.name) {
					//if(!Physics.Raycast (transform.position, transform.right, out hit, 2f)){
					Vector3 dirn = Vector3.Normalize (go [i].transform.position - transform.position);
					dirn.y = 0.0f;
					//if(speed<1.3f)
					//transform.position -= (dirn * speed * (2.35f - disst));
					Vector3 horVertmovement=transform.InverseTransformVector(dirn);
					//Debug.Log("horvert-"+horVertmovement);
					vert-=horVertmovement.z*0.06f;
					hor-=horVertmovement.x*0.22f;
				}

				//for vehicle flocking (not currently being used)
				if (disst < 3f) {
					sum = sum + go [i].transform.position;
					count++;
				}
				/*if (disst < 3.5f) {
					Vector3 dirn = Vector3.Normalize (go [i].transform.position - transform.position);
					//if (gameObject.name.Equals ("GameObject")) {
					//	Debug.Log(Vector3.Normalize(go[i].transform.position-transform.position));
					//}
					//dirn=dirn*100f;
					//Vector3 di=Vector3.Normalize(go[i].transform.position-transform.position);
					if (dirn.x > 0.9f && Mathf.Abs (dirn.y) < 0.1f && Mathf.Abs (dirn.z) < 0.1f && gameObject.tag.Equals ("Player")) {
						transform.Rotate (Vector3.up * 7f * speed * Time.deltaTime);
					} else if (dirn.x < -0.9f && Mathf.Abs (dirn.y) < 0.1f && Mathf.Abs (dirn.z) < 0.1f && gameObject.tag.Equals ("Player1")) {
						transform.Rotate (Vector3.up * 7f * speed * Time.deltaTime);
						//Debug.Log();
					}
				}*/
			}
		}
		/*if (count > 1) {
			sum = sum / count;
			if (gameObject.name.Equals ("GameObject")) {
				Debug.DrawRay (transform.position, sum, Color.yellow);
			}
			transform.position+=(sum-transform.position)*0.01f;
		}*/

		//if there is a car moving in opposite direction in front of this car, turn both vehicles
		if (Physics.Raycast (transform.position, transform.forward, out hit, 6f)&&flag1==0) {
			if((hit.transform.parent.transform.tag.Equals("Player")&&gameObject.tag.Equals("Player1"))||
			   hit.transform.parent.transform.tag.Equals("Player1")&&gameObject.tag.Equals("Player")){
				//Debug.Log(transform.right*0.5f/hit.distance);
				if(!Physics.Raycast (transform.position, transform.right, out hit, 2f)){
					//transform.position=transform.position+(-transform.right*0.01f/(2-hit.distance));
					//transform.Rotate (Vector3.up * 60f * Time.deltaTime);
					//transform.position+=(transform.right*0.03f);
					hor+=0.3f;
				}
				if(Physics.Raycast (transform.position, -transform.right, out hit, 2f)){
					//transform.position=transform.position+(-transform.right*0.01f*(2-hit.distance));
					//transform.Rotate (Vector3.up * 20f * Time.deltaTime);
					hor+=0.08f;
					//transform.position+=(transform.right*0.01f);
				}
			}
		}
		/*if (Physics.Raycast (transform.position, -transform.forward, out hit, 5f)) {
			if(hit.transform.parent.transform.tag.Equals("Player")){
				transform.position=transform.position+(transform.right*0.005f/hit.distance);
			}
		}*/

		/*
		if (Physics.Raycast (transform.position, -transform.forward, out hit, 3f)) {
			Debug.Log("back");
			if (hit.transform.parent.gameObject.tag.Equals ("Player")) {
				Debug.Log ("asdd");
				transform.position+= (transform.right * 2f * Time.deltaTime);
				//speed -= 0.01f;
				//transform.rotation=Quaternion.Slerp(transform.rotation,dir,Time.deltaTime*1);
			}
		}*/
		//change waypoint to next if it is closer to it
		if (gameObject.tag.Equals("Player") &&
		    Vector3.Distance (transform.position, tangents [nfmod ((index + 1) , tangents.Length)].position)<
		    Vector3.Distance (transform.position, tangents [index].position)) {
			index++;
			index=nfmod(index,tangents.Length);
		}
		else if (gameObject.tag.Equals("Player1") &&
		         Vector3.Distance (transform.position, tangents [nfmod ((index - 1) , tangents.Length)].position)<
		         Vector3.Distance (transform.position, tangents [index].position)) {
			index--;
			index=nfmod(index,tangents.Length);
		}
		/*
		if (index == 6) {
			if(Random.value>0.5f){
				index=35;
			}else
				index=7;
		}*/

		//change direction of the car to face direction of waypoint
		Vector3 direction = tangents [index].right;
		direction=new Vector3(direction.x,0f,direction.z);
		Vector3 angles;
		//Debug.Log (transform.rotation.eulerAngles);
		if (gameObject.tag.Equals ("Player")) {
			angles = tangents [nfmod ((index + 1) , tangents.Length)].rotation.eulerAngles;
			angles.y+=90f;
			float angledifference=(angles.y-transform.rotation.eulerAngles.y);
			if(angledifference>270f)angledifference-=360f;
			else if(angledifference<-270f)angledifference+=360f;

			hor+=(angledifference)/32f;
		} else {
			angles = tangents [nfmod((index - 1), tangents.Length)].rotation.eulerAngles;
			angles.y-=90f;
			float angledifference=(angles.y-transform.rotation.eulerAngles.y);
			if(angledifference>270f)angledifference-=360f;
			else if(angledifference<-270f)angledifference+=360f;

			hor+=(angledifference)/32f;
		}

		//move forward
		//gameObject.GetComponent<Rigidbody> ().AddForce (transform.forward * speed2 * speed);
		//transform.position += (transform.forward * speed2 * speed);//*20f/slowdown);
		vert+=(speed/5f);
		//Debug.Log("horvert-"+hor+","+vert);
		if(gameObject.name.Equals("Car"))
			text.text="vert-"+(vert/5f+",hor-"+hor/5f);
		gameObject.GetComponent<CarController> ().Move (hor/5f,vert/5f,vert/5f,0f);
		//transform.position += (Vector3.Normalize (direction)*speed2*speed);
		//Vector3 direction = transform.forward;

		/*
		if (speed > 1.3f) {
			Debug.DrawRay(transform.position-(0.35f*transform.right), transform.forward*4f, Color.red);
			Debug.DrawRay(transform.position+(0.35f*transform.right), transform.forward*4f, Color.red);


			if (Physics.Raycast (transform.position-(0.35f*transform.right), transform.forward, out hit, 4f)) {
				//Debug.Log("back");
				if (hit.transform.parent.gameObject.tag.Equals ("Player")) {
					hit.transform.parent.gameObject.transform.position-= (hit.transform.parent.gameObject.transform.right * 1f * Time.deltaTime);
				}
				else if(hit.transform.parent.gameObject.tag.Equals ("Player1")&&hit1.transform.parent.gameObject.GetComponent<movetrack2>().flag1==0){
					hit.transform.parent.gameObject.transform.position+= (hit.transform.parent.gameObject.transform.right * 1f * Time.deltaTime);
				}

			}
			else if(Physics.Raycast (transform.position+(0.35f*transform.right), transform.forward, out hit1, 4f)){
				if (hit1.transform.parent.gameObject.tag.Equals ("Player")&&hit1.transform.parent.gameObject.GetComponent<movetrack2>().flag1==0) {
					hit1.transform.parent.gameObject.transform.position+= (hit1.transform.parent.gameObject.transform.right * 1f * Time.deltaTime);
				}
				if(hit1.transform.parent.gameObject.tag.Equals ("Player1")){
					hit1.transform.parent.gameObject.transform.position-= (hit1.transform.parent.gameObject.transform.right * 1f * Time.deltaTime);
				}
			}
		}*/
	}

	void OnApplicationQuit(){
		//sw.Close ();
	}

	int nfmod(int a,int b)
	{
		return (a % b + b) % b;
	}
}
