using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System.IO;
namespace SocketIOClient{
	public class objectonscreen3 : MonoBehaviour {
		
		/*
		indd
		dissi
		*/
		int goflag=1;    // goflag: variable to check initialization of variables
		int sentflag=0;  // sentflag: check whether data sent or not
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
		public Text template;
		StreamWriter writer,writer2,writer3;  // to write to files
		public Text rewarddisplay,collidedtext;
		StreamWriter gridwriter;
		
		Transform[] tangents3,tangents2;   // list of all way-points
		Vector3[] tangents1;
		public GameObject track1,track2;
		Vector3 target;   // target way-point
		int targetindex;  // index of the target way-point
		int currentindex; // index of the current way-point
		int indexoffset=1;
		float distancefromright=0.5f;


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
			matrix=new Text[10,4];
			positions=new Vector3[10,4];
			boxes=new GameObject[10,4];
			
			// initialize all cells to 0 (stored as string)
			for (int i=0; i<10; i++) {
				for (int j=0; j<4; j++) {
					matrix [i,j] = (Text)Instantiate (template, new Vector3 (110 + (i * 45), -50 - (j * 20), 0) + sliderposition, Quaternion.identity);
					matrix[i,j].transform.SetParent(transform);
					matrix[i,j].text="0";
				}
			}
			
			string path = "test.txt";
			string path2 = "positions.txt";
			string path3 = "occupancygrid.txt";
			
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
			
			Transform[] tangents21 = track1.GetComponentsInChildren<Transform> ();
			Transform[] tangents31 = track2.GetComponentsInChildren<Transform> ();
			tangents2=new Transform[tangents21.Length-1];
			tangents3=new Transform[tangents31.Length-1];
			
			for (int i=1; i<tangents21.Length; i++) {
				tangents2[i-1]=tangents21[i];
			}
			for (int i=1; i<tangents31.Length; i++) {
				tangents3[i-1]=tangents31[i];
			}
		}
		
		// Update is called once per frame
		void FixedUpdate () {
			
			// called once to re-initialize all variables
			if (goflag == 1) {  
				// Objects to store all cars in both the directions
				GameObject[] go1 = GameObject.FindGameObjectsWithTag ("Player");
				GameObject[] go2 = GameObject.FindGameObjectsWithTag ("Player1");
				// merge all into 'go' object
				go = new GameObject[go1.Length + go2.Length];
				for (int i=0; i<go1.Length; i++) {
					go [i] = go1 [i];
				}
				for (int i=0; i<go2.Length; i++) {
					go [i + go1.Length] = go2 [i];
				}
				
				// set to skip this if statement
				goflag=2;  
				
				// get all way-points
				tangents1 = new Vector3[(tangents2.Length+tangents3.Length)*2];
				for(int i=0;i<tangents2.Length;i++){
					tangents1[i]=tangents2[i].position;
				}
				Vector3 midpoint;
				for(int i=tangents2.Length;i<tangents2.Length*2;i++){
					if(i<tangents2.Length*2-1)
						midpoint=(tangents2[i-tangents2.Length].position+tangents2[i-tangents2.Length+1].position)/2;
					else
						midpoint=(tangents2[i-tangents2.Length].position+tangents2[0].position)/2;
					tangents1[i]=midpoint;
					//GameObject go=GameObject.CreatePrimitive(PrimitiveType.Cube);
					//go.transform.position=midpoint;
				}
				
				for(int i=0;i<tangents3.Length;i++){
					tangents1[tangents2.Length+i]=tangents3[i].position;
				}
				for(int i=tangents3.Length;i<tangents3.Length*2;i++){
					if(i<tangents3.Length*2-1)
						midpoint=(tangents3[i-tangents3.Length].position+tangents3[i-tangents3.Length+1].position)/2;
					else
						midpoint=(tangents3[i-tangents3.Length].position+tangents3[0].position)/2;
					tangents1[tangents2.Length*2+i]=midpoint;
					//GameObject go=GameObject.CreatePrimitive(PrimitiveType.Cube);
					//go.transform.position=midpoint;
					
				}
				//int indexx1 = maincar.GetComponent<moveplayer> ().index; 
				currentindex = maincar.GetComponent<moveplayer> ().index; 
				Vector3 pos_firstCell = Vector3.ProjectOnPlane (maincar.transform.position-maincar.GetComponent<moveplayer> ().tangents [currentindex].transform.position,maincar.GetComponent<moveplayer> ().tangents [currentindex].forward)+maincar.GetComponent<moveplayer> ().tangents [currentindex].transform.position;
				
				// sets positions of all the cells in the grid
				for(int j=0;j<10;j++){
					for(int k=0;k<4;k++){
						GameObject g=GameObject.CreatePrimitive(PrimitiveType.Cube);
						g.transform.position=positions[j,k];
						g.transform.position+=(g.transform.up*5f);
						g.transform.SetParent(maincar.transform);
						g.transform.localScale=new Vector3(1.5f,1f,1f);
						g.GetComponent<Renderer>().enabled = false;
						boxes[j,k]=g;
						positions[j,k]=pos_firstCell-(maincar.GetComponent<moveplayer> ().tangents [currentindex].forward*0.6f)+(maincar.transform.forward*j*1.8f)-(maincar.transform.right*k*1.3f);
						matrix[j,k].text="0";
						boxes[j,k].transform.position=positions[j,k]+(boxes[j,k].transform.up*5f);
					}
					
				}
				
				//  ------------ for checking target way-point
				int inddex = nfmod(maincar.GetComponent<moveplayer>().index-indexoffset,maincar.GetComponent<moveplayer> ().tangents.Length);
				Debug.Log ("inddex-"+inddex);
				//target=maincar.GetComponent<moveplayer> ().tangents[inddex].position-3*maincar.GetComponent<moveplayer> ().tangents[inddex].forward;
				target=maincar.GetComponent<moveplayer> ().tangents[inddex].position-distancefromright*maincar.GetComponent<moveplayer> ().tangents[inddex].forward;
				targetindex=inddex;
				//GameObject goo= GameObject.CreatePrimitive(PrimitiveType.Cube);
				GameObject goo= GameObject.CreatePrimitive(PrimitiveType.Cube);goo.GetComponent<Collider>().enabled=false;
				//goo.transform.position=target;
				//prevtarget=goo;
				prevtarget=goo;
				
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
				}
				
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
			
			//Debug.DrawRay (cam.transform.position,camera.ScreenToWorldPoint(new Vector3(0,0,5)),Color.white);
			//Debug.DrawRay (cam.transform.position,camera.ScreenToWorldPoint(new Vector3(1,1,5)),Color.white);
			
			//finding position of first cell of the grid (to prepare grid)
			
			int indexx = maincar.GetComponent<moveplayer> ().index;
			//float dissi1 = Vector3.Distance (maincar.GetComponent<moveplayer> ().tangents [indexx].position, maincar.transform.position);
			//Debug.Log (dissi1);
			Vector3 project = Vector3.ProjectOnPlane (maincar.transform.position-maincar.GetComponent<moveplayer> ().tangents [indexx].transform.position,maincar.GetComponent<moveplayer> ().tangents [indexx].forward)+maincar.GetComponent<moveplayer> ().tangents [indexx].transform.position;
			//Debug.Log (Vector3.Distance (project, maincar.transform.position));
			/*
			if (Mathf.Abs (dissi1 - dissi) > 0.6f) {
				indd = (int)(4.95f - dissi);
				dissi = dissi1;
			}*/
			
			//set values for main car and road boundaries
			int mposx=0, mposy=0;
			for(int j=0;j<10;j++){
				for(int k=0;k<4;k++){
					//positions[j,k]=maincar.transform.position+(maincar.transform.forward*j*1.8f)+(maincar.transform.right*(k-indd)*1.3f);
					positions[j,k]=project-(maincar.GetComponent<moveplayer> ().tangents [indexx].forward*0.6f)+(maincar.transform.forward*j*1.8f)-(maincar.transform.right*k*1.3f);
					matrix[j,k].text="0";
					boxes[j,k].transform.position=Vector3.Slerp(boxes[j,k].transform.position,positions[j,k]+(boxes[j,k].transform.up*5f),Time.deltaTime*4f);
					float distt1=Vector3.Distance(positions[j,k],maincar.transform.position);
					if(distt1<1.1f){
						matrix[j,k].text="-1";
						mposx=j;mposy=k;
					}
					float distt4;
					for(int l=0;l<tangents1.Length;l++){
						distt4=Vector3.Distance(positions[j,k],tangents1[l]);
						if(distt4<1.1f){
							matrix[j,k].text="5";
						}
					}
					
				}
				nearby[j].name="-";
			}
			
			// Set values for the vehicles that can be viewed
			for (int i=0; i<go.Length; i++) {
				Vector3 pos=camera.WorldToScreenPoint(go[i].transform.position);
				//Debug.Log(go[i].name);
				if(pos.x>0&&pos.y>0&&pos.x<Screen.width&&pos.y<Screen.height&&pos.z>1f&&pos.z<20f){
					string word= pos+"";
					if(go[i].name.Contains("car1"))
						nearby[index].name=word+",red";
					else if(go[i].name.Contains("car2"))
						nearby[index].name=word+",yellow";
					else if(go[i].name.Contains("car3"))
						nearby[index].name=word+",white";
					else if(go[i].name.Contains("car4"))
						nearby[index].name=word+",black";
					else if(go[i].name.Contains("car5"))
						nearby[index].name=word+",blue";
					//time=time+Time.deltaTime;
					//Debug.Log(nearby[index].name+","+time);
					
					
					//writer.WriteLine(nearby[index].name+","+fileno+",img"+fileno+".jpg");
					//writer.Flush();
					
					
					//Application.CaptureScreenshot("Screenshot"+index+".png");
					index++;
					//Vector3 dirn=pos-maincar.transform.position;
					//int minx=0,miny=0;
					//float mindist=999f;
					
					//set values for vehicles that cannot be viewed by main car
					
					for(int j=0;j<10;j++){
						for(int k=0;k<4;k++){
							float distt1=Vector3.Distance(positions[j,k],go[i].transform.position);
							if(distt1<1.1f){//mindist){
								//mindist=distt1;
								//minx=j;
								//miny=k;
								matrix[j,k].text="1";
								if(mposx-j==0) continue;
								Vector2 ddirn=new Vector2(1f,(mposy-k)/(mposx-j));
								for(int l=mposx;l<j;l++){
									Vector2 positn=new Vector2(mposx,mposy);
									positn=positn+(ddirn*(l-mposx+1f));
									if(positn.y<0f ||positn.y>3f)
										break;
									float vall=positn.y-Mathf.Floor(positn.y);
									if(vall>0.35f&&vall<0.65f){
										if(matrix[l,Mathf.FloorToInt(positn.y)].text.Equals("1")
										   &&matrix[l,Mathf.FloorToInt(positn.y)+1].text.Equals("1")){
											matrix[j,k].text="2";
											//Debug.Log (j+","+k+" blocked by "+l+","+Mathf.FloorToInt(positn.y));
											break;
										}
										
									}
									else{
										if(matrix[l,Mathf.RoundToInt(positn.y)].text.Equals("1")){
											matrix[j,k].text="2";
											//Debug.Log (j+","+k+" blocked2 by "+l+","+Mathf.RoundToInt(positn.y));
											break;
										}
									}
								}
							}
						}
					}
					
					
					for(int j=0;j<10;j++){
						for(int k=0;k<4;k++){
							float distt1=Vector3.Distance(positions[j,k],go[i].transform.position);
							if(distt1<1.1f&&matrix[j,k].text.Equals("1")){
								matrix[j,k].text=go[i].name;
							}
						}
					}
					for(int j=0;j<10;j++){
						for(int k=0;k<4;k++){
							if(matrix[j,k].text.Equals("5")){
								for(int l=j;l<10;l++) matrix[l,k].text="5";
							}
						}
					}
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
			
			
			
			float minimum=100f;
			int mx=0,my=0;
			for(int j=0;j<10;j++){
				for(int k=0;k<4;k++){
					float distt1=Vector3.Distance(positions[j,k],maincar.transform.position);
					if(distt1<minimum){
						mx=j;my=k;
						minimum=distt1;
					}
				}
			}
			matrix[mx,my].text="-1";
			
			
			// check current followed way-point (-3 will give index from target)
			indexx = nfmod (maincar.GetComponent<moveplayer> ().index-indexoffset,maincar.GetComponent<moveplayer> ().tangents.Length);
			
			byte colliddedbyte=9;  // helps to compute reward for collision case, 0: collided, 1: not collided, 4: reached target way-point
			//System.Threading.Thread.Sleep (10);
			if (maincar.GetComponent<moveplayer> ().collided == 1) {
				maincar.GetComponent<moveplayer> ().collided =0;
				colliddedbyte=0;  
				
				//send.GetComponent<client2> ().sendData (occu);
				collidedtext.text="collided";
				//Application.LoadLevel(Application.loadedLevel);
				Debug.Log ("collided!!");
				//Application.LoadLevel(Application.loadedLevel);
				
			}
			else if (Mathf.Abs(targetindex-indexx)>=indexoffset) { //&&occu[44]!=0) {
				colliddedbyte=4;  
				float d_s=maincar.GetComponent<moveplayer>().speed*10f;
				//target=maincar.GetComponent<moveplayer> ().tangents[nfmod(indexx,maincar.GetComponent<moveplayer> ().tangents.Length)].position-(3*maincar.GetComponent<moveplayer> ().tangents[nfmod(indexx,maincar.GetComponent<moveplayer> ().tangents.Length)].forward);
				target=maincar.GetComponent<moveplayer> ().tangents[nfmod(indexx,maincar.GetComponent<moveplayer> ().tangents.Length)].position-(distancefromright*maincar.GetComponent<moveplayer> ().tangents[nfmod(indexx,maincar.GetComponent<moveplayer> ().tangents.Length)].forward);
				//Debug.Log ("target reached!!!,  "+target);
				targetindex=indexx%maincar.GetComponent<moveplayer> ().tangents.Length;
				
				if(prevtarget!=null)
					Destroy(prevtarget);
				GameObject goo= GameObject.CreatePrimitive(PrimitiveType.Cube);goo.GetComponent<Collider>().enabled=false;
				goo.transform.position=target;
				prevtarget=goo;
				collidedtext.text="reached waypt";
				
			}
			else {
				colliddedbyte=1; // reward is -1 (-1+2 = 1)
				collidedtext.text="nothing happened";
			}
			
			timeTillLastSend += Time.deltaTime;
			
			// send the grid and reward
			if (sentflag == 0||timeTillLastSend>3f||colliddedbyte==4||colliddedbyte==0) {
				byte[] occu = new byte[45+(go.Length*2)];
				timeTillLastSend=0f;
				
				if(colliddedbyte!=0 && colliddedbyte!=4){
					Vector3 pose1=maincar.GetComponent<moveplayer>().tangents[maincar.GetComponent<moveplayer>().index].rotation.eulerAngles-maincar.transform.rotation.eulerAngles;
					float pose=pose1.y+270f;
					//Debug.Log (pose);
					if(Mathf.Abs( pose)<2f)
						colliddedbyte=3;
					
				}
				else{
					Debug.Log("collidedbyte is 0 or 4");
				}
				
				
				// set reward value at (44)
				occu[44] = colliddedbyte;
				
				
				gridwriter.Write("r-"+occu[44]+",");
				gridwriter.Flush();
				gridwriter.Write("a-"+recdata+",");
				gridwriter.Flush();
				
				string dataSent="";
				// set the grid values at (0:39)
				for (int i=0; i<4; i++) {
					for (int j=0; j<10; j++) {
						if (matrix [j, i].text.Equals ("0")) {
							occu [j + (i * 10)] = 0;
							dataSent+="0,";
						} else if (matrix [j, i].text.Equals ("-1")) {
							occu [j + (i * 10)] = 3;
							dataSent+="3,";
						} else if (matrix [j, i].text.Equals ("2")) {
							occu [j + (i * 10)] = 2;
							dataSent+="2,";
						} else if (matrix [j, i].text.Equals ("5")) {
							occu [j + (i * 10)] = 5;
							dataSent+="5,";
						} else {
							occu [j + (i * 10)] = (byte)(int.Parse (matrix [j, i].text [4] + "") + 10 * int.Parse (matrix [j, i].text [3] + ""));
							dataSent+=(int.Parse (matrix [j, i].text [4] + "") + 10 * int.Parse (matrix [j, i].text [3] + ""))+",";
						} 
						gridwriter.Write(occu [j + (i * 10)]+",");
						gridwriter.Flush();
					}
				}
				
				gridwriter.WriteLine();
				gridwriter.Flush();
				
				
				sentflag = 1;  // update sentflag
				
				// set the current position at (40,41)	
				//occu[40]=System.Convert.ToByte(Mathf.RoundToInt( maincar.transform.position.x)+translateOffset);//position values are negativve, and hence translated
				//occu[41]=System.Convert.ToByte(Mathf.RoundToInt(maincar.transform.position.z)+translateOffset);
				dataSent+=(maincar.transform.position.x+translateOffset)+",";
				dataSent+=(maincar.transform.position.z+translateOffset)+",";
				
				// set the current position at (42,43)	
				//occu[42]=System.Convert.ToByte(Mathf.RoundToInt(target.x)+translateOffset);//position values are negativve, and hence translated
				//occu[43]=System.Convert.ToByte(Mathf.RoundToInt(target.z)+translateOffset);
				dataSent+=(target.x+translateOffset)+",";
				dataSent+=(target.z+translateOffset)+",";
				dataSent+=occu[44]+",";
				
				for(int j=9;j>0;j++){
					if(Physics.Raycast(maincar.transform.position,Quaternion.AngleAxis(j*10f,maincar.transform.up).eulerAngles,out hit,200f)){
						dataSent+=hit.distance+",";
					}else
						dataSent+="200,";
				}
				if(Physics.Raycast(maincar.transform.position,Quaternion.AngleAxis(0f,maincar.transform.up).eulerAngles,out hit,200f)){
					dataSent+=hit.distance+",";
					Debug.Log("front-"+hit.distance);
				}else{
					dataSent+="200,";
					Debug.Log("front-"+hit.distance);
				}
				for(int j=1;j<10;j++){
					if(Physics.Raycast(maincar.transform.position,Quaternion.AngleAxis(j*-10f,maincar.transform.up).eulerAngles,out hit,200f)){
						dataSent+=hit.distance+",";
					}else
						dataSent+="200,";
				}
				
				int ind=0;
				for(int i=45;i<45+(2*go.Length);i+=2){
					occu[i]=(byte)(go[ind].transform.position.x+translateOffset);
					occu[i+1]=(byte)(go[ind].transform.position.z+translateOffset);
					dataSent+=(go[ind].transform.position.x+translateOffset)+",";
					dataSent+=(go[ind].transform.position.z+translateOffset)+",";
					float pose2,speed5;
					if(go[ind].GetComponent("moveplayer") as moveplayer!=null){
						pose2=-90f-go[ind].transform.rotation.eulerAngles.y+go[ind].GetComponent<moveplayer>().tangents[go[ind].GetComponent<moveplayer>().index].transform.eulerAngles.y;
						//speed5=1f;
						//speed5=go[ind].velocity.magnitude*3.6;
						speed5=go[ind].GetComponent<moveplayer>().speed;
						if(pose2<-300f) pose2+=360f;
						Debug.Log (pose2);
					}
					else{
						pose2=-90f-go[ind].transform.rotation.eulerAngles.y+go[ind].GetComponent<movetrack2>().tangents[go[ind].GetComponent<movetrack2>().index].transform.eulerAngles.y;
						speed5=go[ind].GetComponent<movetrack2>().speed;
						if(pose2<-300f) pose2+=360f;
					}
					dataSent+=pose2+","+speed5+",";
					ind++;
				}
				
				
				if(occu[44]==0){
					//goflag=1;
					Debug.Log ("collided byte-"+colliddedbyte+", sentflag-"+sentflag);
					maincar.GetComponent<moveplayer>().resetcar();
					targetindex=nfmod (maincar.GetComponent<moveplayer>().index-indexoffset,maincar.GetComponent<moveplayer>().tangents.Length);
					sentflag=0;
				}
				
				//Debug.Log("occu44-"+occu[44]);
				//send.GetComponent<client2> ().sendData (occu);  // send data 
				
				
				//float pose2=maincar.transform.rotation.eulerAngles.y+90f-maincar.GetComponent<moveplayer>().tangents[maincar.GetComponent<moveplayer>().index].transform.eulerAngles.y;
				//dataSent+=pose2+","+maincar.GetComponent<moveplayer>().speed+",";
				
				
				send.GetComponent<client2> ().sendData (System.Text.Encoding.UTF8.GetBytes(dataSent));
				Debug.Log(dataSent);
				rewarddisplay.text=occu[44]+",   "+(target.x+translateOffset)+", "+(target.z+translateOffset)+"\n"+(maincar.transform.position.x+translateOffset)+", "+(maincar.transform.position.z+translateOffset);
				
				//Time.timeScale=0.5f;
				//System.Threading.Thread.Sleep(5000);
				
			}
			
			//writer2.WriteLine();
			//writer2.Flush();
			//Application.CaptureScreenshot("img"+fileno+".jpg");
			
			fileno++;  // increament frame number
			
			// receive action values from python code
			recdata=send.GetComponent<client2> ().text;
			if(recdata!=null&&sentflag==1){
				sentflag=0;  
				receiveddata.text=recdata;
				Debug.Log (recdata+" - received");
				send.GetComponent<client2> ().text=null;
				
				//part2
				maincar.GetComponent<moveplayer>().readMovement(recdata);
			}
			
			//}
			/*for(int k=0;k<4;k++){
				for(int j=0;j<10;j++){
					writer3.Write(matrix[j,k].text+" ");
					writer3.Flush();
				}
				writer3.WriteLine();
				writer3.Flush();
			}*/
			
		}
		
		void OnApplicationQuit(){
			//writer.Close ();
			//writer2.Close ();
		}
		int nfmod(int a,int b)
		{
			return (a % b + b) % b;
		}
	}
}