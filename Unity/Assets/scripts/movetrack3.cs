using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class movetrack3 : MonoBehaviour {
	public int index=1;
	public float speed=1.0f;
	float speed2=0.06f;
	RaycastHit hit,hit1;
	public GameObject track;
	public Transform[] tangents;
	GameObject[] go;
	int goflag=0;
	public int flag1=0,flag2=0,flag3=0;
	StreamWriter sw;
	int frameno=0;
	Vector3 resultant;
	// Use this for initialization
	void Start () {
		if (speed > 1.3f)
			speed = 1.6f;
		Transform[] tangents1 = track.GetComponentsInChildren<Transform> ();
		tangents =new Transform[tangents1.Length-1];
		for (int i=1; i<tangents1.Length; i++) {
			tangents[i-1]=tangents1[i];
		}
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
	void Update () {
		
		resultant = new Vector3 (0f,0f,0f);
		
		Debug.DrawRay(transform.position, transform.forward*5, Color.green);
		Debug.DrawRay(transform.position, transform.right*0.8f, Color.red);
		Debug.DrawRay(transform.position, -transform.forward*2f, Color.red);
		flag1 = 0;
		if (Physics.Raycast (transform.position, transform.right, out hit, 2f)) {
			if ((hit.transform.parent.gameObject.tag.Equals ("trackbounds")&&transform.tag.Equals("Player"))
			    ||(hit.transform.parent.gameObject.tag.Equals ("trackbounds2")&&transform.tag.Equals("Player1"))) {
				transform.position-= (transform.right * speed * Time.deltaTime*8);
				resultant-=(transform.right * speed * Time.deltaTime*8);
				flag1=1;
			}
		}if (Physics.Raycast (transform.position, -transform.right, out hit, 2f)) {
			if ((hit.transform.parent.gameObject.tag.Equals ("trackbounds2")&&transform.tag.Equals("Player"))
			    ||(hit.transform.parent.gameObject.tag.Equals ("trackbounds")&&transform.tag.Equals("Player1"))) {
				transform.position+= (transform.right * speed * Time.deltaTime*8);
				resultant+=(transform.right * speed * Time.deltaTime*8);
				flag1=1;
			}
		}
		
		
		if (Physics.Raycast (transform.position, transform.forward, out hit, 3f)) {
			if (hit.transform.parent.gameObject.name.Equals ("trackbounds")&&gameObject.tag.Equals("Player")) {
				transform.Rotate (Vector3.up * 20f * Time.deltaTime);
				flag1=1;
			} else if (hit.transform.parent.transform.gameObject.name.Equals ("trackbounds2")&&gameObject.tag.Equals("Player")) {
				transform.Rotate (Vector3.up * -20f * Time.deltaTime);
				flag1=1;
			}else if (hit.transform.parent.gameObject.name.Equals ("trackbounds")&&gameObject.tag.Equals("Player1")) {
				transform.Rotate (Vector3.up * -20f * Time.deltaTime);
				flag1=1;
			} else if (hit.transform.parent.transform.gameObject.name.Equals ("trackbounds2")&&gameObject.tag.Equals("Player1")) {
				transform.Rotate (Vector3.up * 20f * Time.deltaTime);
				flag1=1;
			}
		}

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
			sw=new StreamWriter(gameObject.name+".txt");
		}
		for (int i=0; i<go.Length; i++) {
			if(Vector3.Distance(transform.position,go[i].transform.position)<20f){
				float anglee=Vector3.Angle(transform.forward,go[i].transform.position-transform.position);
				if(anglee>-60f&anglee<=60f){
					sw.WriteLine(frameno+","+go[i].name+","+go[i].transform.position+","+transform.position);
					sw.Flush();
				}
			}
		}
		frameno++;
		Vector3 sum=new Vector3(0f,0f,0f);
		int count = 0;
		if (flag1 == 0) {
			for (int i=0; i<go.Length; i++) {
				float disst = Vector3.Distance (go [i].transform.position, transform.position);
				if (disst < 1.5f) {
					//if(!Physics.Raycast (transform.position, transform.right, out hit, 2f)){
					Vector3 dirn = Vector3.Normalize (go [i].transform.position - transform.position) * 0.01f;
					dirn.y = 0.0f;
					if(speed<1.3f){
						transform.position -= (dirn * speed * (2.25f - disst));
						resultant-=(dirn * speed * (2.25f - disst));
					}
					if(go [i].GetComponent<movetrack2>().speed<1.3f){
						go [i].transform.position += (dirn * go [i].GetComponent<movetrack2> ().speed * (2.75f - disst));
						go [i].GetComponent<movetrack3>().resultant+=(dirn * go [i].GetComponent<movetrack2> ().speed * (2.75f - disst));
					}
					
				}
				if (disst < 3f) {
					sum = sum + go [i].transform.position;
					count++;
				}
				if (disst < 3.5f) {
					Vector3 dirn = Vector3.Normalize (go [i].transform.position - transform.position);

					if (dirn.x > 0.9f && Mathf.Abs (dirn.y) < 0.1f && Mathf.Abs (dirn.z) < 0.1f && gameObject.tag.Equals ("Player")&&speed<1.3f) {
						transform.Rotate (Vector3.up * 7f * speed * Time.deltaTime);
					} else if (dirn.x < -0.9f && Mathf.Abs (dirn.y) < 0.1f && Mathf.Abs (dirn.z) < 0.1f && gameObject.tag.Equals ("Player1")&&speed<1.3f) {
						transform.Rotate (Vector3.up * 7f * speed * Time.deltaTime);
						//Debug.Log();
					}
				}

			}
		}

		
		if (Physics.Raycast (transform.position, transform.forward, out hit, 6f)&&flag1==0) {
			if((hit.transform.parent.transform.tag.Equals("Player")&&gameObject.tag.Equals("Player1"))||
			   hit.transform.parent.transform.tag.Equals("Player1")&&gameObject.tag.Equals("Player")){
				//Debug.Log(transform.right*0.5f/hit.distance);
				if(!Physics.Raycast (transform.position, transform.right, out hit, 2f)){
					//transform.position=transform.position+(-transform.right*0.01f/(2-hit.distance));
					transform.Rotate (Vector3.up * 60f * Time.deltaTime);
					transform.position+=(transform.right*0.03f);
					resultant+=(transform.right*0.03f);
				}
				if(Physics.Raycast (transform.position, -transform.right, out hit, 2f)){
					//transform.position=transform.position+(-transform.right*0.01f*(2-hit.distance));
					transform.Rotate (Vector3.up * 20f * Time.deltaTime);
					transform.position+=(transform.right*0.01f);
					resultant+=(transform.right*0.01f);
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
		Vector3 direction = tangents [index].right;
		direction=new Vector3(direction.x,0f,direction.z);
		//Debug.Log (transform.rotation.eulerAngles);
		if (gameObject.tag.Equals ("Player")) {
			Vector3 angles = tangents [nfmod ((index + 1) , tangents.Length)].rotation.eulerAngles;
			angles.y+=90f;
			transform.rotation = Quaternion.Slerp (transform.rotation, Quaternion.Euler(angles), speed*Time.deltaTime * 0.95f);
			//transform.position += (direction*speed2*speed);
		} else {
			Vector3 angles = tangents [nfmod((index - 1), tangents.Length)].rotation.eulerAngles;
			angles.y-=90f;
			transform.rotation = Quaternion.Slerp (transform.rotation, Quaternion.Euler(angles), speed*Time.deltaTime * 0.95f);
			//transform.position += (-direction*speed2*speed);
		}
		transform.position += (transform.forward*speed2*speed);
		resultant+=(transform.forward*speed2*speed);
		//transform.position += (Vector3.Normalize (direction)*speed2*speed);
		//Vector3 direction = transform.forward;
		
		
		
		if (speed > 1.3f) {
			Debug.DrawRay(transform.position-(0.35f*transform.right), transform.forward*4f, Color.red);
			Debug.DrawRay(transform.position+(0.35f*transform.right), transform.forward*4f, Color.red);
			
			
			if (Physics.Raycast (transform.position-(0.35f*transform.right), transform.forward, out hit, 4f)) {
				Debug.Log("back");
				if (hit.transform.parent.gameObject.tag.Equals ("Player")) {
					hit.transform.parent.gameObject.transform.position-= (hit.transform.parent.gameObject.transform.right * 1f * Time.deltaTime);
					hit.transform.parent.gameObject.GetComponent<movetrack3>().resultant-=(hit.transform.parent.gameObject.transform.right * 1f * Time.deltaTime);
				}
				else if(hit.transform.parent.gameObject.tag.Equals ("Player1")&&hit1.transform.parent.gameObject.GetComponent<movetrack2>().flag1==0){
					hit.transform.parent.gameObject.transform.position+= (hit.transform.parent.gameObject.transform.right * 1f * Time.deltaTime);
					hit.transform.parent.gameObject.GetComponent<movetrack3>().resultant+=(hit.transform.parent.gameObject.transform.right * 1f * Time.deltaTime);
				}
				
			}
			else if(Physics.Raycast (transform.position+(0.35f*transform.right), transform.forward, out hit1, 4f)){
				if (hit1.transform.parent.gameObject.tag.Equals ("Player")&&hit1.transform.parent.gameObject.GetComponent<movetrack2>().flag1==0) {
					hit1.transform.parent.gameObject.transform.position+= (hit1.transform.parent.gameObject.transform.right * 1f * Time.deltaTime);
					hit1.transform.parent.gameObject.GetComponent<movetrack3>().resultant+=(hit1.transform.parent.gameObject.transform.right * 1f * Time.deltaTime);
				}
				if(hit1.transform.parent.gameObject.tag.Equals ("Player1")){
					hit1.transform.parent.gameObject.transform.position-= (hit1.transform.parent.gameObject.transform.right * 1f * Time.deltaTime);
					hit1.transform.parent.gameObject.GetComponent<movetrack3>().resultant-=(hit1.transform.parent.gameObject.transform.right * 1f * Time.deltaTime);
				}
			}
		}
		//write to file
	}
	int nfmod(int a,int b)
	{
		return (a % b + b) % b;
	}
}
