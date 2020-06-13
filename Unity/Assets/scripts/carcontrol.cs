using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class carcontrol : MonoBehaviour {
	public GameObject intersections1,directions1,currentdirn;
	public Transform[] intersections,directions;
	public int[,] direction,endpoints;
	StreamReader reader;
	public int currentDirection,currentintersection;
	public float hor,vert;
	int intersectionFlag=0;
	int flag=0,flag1=0,goflag=1,slowturn=0;
	public int targetIntersection;
	Rigidbody rigidbody;
	RaycastHit hit;
	GameObject[] go;
	public int viewdirn;
	public int enterIntersection=-1;
	float turnDistance=1.5f;
	int slowTurnIntersections=15;
	[System.Serializable]
	public struct path{
		public int[] intersection,direction;
	};
	public path mpath=new path();
	public bool pathflag=false;
	int pathindex=0;
	float vertprev=0f,horprev=0f;
	GameObject carInFront;
	float totaltimestationary=0f;
	Vector3 positiontensecsago;
	public float pose;
	public int laneFromRight;
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


		/*for(int i=0;i<direction.Length;i++){
			for(int j=0;j<4;j++)
				Debug.Log(direction[i,j]);
		}*/
		/*for (int i=0; i<directions.Length; i++) {
			Debug.Log(i+"--"+directions[i].gameObject.name);
		}*/

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
		//Debug.Log(intersections[minindex].gameObject.name);
		/*int t=targetIntersection;
		targetIntersection=currentintersection;
		currentintersection=t;
        */
		int closeindex=0;
		float closedist = 999f;
		for (int j=0; j<4; j++) {
			//Debug.Log(direction[minindex,j]);
			if(direction[minindex,j]==-1)
				break;
			float angle = Vector3.Angle (directions[direction[minindex,j]].forward,transform.forward);
			//Debug.Log(directions[direction[minindex,j]].gameObject.name+","+angle);

			if(Mathf.Abs(angle)<30f && direction[minindex,j]!=-1 && Vector3.Distance(directions[direction[minindex,j]].position,transform.position)<closedist){
				closeindex=j;
				closedist=Vector3.Distance(directions[direction[minindex,j]].position,transform.position);
			}
		}
		currentDirection = closeindex;
		findOtherEnd (currentDirection,currentintersection);

		//Debug.Log(directions[direction[currentintersection,currentDirection]].gameObject.name+","+currentintersection);

		currentdirn=directions[direction[currentintersection,currentDirection]].gameObject;
		//Debug.Log (currentDirection+","+currentintersection);
		/*
		for(int i=1;i<direction.Length;i++){
			for(int j=0;j<5;j++)
				Debug.Log(direction[i,j]);
		}*/
		/*if(pathflag){
			currentintersection=mpath.intersection[pathindex];
			targetIntersection=mpath.intersection[pathindex+1];
			currentDirection=mpath.direction[pathindex];
		}*/
	}
	
	// Update is called once per frame
	void FixedUpdate () {
		Initialize ();

		//Debug.Log (targetIntersection+","+Vector3.Distance (transform.position, intersections [targetIntersection].position));

		Physics.Raycast (transform.position+transform.up*0.3f,transform.right,out hit,10f);
		laneFromRight=Mathf.RoundToInt(Vector3.Distance(hit.point,transform.position)/1.05f);

		if (Vector3.Distance (transform.position, intersections [targetIntersection].position) < turnDistance && intersectionManagement()&&!anotherCarAtIntersection()) {
			/*if(pathflag){
				pathindex+=1;
				currentintersection=targetIntersection;
				targetIntersection=mpath.intersection[pathindex];
				currentDirection=directions[mpath.direction[pathindex]];
			}*/
			enterIntersection=targetIntersection;
			//Debug.Log("reached "+intersections [currentintersection].gameObject.name);
			int closeindex=0;
			float closedist = 999f;
			currentintersection=targetIntersection;
			int count=0;
			for (int j=0; j<4; j++) {
				if(direction[currentintersection,j]==-1)
					break;
				count++;
			}
			while(true) {
				int newdirn=Mathf.FloorToInt(Random.value*count);
				//Debug.Log("options-"+directions[direction[currentintersection,j]].name);
				float angle = Vector3.Angle (directions[direction[currentintersection,newdirn]].position-transform.position,transform.forward);
				if(Mathf.Abs(angle)<110f){
					closeindex=newdirn;
					closedist=Vector3.Distance(directions[direction[currentintersection,newdirn]].position,transform.position);
					break;
				}
			}
			currentDirection = closeindex;
			findOtherEnd(currentDirection,currentintersection);
			intersectionFlag=1;
			currentdirn=directions[direction[currentintersection,currentDirection]].gameObject;
			setSpeedOfTurn();
			//Debug.Log(directions[direction[currentintersection,currentDirection]].gameObject.name+","+currentintersection);

		}


		hor = 0f;vert = 0f;


		float angle1 = directions[direction[currentintersection,currentDirection]].rotation.eulerAngles.y-transform.rotation.eulerAngles.y;
		if (angle1 <= -180f) {
			angle1+=360f;
		}
		if (angle1 >= 180f) {
			angle1-=360f;
		}
		pose=angle1;

		//Debug.Log (angle1);//+","+directions[direction[currentintersection,currentDirection]].gameObject.name);
		hor += angle1 * 0.24f;//*speedup;

		if(Mathf.Abs(angle1)<10f){
			enterIntersection=-1;
		}

		hor=Mathf.Clamp(hor,-3f,3f);


		//Debug.Log(rigidbody.velocity.magnitude);
		if(rigidbody.velocity.magnitude<3f)
			vert = 0.5f/Mathf.Abs (angle1/10f);
		if (Vector3.Distance (transform.position, intersections [targetIntersection].position) < 6f && rigidbody.velocity.magnitude>0.8f) {
			vert=0.2f;
			//Debug.Log("slwing down");
		}
		float tempdist=Vector3.Distance(intersections[targetIntersection].position,intersections[currentintersection].position);
		if (Vector3.Distance (transform.position, intersections [targetIntersection].position) < 0.3f*tempdist && tempdist>4f) {
			vert=vert*0.6f;
			hor=hor*1.3f;
		}
		//Debug.Log("2-"+vert);

		if(currentintersection>slowTurnIntersections){
			hor*=0.3f;
			vert*=1.7f;
		}

		stayonroad();

		//Debug.Log("3-"+vert);


		//Debug.Log("4-"+vert);
		speedUpAtSlope();
		//float legbrake=0f;
		if(targetIntersection<=slowTurnIntersections&&
		   Vector3.Distance (transform.position, intersections [targetIntersection].position)<turnDistance+3.5f&&
		   !intersectionManagement()&&
		   checkVelocity()){
			if(vert>0)
			vert=0f;
			//legbrake=-0.5f;
			//Debug.Log("moving baackwards!!!!!!!!!!!!!!!!!!!!!!!!!"+gameObject.name);
		}
		if(targetIntersection<=slowTurnIntersections&&anotherCarAtIntersection()&&Vector3.Distance (transform.position, intersections [targetIntersection].position)<5f){
			if(vert>0) vert=0f;
			//legbrake=-0.5f;
			//Debug.Log("another car at intersection "+gameObject.name);
		}
		//else if(angle1<10f)
		stayOnRight();
		if((!intersectionManagement()&&Vector3.Distance (transform.position, intersections [targetIntersection].position)<turnDistance+3.5f)){
			//Debug.Log("waiting at intersection"+gameObject.name);
			if(vert>0) vert=0f;
			//legbrake=-0.5f;
		}
		//if(anotherCarAtIntersection()&&Vector3.Distance (transform.position, intersections [targetIntersection].position)<turnDistance+3.5f)
		//	vert=0f;
		//Debug.Log("5-"+vert);

		//Debug.Log (rigidbody.velocity.magnitude);
		//Debug.Log (hor+","+vert);
		avoidOthers ();
		if(angle1<-10f){
			hor*=0.85f;
			vert*=1.7f;
		}else if(angle1>10f){
			hor*=1.5f;
			vert*=0.6f;
		}
		hor=Mathf.Clamp(hor,-15f,15f);
		vert=Mathf.Clamp(vert,-15f,15f);
		if((vert<0f||vertprev<0f)&&Vector3.Dot(transform.forward,rigidbody.velocity)<1f) vert=0f;
		if(carInFront!=null){
			if(Vector3.Dot(carInFront.transform.position-transform.position,transform.forward)<2f&&
			   Mathf.Abs(Vector3.Dot(carInFront.transform.position-transform.position,transform.right))<0.9f&&
			   vert>0f)vert=0f;
			else carInFront=null;
		}
		/*hor=hor*0.65f+0.35f*horprev;
		vert=vert*0.65f+0.35f*vertprev;
		vertprev=vert;
		horprev=hor;
		*/
		//if(gameObject.name.Equals("Car 1")) vert=-1f;
		totaltimestationary+=Time.deltaTime;
		if(totaltimestationary>10f){
			if(Vector3.Distance(transform.position,positiontensecsago)<=2f){
				vert=-0.01f;
			}
			totaltimestationary=0f;
			positiontensecsago=transform.position;
		}

		gameObject.GetComponent<CarController> ().Move (hor/5f,vert/5f,vert,0f);
		viewdirn = direction [currentintersection, currentDirection];
	}

	bool anotherCarAtIntersection(){
		for(int i=0;i<go.Length;i++){
			if(!go[i].name.Equals(gameObject.name)){
				if(targetIntersection==go[i].GetComponent<carcontrol>().targetIntersection){
					if(Vector3.Distance(go[i].transform.position,intersections[targetIntersection].position)<Vector3.Distance(transform.position,intersections[targetIntersection].position)){
						return true;
					}
				}
			}
		}
		return false;
	}

	void stayOnRight(){
		if (Physics.Raycast (transform.position+transform.up*0.3f, transform.right+transform.up*0.3f, out hit, 5f)
		    &&Vector3.Distance(transform.position,intersections[currentintersection].position)>2f) {
			if(hit.distance>0.7f)
				hor+=0.8f*hit.distance;
			//Debug.Log("moev right");
		}
	}

	void setSpeedOfTurn(){
		float tangle=Vector3.Angle (currentdirn.transform.position-transform.position,transform.forward);
		if(tangle>60f){
			if(Physics.Raycast(transform.position+transform.up*0.5f,transform.right,out hit)){
				if(hit.distance<1f){
					slowturn=0;
				}
				else{
					slowturn=1;
				}
			}
		}
		else if(tangle<-60f){
			if(Physics.Raycast(transform.position+transform.up*0.5f,-transform.right,out hit)){
				if(hit.distance<1f){
					slowturn=0;
				}
				else{
					slowturn=2;
				}
			}
		}
		else{
			slowturn=2;
		}
	}

	void Initialize(){
		//initialization of variables
		if (goflag == 1) {
			GameObject[] go1 = GameObject.FindGameObjectsWithTag ("Player");
			GameObject[] go2 = GameObject.FindGameObjectsWithTag ("Player1");
			go = new GameObject[go1.Length + go2.Length];
			for (int i=0; i<go1.Length; i++) {
				go [i] = go1 [i];
				//Debug.Log(go[i].name);
			}
			for (int i=0; i<go2.Length; i++) {
				go [i + go1.Length] = go2 [i];
			}
			//for(int i=0;i<go.Length;i++){
			//	Debug.Log(go[i].name);
			//}
			goflag=2;
		}
	}

	bool checkVelocity(){
		float t=Vector3.Dot(rigidbody.velocity,transform.forward);
		if(t<=0)
			return false;
		return true;
	}

	void stayonroad(){
		Debug.DrawRay(transform.position+transform.up*0.3f,transform.TransformDirection(Vector3.right) * 0.5f);
		Debug.DrawRay(transform.position+transform.up*0.3f,transform.TransformDirection(-Vector3.right) * 0.5f);
		//Debug.draw
		if (Physics.Raycast (transform.position+transform.up*0.3f, transform.forward, out hit, 2.15f)) {
			//Debug.Log(hit.transform.root.gameObject.name);
			if (!hit.transform.root.gameObject.tag.Equals ("Player"))
				hor*=4.5f;
				//Debug.Log("warning!!!!!!!!!!!!!!");
			Debug.DrawLine(transform.position+transform.up*0.3f,transform.position+transform.forward.normalized*3f);

			//}
		}
		if (Physics.Raycast (transform.position+transform.up*0.3f, transform.right, out hit, 0.5f)) {
			//Debug.Log(hit.transform.root.gameObject.name);
			///if (hit.transform.root.gameObject.name.Equals ("roadboundaries")||hit.transform.root.gameObject.tag.Equals ("Player")) {
				hor-=0.8f/(hit.distance+0.1f);
				//Debug.Log(gameObject.name+",turn-"+hor+","+vert);
				Debug.DrawLine(transform.position,transform.position+transform.right.normalized*2.5f);
			//}
		}
		if (Physics.Raycast (transform.position+transform.up*0.3f, -transform.right, out hit, 0.5f)) {
			//Debug.Log(hit.transform.root.gameObject.name);
			if (hit.transform.root.gameObject.name.Equals ("roadboundaries")) {
				hor+=0.8f/(hit.distance+0.1f);
				//Debug.Log(gameObject.name+",turn-"+hor+","+vert);
				Debug.DrawLine(transform.position,transform.position-transform.right.normalized*2.5f);

			}
		}

	}

	bool intersectionManagement(){
		for(int i=0;i<go.Length;i++){
			if(go[i].GetComponent<carcontrol>().enterIntersection==targetIntersection &&targetIntersection<=slowTurnIntersections&&go[i]!=gameObject){
				return false;
			}

		}
		return true;
	}

	void avoidOthers(){
		flag1 = 0;

		Vector3 sum=new Vector3(0f,0f,0f);
		int count = 0;
		//check if car is close to a road boundary
		//flag1 is 1 if it is close to a road boundary
		if (flag1 == 0 &&enterIntersection==-1) {
			for (int i=0; i<go.Length; i++) {
				if(go[i]!=gameObject){
					float disst = Vector3.Distance (go [i].transform.position, transform.position);
					//if a car is close to another vehicle, move this car in direction away from vehicle
					if (disst < 4f) {
						Vector3 dirn = Vector3.Normalize (go [i].transform.position - transform.position);
						dirn.y = 0.0f;
						Vector3 horVertmovement=transform.InverseTransformVector(dirn);
						if((Mathf.Abs(horVertmovement.x)>1.4f&&Mathf.Abs(transform.rotation.eulerAngles.y-go[i].transform.rotation.eulerAngles.y)>170f)||horVertmovement.z<0f) continue;
						//Debug.Log("other vehicle-"+go[i].name);
						vert-=horVertmovement.z*0.5f*rigidbody.velocity.magnitude*4f/Mathf.Exp(disst);
						hor-=horVertmovement.x*0.2f*rigidbody.velocity.magnitude*4f/Mathf.Exp(disst);
						if(Mathf.Abs(transform.rotation.eulerAngles.y-go[i].transform.rotation.eulerAngles.y)<15f)
							carInFront=go[i];
						if(Vector3.Angle(transform.forward,go[i].transform.forward)>150f/*&&rigidbody.velocity.magnitude>0.5f*/){
							stayOnRight();
						}
					}
					
					//for vehicle flocking (not currently being used)
					if (disst < 3f) {
						sum = sum + go [i].transform.position;
						count++;

					}
				}
			}
		}

		
		//if there is a car moving in opposite direction in front of this car, turn both vehicles
		if (Physics.Raycast (transform.position, transform.forward, out hit, 6f)&&flag1==0) {
			//Debug.Log("detected-"+hit.transform.gameObject.name);
			if( Quaternion.Angle(transform.rotation, hit.transform.rotation)>150f){
				//Debug.Log("moving to side");
				if(!Physics.Raycast (transform.position, transform.right, out hit, 2f)){
					//Debug.Log("left");
					hor-=0.7f;
				}
				if(Physics.Raycast (transform.position, -transform.right, out hit, 2f)){
					//Debug.Log("right");
					hor+=0.7f;
				}
			}
		}

	}

	void speedUpAtSlope(){
		float ang=360f-transform.rotation.eulerAngles.x;
		//Debug.Log(ang+"-----ad");
		if(ang>2f&&ang<90f)
			vert+=ang*0.07f;
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

}
