using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System.IO;
using SimpleJson;
using ROSBridgeLib.geometry_msgs.SocketIOClient;

namespace SocketIOClient{

	public class fetchData : MonoBehaviour {
		
		/*
		indd
		dissi
		*/
		int goflag=1;    // goflag: variable to check initialization of variables
		public static int sentflag=0;  // sentflag: check whether data sent or not
		int fps=0;  // fps: fps of game
		float time=0f;    // to compute time (for fps)
		int fileno=0;    // to store frame number
		int translateOffset=100;
		
		
		GameObject[] go; // game objects -- list of all the cars
		public GameObject cam;
		public GameObject maincar;
		GameObject[,] boxes;  // boxes of grid -- positions for all grid cells  
		Vector3[,] positions;   // 2D array to store grid cell positions
		Camera camera;
		GameObject prevtarget=null;  // box for the target way-point
		RaycastHit hit;
		
		public GameObject send;
		public Text receiveddata;   // to display received data from python code
		string recdata;   // store received data
		Client client;
		float timeTillLastSend=0f;
		
		/* Variables used for display */
		Text[] nearby;  
		Text[,] matrix;
		string[,] matrix_text;
		public Text template;
		StreamWriter writer,writer2,writer3;  // to write to files
		public Text rewarddisplay,collidedtext;
		StreamWriter gridwriter;
		
		Vector3 target;   // target way-point
		float distancefromright=0.5f;

		int c_intersection,c_direction;
		int index_screenshot=0;
		public string dataSent;

		bool httpConnect=false;
		bool visualise=false;

		int m_size=51;
		bool constantStart=true;
		// Use this for initialization
		void Start () {
			
			camera=cam.GetComponent<Camera>();
			Vector3 sliderposition = new Vector3 (90,(Screen.height)-10,0);
			//Debug.Log(camera.WorldToScreenPoint(transform.position));
			
			
			nearby = new Text[10];
			for (int i=0; i<10; i++) {
				nearby [i] = (Text)Instantiate (template, new Vector3 (90, -50 - (i * 20), 0) + sliderposition, Quaternion.identity);
				nearby[i].transform.SetParent(transform);
				nearby[i].text="";
			}
			
			// initialize grid text,positions and gameObject
			if(visualise)
				matrix=new Text[m_size,m_size];//10,4
			else
				matrix_text = new string[m_size,m_size];;
			positions=new Vector3[m_size,m_size];//10,4
			if(visualise)
				boxes=new GameObject[m_size,m_size];//10,4
			
			// initialize all cells to 0 (stored as string)
			for (int i=0; i<m_size; i++) {//10
				for (int j=0; j<m_size; j++) {//4
					if (visualise) {
						matrix [i,j] = (Text)Instantiate (template, new Vector3 (110 + (i * 25), -50 - (j * 10), 0) + sliderposition, Quaternion.identity);
						matrix[i,j].transform.SetParent(transform);
						matrix [i, j].text = "0";
					}
					else
						matrix_text[i,j]="0";
				}
			}
			
			string path = "test.txt";
			string path2 = "positions.txt";
			string path3 = "occupancygrid.txt";
			writer = new StreamWriter("data.txt", true);
			//Write some text to the test.txt file
			/*writer = new StreamWriter(path, true);
			writer2 = new StreamWriter(path2, true);
			writer3=new StreamWriter(path3, true);
			*/
			
			// connect to machine running Unity
			//client = new Client("ws://192.168.55.245:4567");
			//client.Connect();
			
			//Application.targetFrameRate = 1;
			//writer.WriteLine("Test");
			//writer.Close();
			gridwriter = new StreamWriter ("grid.txt",true);
			
			Debug.Log("start end");
			/*if (visualise) {
				MeshRenderer[] boundaries = GameObject.Find ("carboundaries").GetComponentsInChildren<MeshRenderer> ();
				for (int i = 0; i < boundaries.Length; i++) {
					if (boundaries [i] != null)
						boundaries [i].
				}
			}*/

			//GameObject gobj=GameObject.CreatePrimitive (PrimitiveType.Cube);
			//gobj.transform.position =maincar.transform.position+ maincar.transform.up *0.5f;
			//gobj.transform.localScale = new Vector3 (0.1f, 0.1f, 0.1f);
			//gobj.transform.parent = maincar.transform;
		}

		Vector3 getFirstCellPosition(){
			if (!constantStart) {
				Vector3 pos_firstCell;
				if (Physics.Raycast (maincar.transform.position, -maincar.transform.right, out hit, 10f)) {
					if (hit.distance < 4f) {
						pos_firstCell = hit.point;
					} else {
						pos_firstCell = maincar.transform.position - maincar.transform.right * 2f;
					}
				} else {
					pos_firstCell = maincar.transform.position - maincar.transform.right * 2f;
				}
				//added on 2/12/19
				//pos_firstCell = pos_firstCell - maincar.transform.forward * 3f;
				return pos_firstCell;
			} else {
				//modified on 2/12/19
				return maincar.transform.position - maincar.transform.right * 6f;// - maincar.transform.forward * 3f;
				//return maincar.transform.position - maincar.transform.right * 6f;
			}
		}
		
		// Update is called once per frame
		void FixedUpdate () {
			
			// called once to re-initialize all variables
			if (goflag == 1) {
				// Objects to store all cars in both the directions
				go = GameObject.FindGameObjectsWithTag ("Player");

				
				// set to skip this if statement
				goflag=2;  
				Vector3 pos_firstCell=getFirstCellPosition();
				// sets positions of all the cells in the grid
				for(int j=0;j<m_size;j++){//10
					for(int k=0;k<m_size;k++){//4
						if (visualise) {
							GameObject g = new GameObject ();
							g.transform.position = positions [j, k];
							g.transform.position += (g.transform.up * 5f);
							//g.transform.SetParent(maincar.transform);
							g.transform.localScale = new Vector3 (1f, 0.7f, 0.7f);
							//g.GetComponent<Renderer>().enabled = false;
							boxes [j, k] = g;
						}
						positions[j,k]=pos_firstCell-(maincar.transform.right*(0.5f))+(maincar.transform.forward*j*1.5f)+(maincar.transform.right*k*1f);
						if(visualise)
							matrix[j,k].text="0";
						else
							matrix_text[j,k]="0";
						if(visualise)
							boxes[j,k].transform.position=positions[j,k]+(boxes[j,k].transform.up*5f);
					}
					
				}
				
				//  ------------ for checking target way-point
				setTarget();
				//GameObject goo= GameObject.CreatePrimitive(PrimitiveType.Cube);
				GameObject goo= GameObject.CreatePrimitive(PrimitiveType.Cube);goo.GetComponent<Collider>().enabled=false;
				//goo.transform.position=target;
				//prevtarget=goo;
				prevtarget=goo;
				/*
				StreamWriter w = new StreamWriter ("waypoints.txt",true);
				Transform[] track=track1.GetComponentsInChildren<Transform>();
				w.WriteLine(track.Length+"");
				w.Flush();
				for (int k=1; k<track.Length; k++) {
					w.WriteLine(track[k].position.x+","+track[k].position.z);
					w.Flush();
				}
				track=track2.GetComponentsInChildren<Transform>();
				for (int k=1; k<track.Length; k++) {
					w.WriteLine(track[k].position.x+","+track[k].position.z);
					w.Flush();
				}*/
				
			} // goflag end here (for initialization)
			
			//for (int i=0; i<go.Length; i++) {
			//	camera.WorldToScreenPoint(transform.position);
			//}
			
			/* initialise text boxes for the occupancy grid */
			int index = 0;
			for (int i=0; i<10; i++) {
				nearby[i].text="";
			}
			/* to compute FPS */
			time=time+Time.deltaTime;
			if(time>1f){
				time=0f;
				fps=fileno-fps;
				//Debug.Log(fps);
			}
			
			c_direction=maincar.GetComponent<moveplayer2>().currentDirection;
			c_intersection=maincar.GetComponent<moveplayer2>().currentintersection;

			//finding position of first cell of the grid (to prepare grid)
			
			Vector3 project = getFirstCellPosition();

			//set values for main car and road boundaries
			int mposx=0, mposy=0;
			int closestX=0, closestY=0;
			float minDist = 999f;
			for(int j=0;j<m_size;j++){//10
				for(int k=0;k<m_size;k++){//4
					//positions[j,k]=maincar.transform.position+(maincar.transform.forward*j*1.8f)+(maincar.transform.right*(k-indd)*1.3f);
					positions[j,k]=project+(maincar.transform.right*3f)+(maincar.transform.forward*j*0.15f)+(maincar.transform.right*k*0.1f);
					if(visualise)
						matrix[j,k].text="0";
					else
						matrix_text[j,k]="0";
					//Debug.Log (j+","+k);
					if(visualise)
						boxes[j,k].transform.position=Vector3.Slerp(boxes[j,k].transform.position,positions[j,k]+(boxes[j,k].transform.up*5f),Time.deltaTime*4f);
					float distt1=Vector3.Distance(positions[j,k],maincar.transform.position);
					if(distt1<1.1f){
						if (distt1 < minDist) {
							minDist = distt1;
							closestX = j;
							closestY = k;
						}
						/*if(visualise)//22/9/19
							matrix[j,k].text="-1";
						else
							matrix_text[j,k]="-1";
						*/
						mposx=j;mposy=k;
					}
					float distt4;

					if(Physics.Raycast(positions[j,k]+maincar.transform.up*0.25f,maincar.transform.position-positions[j,k]+maincar.transform.up*0.25f,out hit,Vector3.Distance(positions[j,k],maincar.transform.position))){
						if(visualise)
							matrix[j,k].text="5"+hit.collider.gameObject.name;
						else
							matrix_text[j,k]="5"+hit.collider.gameObject.name;
					}
					
				}
				//nearby[j].name="-";
			}
			if(visualise)//22/9/19
				matrix[closestX,closestY].text="-1";
			else
				matrix_text[closestX,closestY]="-1";

			// Set values for the vehicles that can be viewed
			for (int i=0; i<go.Length; i++) {
				Vector3 pos=camera.WorldToScreenPoint(go[i].transform.position);
				//Debug.Log(go[i].name);
				if(pos.x>0&&pos.y>0&&pos.x<Screen.width&&pos.y<Screen.height&&pos.z>1f&&pos.z<20f){
					string word= pos+"";
					nearby[index].name=word+",car";
					//Application.CaptureScreenshot("Screenshot"+index+".png");
					index++;
					//set values for vehicles that cannot be viewed by main car
					for(int i1=0;i1<m_size;i1++){//10
						for(int j1=0;j1<m_size;j1++){
							if(Vector3.Distance(go[i].transform.position,positions[i1,j1])<0.5f){
								if(Physics.Raycast(go[i].transform.position,maincar.transform.position-go[i].transform.position,out hit, Vector3.Distance(go[i].transform.position,maincar.transform.position))){
									if (maincar.name.Equals (hit.transform.gameObject.name)) {
										if(visualise)
											matrix [i1, j1].text = "1";
										else
											matrix_text [i1, j1] = "1";
									} else {
										if(visualise)
											matrix [i1, j1].text = "2";
										else
											matrix_text[i1,j1]="2";
									}
								}
								else{
									if(visualise)
										matrix[i1,j1].text="1";
									else
										matrix_text[i1,j1]="1";
								}
								if((visualise && matrix[i1,j1].text.Equals("1"))||(!visualise&&matrix_text[i1,j1].Equals("1"))){
									if(visualise)
										matrix[i1,j1].text=go[i].name;
									else
										matrix_text[i1,j1]=go[i].name;
								}
							}
						}
					}
					if (index >= 10)
						break;


					//matrix[minx,miny].text="1";
					//Debug.Log(go[i].name+minx+","+miny);
				}

				//writer2.WriteLine(fileno+","+go[i].name+","+go[i].GetComponent<movetrack2>().speed+","+go[i].transform.position.x+","+go[i].transform.position.z);
				
				
				
				//if(i==go.Length-1)
				//	writer2.Write(go[i].transform.position.x+","+go[i].transform.position.z);
				//else
				//	writer2.Write(go[i].transform.position.x+","+go[i].transform.position.z+",");
				//writer2.Flush();
			}
			
			
			
			/*float minimum=100f;//22/9/19
			int mx=0,my=0;
			for(int j=0;j<m_size;j++){//10
				for(int k=0;k<m_size;k++){
					float distt1=Vector3.Distance(positions[j,k],maincar.transform.position);
					if(distt1<minimum){
						mx=j;my=k;
						minimum=distt1;
					}
				}
			}
			if(visualise)
				matrix[mx,my].text="-1";
			else
				matrix_text[mx,my]="-1";
			*/
			
			// check current followed way-point (-3 will give index from target)
			int indexx = maincar.GetComponent<moveplayer2> ().targetIntersection;

			byte colliddedbyte=9;  // helps to compute reward for collision case, 0: collided, 1: not collided, 4: reached target way-point
			//System.Threading.Thread.Sleep (10);
			if (maincar.GetComponent<moveplayer2> ().collided == 1) {
				maincar.GetComponent<moveplayer2> ().collided =0;
				colliddedbyte=0;  
				
				//send.GetComponent<client2> ().sendData (occu);
				collidedtext.text="collided";
				//Application.LoadLevel(Application.loadedLevel);
				Debug.Log ("collided!!");
				//Application.LoadLevel(Application.loadedLevel);
				
			}
			else if (Vector3.Distance(maincar.transform.position,target)<1.5f&&colliddedbyte!=0) { //&&occu[44]!=0) {
				colliddedbyte=4;  
				float d_s=maincar.GetComponent<moveplayer2>().speed*10f;
				setTarget();
				if(prevtarget!=null)
					Destroy(prevtarget);
				GameObject goo= GameObject.CreatePrimitive(PrimitiveType.Cube);goo.GetComponent<Collider>().enabled=false;
				goo.transform.position=target;
				prevtarget=goo;
				collidedtext.text="reached waypt";
				
			}
			else if(colliddedbyte!=0){
				colliddedbyte=1; // reward is -1 (-1+2 = 1)
				collidedtext.text="nothing happened";
			}
			
			timeTillLastSend += Time.deltaTime;
			
			// send the grid and reward
			if (/*true||*/sentflag == 0||timeTillLastSend>1f||colliddedbyte==4||colliddedbyte==0) {
				byte[] occu = new byte[m_size*m_size+5+(go.Length*2)];//45
				timeTillLastSend=0f;
				
				if(colliddedbyte!=0 && colliddedbyte!=4){
					float pose1=maincar.GetComponent<moveplayer2> ().directions[maincar.GetComponent<moveplayer2> ().direction[c_intersection,c_direction]].rotation.eulerAngles.y-maincar.transform.rotation.eulerAngles.y;
					if (pose1 <= -180f) {
						pose1+=360f;
					}
					if (pose1 >= 180f) {
						pose1-=360f;
					}
					if(Mathf.Abs( pose1)<2f)
						colliddedbyte=3;
					
				}
				else{
					Debug.Log("collidedbyte is 0 or 4");
				}
				
				
				// set reward value at (44)
				//occu[44] = colliddedbyte;
				
				
				gridwriter.Write("r-"+occu[44]+",");
				gridwriter.Flush();
				gridwriter.Write("a-"+recdata+",");
				gridwriter.Flush();


				dataSent=index_screenshot+",";
				// set the grid values at (0:39)
				for (int i=0; i<m_size; i++) {//4
					for (int j=0; j<m_size; j++) {
						if ((visualise && matrix [j, i].text.Equals ("0")) || (!visualise && matrix_text [j, i].Equals ("0"))) {
							occu [j + (i * 10)] = 0;
							dataSent += "0";
						}else if ((visualise && matrix [j, i].text.Equals ("-1")) || (!visualise && matrix_text [j, i].Equals ("-1"))) {
							occu [j + (i * 10)] = 3;
							//dataSent += "3";//deepak
							dataSent += "3";//0
						}else if ((visualise && matrix [j, i].text.Equals ("2")) || (!visualise && matrix_text [j, i].Equals ("2"))) {
							occu [j + (i * 10)] = 2;
							dataSent += "2";
						}else if ((visualise && matrix [j, i].text.Equals ("5")) || (!visualise &&matrix_text [j, i].Equals ("5"))) {
							occu [j + (i * 10)] = 5;
							dataSent+="5";
						} else {
							//occu [j + (i * 10)] = (byte)(int.Parse (matrix [j, i].text [4] + "") + 10 * int.Parse (matrix [j, i].text [3] + ""));
							occu [j + (i * 10)] =(byte)1;
							dataSent+="1";
							//dataSent+=(int.Parse (matrix [j, i].text [4] + "") + 10 * int.Parse (matrix [j, i].text [3] + ""))+",";
						}
						if(j!=m_size-1)
							dataSent+=",";
					}
					dataSent+=".";
				}

				
				sentflag = 1;  // update sentflag
				if (colliddedbyte == 0) {
					Debug.Log ("resetting");
					maincar.GetComponent<moveplayer2> ().resetcar ();
				}
				//Commented for ROS
				//uncomment for compatibility with python script
				/*
				dataSent+=(maincar.transform.position.x+translateOffset)+",";
				dataSent+=(maincar.transform.position.z+translateOffset)+",";
				
				dataSent+=(target.x+translateOffset)+",";
				dataSent+=(target.z+translateOffset)+",";

				dataSent+=getPose()+","+maincar.GetComponent<Rigidbody>().velocity.magnitude+",";
				dataSent+=occu[m_size*m_size+4]+",";//44
				dataSent+=maincar.GetComponent<CarController>().AccelInput+","+maincar.GetComponent<CarController>().CurrentSteerAngle+","+maincar.GetComponent<CarController>().BrakeInput+",";
				
				for(int j=9;j>-10;j--){
					if(Physics.Raycast(maincar.transform.position,(maincar.transform.forward * Mathf.Cos(j*10f) + maincar.transform.right * Mathf.Sin(j*10f)).normalized,out hit,200f)){
						dataSent+=hit.distance+",";
					}else
						dataSent+="200,";
				}

				int ind=0;
				for(int i=m_size*m_size+5;i<m_size*m_size+5+(2*go.Length);i+=2){//45 45
					if(go[ind].name.Equals(maincar.name)){
						ind++;
						continue;
					}
					occu[i]=(byte)(go[ind].transform.position.x+translateOffset);
					occu[i+1]=(byte)(go[ind].transform.position.z+translateOffset);
					dataSent+=(go[ind].transform.position.x+translateOffset)+",";
					dataSent+=(go[ind].transform.position.z+translateOffset)+",";
					float pose2,speed5;

					pose2=getPoseForOtherCars(go[ind]);
					//speed5=go[ind].velocity.magnitude*3.6;
					speed5=go[ind].GetComponent<Rigidbody>().velocity.magnitude;
					//Debug.Log (pose2);
					
					dataSent+=pose2+","+speed5+",";
					ind++;
				}
				*/
				
				/*if(occu[m_size*m_size+4]==0){//44
					//goflag=1;
					Debug.Log ("collided byte-"+colliddedbyte+", sentflag-"+sentflag);
					maincar.GetComponent<moveplayer2>().resetcar();
					sentflag=0;
				}*/
				
				//Debug.Log("occu44-"+occu[44]);
				//send.GetComponent<client2> ().sendData (occu);  // send data 
				
				
				//float pose2=maincar.transform.rotation.eulerAngles.y+90f-maincar.GetComponent<moveplayer>().tangents[maincar.GetComponent<moveplayer>().index].transform.eulerAngles.y;
				//dataSent+=pose2+","+maincar.GetComponent<moveplayer>().speed+",";
				//Debug.Log(dataSent);
				if(httpConnect)
					send.GetComponent<client2> ().sendData (System.Text.Encoding.UTF8.GetBytes(dataSent));
				else
					GetComponent<sendToROS>().sendFlag=true;
				//Debug.Log(dataSent);
				writer.WriteLine(index_screenshot+","+dataSent);
				writer.Flush();
				ScreenCapture.CaptureScreenshot("img"+index_screenshot+".jpg");
				//StartCoroutine(savescreenshot());
				index_screenshot++;

				rewarddisplay.text=occu[m_size*m_size+4]+",   "+(target.x+translateOffset)+", "+(target.z+translateOffset)+"\n"+(maincar.transform.position.x+translateOffset)+", "+(maincar.transform.position.z+translateOffset);//44
				//Time.timeScale=0.5f;
				//System.Threading.Thread.Sleep(5000);
				
			}
			//writer2.WriteLine();
			//writer2.Flush();
			//Application.CaptureScreenshot("img"+fileno+".jpg");
			
			fileno++;  // increament frame number
			if (httpConnect) {
				// receive action values from python code
				recdata = send.GetComponent<client2> ().text;
				if (recdata != null && sentflag == 1) {
					sentflag = 0;  
					receiveddata.text = recdata;
					Debug.Log (recdata + " - received");
					send.GetComponent<client2> ().text = null;
				
					//part2
					maincar.GetComponent<moveplayer2> ().readMovement (recdata);
				}
			} 
			/*else {
				string recievedData = Receiver.action;
				maincar.GetComponent<moveplayer2> ().readMovement (recdata);
				Receiver.action = null;
			}*///code written in sendtoROS.cs

			
		}

		IEnumerator savescreenshot(){
			yield return StartCoroutine(checksaved()) ;
		}

		IEnumerator checksaved(){
			ScreenCapture.CaptureScreenshot("img"+index_screenshot+".jpg");
			yield return new WaitForSeconds(0.01f);
		}

		float getPose(){
			float pose1=Vector3.Angle(maincar.GetComponent<moveplayer2> ().directions[maincar.GetComponent<moveplayer2> ().direction[c_intersection,c_direction]].forward,maincar.transform.forward);
			if (pose1 <= -180f) {
				pose1+=360f;
			}
			if (pose1 >= 180f) {
				pose1-=360f;
			}
			return pose1;
		}

		float getPoseForOtherCars(GameObject car){
			int ointersection=car.GetComponent<carcontrol> ().currentintersection;
			int odirn=car.GetComponent<carcontrol>().currentDirection;
			float pose1=Vector3.Angle(car.GetComponent<carcontrol> ().directions[car.GetComponent<carcontrol> ().direction[ointersection,odirn]].forward,car.transform.forward);
			if (pose1 <= -180f) {
				pose1+=360f;
			}
			if (pose1 >= 180f) {
				pose1-=360f;
			}
			return pose1;
		}

		void setTarget(){
			if(maincar.GetComponent<moveplayer2> ().targetIntersection>maincar.GetComponent<moveplayer2> ().slowTurnIntersections){
				target=maincar.GetComponent<moveplayer2> ().intersections[maincar.GetComponent<moveplayer2> ().targetIntersection].position;
			}
			else
				target=maincar.transform.position+maincar.GetComponent<moveplayer2> ().directions[maincar.GetComponent<moveplayer2> ().direction[c_intersection,c_direction]].transform.forward*2f;

		}
		
		void OnApplicationQuit(){
			writer.Close ();
			//writer2.Close ();
		}
		int nfmod(int a,int b)
		{
			return (a % b + b) % b;
		}

		string getNewObstacleMatrix(){
			return "";
		}
	
	}
}

