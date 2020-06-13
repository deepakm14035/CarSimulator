using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class followTrack : MonoBehaviour {
	public GameObject track1,track3,track5;
	Transform[] currentTrack;
	int trackNo=1;
	int cleared=0;
	int index=1;
	public float speed=1.0f;
	float speed2=0.06f;
	int changeLaneFlag=0;
	int slowdownflag = 0, speedupflag = 0;
	Transform[] t1,t3,t5;
	// Use this for initialization
	void Start () {
		currentTrack = track1.GetComponentsInChildren<Transform> ();
		t1 = track1.GetComponentsInChildren<Transform> ();
		t3 = track3.GetComponentsInChildren<Transform> ();
		t5 = track5.GetComponentsInChildren<Transform> ();

	}
	
	// Update is called once per frame
	void Update () {
		Debug.DrawRay(transform.position, transform.right*5, Color.green);
		Debug.DrawRay(transform.position, (transform.right-(0.25f*transform.forward))*5, Color.green);
		Debug.DrawRay(transform.position, (transform.right+(0.25f*transform.forward))*5, Color.green);
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
				trackNo=3;
			}
			else if(trackNo==1){
				trackNo=3;
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
			transform.rotation=Quaternion.Slerp(transform.rotation,currentTrack[(index+1)%currentTrack.Length].rotation,Time.deltaTime*0.55f);
			float diff=Vector3.Magnitude(transform.rotation.eulerAngles-currentTrack[index].rotation.eulerAngles);
			transform.position += (Vector3.Normalize (direction)*speed2*speed)*2f;
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
			currentTrack=t1;
		if(trackNo==3)
			currentTrack=t3;
		if(trackNo==5)
			currentTrack=t5;

	}
}
