using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class moveplayer2 : MonoBehaviour {
	public GameObject intersections1,directions1,currentdirn;
	public Transform[] intersections,directions;
	public int[,] direction,endpoints;
	public int currentDirection,currentintersection,c_intersection,c_direction,t_intersection;
	public float hor,vert;
	StreamReader reader;
	public int targetIntersection;
	public int slowTurnIntersections=15;
	Rigidbody rigidbody;

	float movement=-1;
	Quaternion targetDirection;
	public int collided=0;
	public int index=1;
	public float speed=0.5f;
	public Vector3 startposition;
	public int startindex;
	public Quaternion startrotation;
	public int count=0,turned=1;
	// Use this for initialization
	void Start () {
		rigidbody = gameObject.GetComponent<Rigidbody> ();
		intersections = intersections1.GetComponentsInChildren <Transform>();
		Transform[] intersections2=new Transform[intersections.Length-1];
		for (int i=0; i<intersections.Length-1; i++) {
			intersections2[i]=intersections[i+1];
		}
		intersections = intersections2;
		directions = directions1.GetComponentsInChildren <Transform>();
		Transform[] directions2=new Transform[directions.Length-1];
		for (int i=0; i<directions.Length-1; i++) {
			directions2[i]=directions[i+1];
		}
		directions = directions2;
		direction=new int[intersections.Length,4];
		
		
		Debug.Log ("length-"+direction.Length);
		reader = new StreamReader ("bigcity.txt");
		string str;
		int index = 0;
		string[] arr;
		while ((str=reader.ReadLine())!=null) {
			arr=str.Split(' ');
			for(int i=1;i<arr.Length;i++){
				
				direction[index,i-1]=int.Parse(arr[i]);
			}
			if(arr.Length<5){
				for(int i=arr.Length;i<5;i++){
					direction[index,i-1]=-1;
				}
			}
			index++;
		}
		index = 0;
		endpoints=new int[directions.Length,2];
		reader = new StreamReader ("edges.txt");
		while ((str=reader.ReadLine())!=null) {
			//Debug.Log(str);
			arr=str.Split(' ');
			endpoints[index,0]=int.Parse(arr[0]);
			endpoints[index,1]=int.Parse(arr[1]);
			//Debug.Log(endpoints[index,0]+","+endpoints[index,1]);
			index=index+1;
		}
		
		
		float mindist = 999f;
		int minindex = 0;
		for(int i=1;i<intersections.Length;i++){
			float angle = Vector3.Angle (intersections[i].position-transform.position,transform.forward);
			if(Mathf.Abs(angle)>160f && Vector3.Distance(transform.position,intersections[i].position)<mindist){
				minindex=i;
				mindist=Vector3.Distance(transform.position,intersections[i].position);
			}
			
		}
		currentintersection = minindex;
		int closeindex=0;
		float closedist = 999f;
		for (int j=0; j<4; j++) {
			if(direction[minindex,j]==-1)
				break;
			float angle = Vector3.Angle (directions[direction[minindex,j]].forward,transform.forward);
			if(Mathf.Abs(angle)<30f && direction[minindex,j]!=-1 && Vector3.Distance(directions[direction[minindex,j]].position,transform.position)<closedist){
				closeindex=j;
				closedist=Vector3.Distance(directions[direction[minindex,j]].position,transform.position);
			}
		}
		currentDirection = closeindex;
		findOtherEnd (currentDirection,currentintersection);
		
		currentdirn=directions[direction[currentintersection,currentDirection]].gameObject;

		c_direction=currentDirection;
		c_intersection=currentintersection;
		t_intersection=targetIntersection;

		startrotation = transform.rotation;
		startposition = transform.position;
		speed = 1f;
		startindex = index;
		//resetcar ();
	}

	void findOtherEnd(int intersect, int other){
		//Debug.Log (other+","+intersect+","+endpoints [direction[other,intersect], 0]+"--"+endpoints [direction[other,intersect], 1]);
		if (endpoints [direction[other,intersect], 0] == other) {
			targetIntersection=endpoints [direction[other,intersect], 1];
		}
		else{
			targetIntersection=endpoints [direction[other,intersect], 0];
		}
	}
	
	public void readMovement(string data){
		movement = int.Parse (data);
		if (movement == 0) {
			gameObject.GetComponent<CarController> ().Move (-0.05f,speed,speed,0f);
			turned=0;
		}
		if (movement == 1) {
			gameObject.GetComponent<CarController> ().Move (-0.0375f,speed,speed,0f);
			turned=0;
		}
		if (movement == 2) {
			gameObject.GetComponent<CarController> ().Move (-0.025f,speed,speed,0f);
			turned=0;
		}
		if(movement==3){
			gameObject.GetComponent<CarController> ().Move (0f,speed,speed,0f);
			turned=0;
		}
		
		if (movement == 4) {
			gameObject.GetComponent<CarController> ().Move (0.025f,speed,speed,0f);
			turned=0;
		}
		if (movement == 5) {
			gameObject.GetComponent<CarController> ().Move (0.0375f,speed,speed,0f);
			turned=0;
		}
		if (movement == 6) {
			gameObject.GetComponent<CarController> ().Move (0.05f,speed,speed,0f);
			turned=0;
		}
		
	}

	public void readMovement1(string data){
		movement = (float)double.Parse (data.Split(',')[0]);
		speed = (float)double.Parse (data.Split (',') [1]);///30;
		//Debug.Log("movement-"+movement);
		//Debug.Log("speed-"+speed);

		//if (movement <0) {
		/*if (gameObject.GetComponent<CarController> ().CurrentSpeed > speed)
			speed = 0f;
		else
			speed = 0.5f;
			*/
		speed=speed*0.5f;
		gameObject.GetComponent<CarController> ().Move (movement*Time.deltaTime*2f,speed,speed,0f);

		gameObject.GetComponent<CarController> ().m_Rigidbody.velocity = speed*gameObject.GetComponent<CarController> ().m_Rigidbody.velocity.normalized;
		Debug.Log ("current speed-"+gameObject.GetComponent<CarController> ().CurrentSpeed);
		/*	turned=0;
		}
		if(movement==0){
			gameObject.GetComponent<CarController> ().Move (0f,speed,speed,0f);
			turned=0;
		}

		if (movement >0) {
			gameObject.GetComponent<CarController> ().Move (0.25f,speed,speed,0f);
			turned=0;
		}*/

	}


	// Update is called once per frame
	void FixedUpdate () {
		//readMovement ("1");
		if (collided == 1) {
			Debug.Log ("Collided");
		}
		
		
		if (gameObject.tag.Equals("Player") &&
		    Vector3.Distance (transform.position, intersections [targetIntersection].position)<2f) {
			setNewIntersection();
		}
		
	}

	public void setNewIntersection(){
		//Debug.Log(Vector3.Dot(intersections [targetIntersection].position-transform.position,transform.forward));
		if(Vector3.Dot(intersections [targetIntersection].position-transform.position,transform.forward)>-1f) return;

		int closeindex=0;
		float closedist = 999f;
		currentintersection=targetIntersection;
		int count=0;
		for (int j=0; j<4; j++) {
			if(direction[currentintersection,j]==-1)
				break;
			float angle = Vector3.Angle (directions[direction[currentintersection,j]].position-transform.position,transform.forward);
			if(angle<=90f){
				currentDirection=j;
				findOtherEnd(currentDirection,currentintersection);
			}
		}
	}
	
	public void resetcar(){
		transform.position=startposition;
		transform.rotation=startrotation;
		currentintersection=c_intersection;
		currentDirection=c_direction;
		targetIntersection=t_intersection;
		gameObject.GetComponent<carcontrol>().currentintersection=c_intersection;
		gameObject.GetComponent<carcontrol>().targetIntersection=t_intersection;
		gameObject.GetComponent<carcontrol>().currentDirection=c_direction;
		gameObject.GetComponent<CarController>().Stop();//.Move (-1f,-1f,0f,0f);
		gameObject.GetComponent<CarController> ().m_Rigidbody.velocity=Vector3.zero;
	}
	
	
	int nfmod(int a,int b)
	{
		return (a % b + b) % b;
	}
}
