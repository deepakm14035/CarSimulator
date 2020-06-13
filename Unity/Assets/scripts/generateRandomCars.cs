using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class generateRandomCars : MonoBehaviour {
	public GameObject track1,track3,track5;
	Transform[] currentTrack;
	int trackNo=1;
	int cleared=0;
	int index=1;
	public float speed=1.0f;
	float speed2=0.06f;
	int changeLaneFlag=0;
	int slowdownflag = 0, speedupflag = 0;
	// Use this for initialization
	void Start () {
		track1=GameObject.Find("trackbounds");
		track3=GameObject.Find("Track2");
		track5=GameObject.Find("Track3");

		currentTrack = track1.GetComponentsInChildren<Transform> ();
		index = (int)(Random.value * (currentTrack.Length-1));
		speed = Random.value + 0.4f;
		//transform.Translate (new Vector3(0,-2.8f,0));
		transform.position += currentTrack [index].transform.position - transform.position+new Vector3(0,0.23f,0);
		transform.rotation = Quaternion.Euler(currentTrack [index].transform.rotation.eulerAngles - transform.rotation.eulerAngles);
	}
	
	// Update is called once per frame
	void Update () {
		Debug.DrawRay(transform.position, transform.right*3, Color.green);
		if (slowdownflag == 1) {
			speed -= 0.1f;
			if(speed<=0.2f){
				slowdownflag=0;
				speed=0.0f;
				changeLaneFlag=0;
			}
		}
		if (speedupflag == 1) {
			speed += 0.1f;
			if(speed>=1.0f){
				speedupflag=0;
				speed=1.0f;
			}
		}
		
		if (Physics.Raycast (transform.position, transform.right, 5f)&& changeLaneFlag==0) {
			Debug.Log("checking");
			if(trackNo==5){
				if(Physics.Raycast (transform.position, transform.right+(0.25f*transform.forward), 5f)
				   && Physics.Raycast (transform.position, transform.right+(0.5f*transform.forward), 6f)){
					speed-=0.1f;
					slowdownflag=1;
					Debug.Log("1");
				}
				else{
					Debug.Log("2");
					speedupflag=1;
					trackNo=3;
				}
			}
			else if(trackNo==1){
				if(Physics.Raycast (transform.position, transform.right-(0.25f*transform.forward), 5f)
				   && Physics.Raycast (transform.position, transform.right-(0.5f*transform.forward), 6f)){
					slowdownflag=1;
					speed-=0.1f;
					Debug.Log("3");
				}
				else{
					trackNo=3;
					speedupflag=1;
					Debug.Log("4");
				}
			}
			else{
				if(Physics.Raycast (transform.position, transform.right+(0.25f*transform.forward), 5f)
				   && Physics.Raycast (transform.position, transform.right-(0.25f*transform.forward), 5f)){
					speed-=0.1f;
					slowdownflag=1;
					Debug.Log("5");
				}
				else if(Physics.Raycast (transform.position, transform.right+(0.25f*transform.forward), 5f)){
					trackNo=5;
					speedupflag=1;
					Debug.Log("6");
				}
				else if(Physics.Raycast (transform.position, transform.right-(0.25f*transform.forward), 5f)){
					trackNo=1;
					Debug.Log("7");
					speedupflag=1;
				}
				else{
					Debug.Log ("8");
					trackNo=5;
					speedupflag=1;
				}
			}
			putInRange();
			changeLaneFlag=1;
		}
		if (cleared == 0) {
			Debug.Log("moving");
			Vector3 direction = currentTrack [index].position - transform.position;
			direction=new Vector3(direction.x,0,direction.z);
			transform.rotation=Quaternion.Slerp(transform.rotation,currentTrack[index].rotation,Time.deltaTime*1);
			float diff=Vector3.Magnitude(transform.rotation.eulerAngles-currentTrack[index].rotation.eulerAngles);
			/*if(diff>25)
				speed2=0.05f;
			else
				speed2=0.08f;
			Debug.Log (speed2);
			*/
			transform.position += (Vector3.Normalize (direction)*speed2*speed);
			if (Vector3.Magnitude(direction) < 1)
				cleared = 1;
		} else {
			index+=1;
			if(index==currentTrack.Length)
				index=1;
			cleared=0;
			changeLaneFlag=0;
		}
	}
	void putInRange(){
		trackNo=Mathf.Clamp(trackNo,1,5);
		if(trackNo==1)
			currentTrack=track1.GetComponentsInChildren<Transform> ();
		if(trackNo==3)
			currentTrack=track3.GetComponentsInChildren<Transform> ();
		if(trackNo==5)
			currentTrack=track5.GetComponentsInChildren<Transform> ();
		
	}
}
