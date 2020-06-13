using System.Collections;
using System.Collections.Generic;
using UnityEngine;
[RequireComponent(typeof (CarController))
public class moveplayer : MonoBehaviour {
	int movement=-1;
	Quaternion targetDirection;
	public int collided=0;
	public int index=1;
	public Transform[] tangents;
	public float speed=0.06f;
	public GameObject track;
	public Vector3 startposition;
	public int startindex;
	public Quaternion startrotation;
	public int count=0,turned=1;
	// Use this for initialization
	void Start () {
		startrotation = transform.rotation;
		Transform[] tangents1 = track.GetComponentsInChildren<Transform> ();//.OrderBy(go =>go.name).ToArray();
		tangents =new Transform[tangents1.Length-1];
		for (int i=1; i<tangents1.Length; i++) {
			tangents[i-1]=tangents1[i];
		}

		startposition = transform.position;
		float dist = 1000f;
		for (int i=1; i<tangents.Length; i++) {
			float dis=Vector3.Distance(transform.position,tangents[i].position);
			if(dis<dist){
				dist=dis;
				index=i;
			}
		}
		speed = 0.06f;
		startindex = index;
		//resetcar ();
	}

	public void readMovement(string data){
		movement = int.Parse (data);
		if (movement == 0) {
			//targetDirection=Quaternion.Euler(new Vector3(0f,-1f,0f)+transform.rotation.eulerAngles);
			//transform.rotation=targetDirection;
			gameObject.GetComponent<CarController> ().Move (-1f,0f,0f,0f);
			turned=0;
		}
		if (movement == 1) {
			//targetDirection=Quaternion.Euler(new Vector3(0f,-0.75f,0f)+transform.rotation.eulerAngles);
			//transform.rotation=targetDirection;
			gameObject.GetComponent<CarController> ().Move (-0.75f,0f,0f,0f);
			turned=0;
		}
		if (movement == 2) {
			//targetDirection=Quaternion.Euler(new Vector3(0f,-0.5f,0f)+transform.rotation.eulerAngles);
			//transform.rotation=targetDirection;
			gameObject.GetComponent<CarController> ().Move (-0.5f,0f,0f,0f);
			turned=0;
		}

		if (movement == 4) {
			//targetDirection=Quaternion.Euler(new Vector3(0f,0.5f,0f)+transform.rotation.eulerAngles);
			//transform.rotation=targetDirection;
			gameObject.GetComponent<CarController> ().Move (0.5f,0f,0f,0f);
			turned=0;
		}
		if (movement == 5) {
			//targetDirection=Quaternion.Euler(new Vector3(0f,0.75f,0f)+transform.rotation.eulerAngles);
			//transform.rotation=targetDirection;
			gameObject.GetComponent<CarController> ().Move (0.75f,0f,0f,0f);
			turned=0;
		}
		if (movement == 6) {
			//targetDirection=Quaternion.Euler(new Vector3(0f,1f,0f)+transform.rotation.eulerAngles);
			//transform.rotation=targetDirection;
			gameObject.GetComponent<CarController> ().Move (1f,0f,0f,0f);
			turned=0;
		}
		//Debug.Log ((movement/2f)-1f+" ");
		//targetDirection=Quaternion.Euler(new Vector3(0f,(movement/2)-1f,0f)+transform.rotation.eulerAngles);
		//transform.rotation = targetDirection;
		//transform.position += transform.forward * speed;
		gameObject.GetComponent<CarController> ().Move (0f,speed*10f,0f,0f);

	}

	// Update is called once per frame
	void FixedUpdate () {
		/*if (movement !=-1) {
			transform.rotation=//Quaternion.Lerp(transform.rotation,targetDirection,Time.deltaTime*3f);
			transform.position += transform.forward * Time.deltaTime;
			float diff=targetDirection.eulerAngles.y-transform.rotation.eulerAngles.y;
			diff=Mathf.Abs(diff);
			Debug.Log (diff);
			if(diff<1f){
				movement=-1;
				turned=1;
			}

		}*/
		//readMovement ("1");
		if (collided == 1) {
			Debug.Log ("Collided");
			//transform.position=startposition;
			//transform.position=startposition;
			//transform.rotation=startrotation;
			//index=startindex;
		}


		if (gameObject.tag.Equals("Player") &&
		    Vector3.Distance (transform.position, tangents [nfmod ((index + 1) , tangents.Length)].position)<6f) {
			index++;
			index=nfmod(index,tangents.Length);
		}
		else if (gameObject.tag.Equals("Player1") &&
		         Vector3.Distance (transform.position, tangents [nfmod ((index - 1) , tangents.Length)].position)<6f) {
			index--;
			index=nfmod(index,tangents.Length);
		}

	}

	public void resetcar(){
		transform.position=new Vector3((35*Random.value)-32f,0.47f,(1.7f*Random.value)+9.3f);
		transform.rotation=startrotation;
		float dist = 1000f;
		for (int i=1; i<tangents.Length; i++) {
			float dis=Vector3.Distance(transform.position,tangents[i].position);
			if(dis<dist){
				dist=dis;
				index=i;
			}
		}
	}


	int nfmod(int a,int b)
	{
		return (a % b + b) % b;
	}
}
