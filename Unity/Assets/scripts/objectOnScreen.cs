using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System.IO;
namespace SocketIOClient{
	public class objectOnScreen : MonoBehaviour {
		GameObject[] go;
		int goflag=1;
		public GameObject cam;
		public GameObject maincar;
		GameObject[,] boxes;
		Camera camera;
		Text[] nearby;
		Text[,] matrix;
		public Text template;
		float time=0f;
		StreamWriter writer,writer2,writer3;
		int fileno=0;
		int fps=0,indd,sentflag=0,recflag=1;
		public Text receiveddata;
		float dissi;
		Vector3[,] positions;
		string recdata;
		public GameObject send;
		Client client;
		// Use this for initialization
		void Start () {
			camera=cam.GetComponent<Camera>();
			Vector3 sliderposition = new Vector3 (90,(Screen.height)-10,0);
			//Debug.Log(camera.WorldToScreenPoint(transform.position));
			nearby=new Text[10];
			for (int i=0; i<10; i++) {
				nearby [i] = (Text)Instantiate (template, new Vector3 (90, -50 - (i * 20), 0) + sliderposition, Quaternion.identity);
				nearby[i].transform.SetParent(transform);
				nearby[i].text="";
			}
			
			matrix=new Text[10,4];
			positions=new Vector3[10,4];
			boxes=new GameObject[10,4];
			
			for (int i=0; i<10; i++) {
				for (int j=0; j<4; j++) {
					matrix [i,j] = (Text)Instantiate (template, new Vector3 (90 + (i * 20), -50 - (j * 20), 0) + sliderposition, Quaternion.identity);
					matrix[i,j].transform.SetParent(transform);
					matrix[i,j].text="0";
				}
			}
			
			string path = "test.txt";
			string path2 = "positions.txt";
			string path3 = "occupancygrid.txt";
			
			//Write some text to the test.txt file
			writer = new StreamWriter(path, true);
			writer2 = new StreamWriter(path2, true);
			writer3=new StreamWriter(path3, true);
			
			client = new Client("ws://192.168.55.245:4567");
			
			
			
			client.Connect();
			
			
			//writer.WriteLine("Test");
			//writer.Close();
		}
		
		// Update is called once per frame
		void Update () {
			
			
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
				
				int indexx1 = maincar.GetComponent<movetrack2> ().index;
				Vector3 project1 = Vector3.ProjectOnPlane (maincar.transform.position-maincar.GetComponent<movetrack2> ().tangents [indexx1].transform.position,maincar.GetComponent<movetrack2> ().tangents [indexx1].forward)+maincar.GetComponent<movetrack2> ().tangents [indexx1].transform.position;
				
				
				for(int j=0;j<10;j++){
					for(int k=0;k<4;k++){
						GameObject g=GameObject.CreatePrimitive(PrimitiveType.Cube);
						g.transform.position=positions[j,k];
						g.transform.position+=(g.transform.up*5f);
						g.transform.SetParent(maincar.transform);
						g.transform.localScale=new Vector3(1.5f,1f,1f);
						g.GetComponent<Renderer>().enabled = false;
						boxes[j,k]=g;
						positions[j,k]=project1-(maincar.GetComponent<movetrack2> ().tangents [indexx1].forward*0.6f)+(-maincar.GetComponent<movetrack2> ().tangents [indexx1].right*j*1.8f)-(maincar.GetComponent<movetrack2> ().tangents [indexx1].forward*k*1.3f);
						matrix[j,k].text="0";
						boxes[j,k].transform.position=positions[j,k]+(boxes[j,k].transform.up*5f);
						
					}
					
				}
				
				StreamWriter w = new StreamWriter ("waypoints.txt",true);
				Transform[] track=maincar.GetComponent<movetrack2>().tangents;
				for (int k=0; k<track.Length; k++) {
					w.WriteLine(track[k].position.x+","+track[k].position.z);
					w.Flush();
				}
				
				
			}
			//for (int i=0; i<go.Length; i++) {
			//	camera.WorldToScreenPoint(transform.position);
			//}
			int index = 0;
			for (int i=0; i<10; i++) {
				nearby[i].text="";
			}
			time=time+Time.deltaTime;
			if(time>1f){
				time=0f;
				fps=fileno-fps;
				//Debug.Log(fps);
			}
			//Debug.DrawRay (cam.transform.position,camera.ScreenToWorldPoint(new Vector3(0,0,5)),Color.white);
			//Debug.DrawRay (cam.transform.position,camera.ScreenToWorldPoint(new Vector3(1,1,5)),Color.white);
			int indexx = maincar.GetComponent<movetrack2> ().index;
			float dissi1=Vector3.Distance (maincar.GetComponent<movetrack2> ().tangents [indexx].position, maincar.transform.position);
			//Debug.Log (dissi1);
			Vector3 project = Vector3.ProjectOnPlane (maincar.transform.position-maincar.GetComponent<movetrack2> ().tangents [indexx].transform.position,maincar.GetComponent<movetrack2> ().tangents [indexx].forward)+maincar.GetComponent<movetrack2> ().tangents [indexx].transform.position;
			//Debug.Log (Vector3.Distance (project, maincar.transform.position));
			if (Mathf.Abs (dissi1 - dissi) > 0.6f) {
				indd = (int)(4.95f - dissi);
				dissi = dissi1;
			}
			int mposx=0, mposy=0;
			for(int j=0;j<10;j++){
				for(int k=0;k<4;k++){
					//positions[j,k]=maincar.transform.position+(maincar.transform.forward*j*1.8f)+(maincar.transform.right*(k-indd)*1.3f);
					positions[j,k]=project-(maincar.GetComponent<movetrack2> ().tangents [indexx].forward*0.6f)+(-maincar.GetComponent<movetrack2> ().tangents [indexx].right*j*1.8f)-(maincar.GetComponent<movetrack2> ().tangents [indexx].forward*k*1.3f);
					matrix[j,k].text="0";
					boxes[j,k].transform.position=Vector3.Slerp(boxes[j,k].transform.position,positions[j,k]+(boxes[j,k].transform.up*5f),Time.deltaTime*4f);
					float distt1=Vector3.Distance(positions[j,k],maincar.transform.position);
					if(distt1<1.1f){
						matrix[j,k].text="-1";
						mposx=j;mposy=k;
					}
				}
				nearby[j].name="-";
			}
			
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
					writer.WriteLine(nearby[index].name+","+fileno+",img"+fileno+".jpg");
					writer.Flush();
					//Application.CaptureScreenshot("Screenshot"+index+".png");
					index++;
					//Vector3 dirn=pos-maincar.transform.position;
					//int minx=0,miny=0;
					//float mindist=999f;
					
					for(int j=0;j<10;j++){
						for(int k=0;k<4;k++){
							float distt1=Vector3.Distance(positions[j,k],go[i].transform.position);
							if(distt1<1.1f){//mindist){
								//mindist=distt1;
								//minx=j;
								//miny=k;
								matrix[j,k].text="1";
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
					
					//matrix[minx,miny].text="1";
					//Debug.Log(go[i].name+minx+","+miny);
				}
				//writer2.WriteLine(fileno+","+go[i].name+","+go[i].GetComponent<movetrack2>().speed+","+go[i].transform.position.x+","+go[i].transform.position.z);
				if(i==go.Length-1)
					writer2.Write(go[i].transform.position.x+","+go[i].transform.position.z);
				else
					writer2.Write(go[i].transform.position.x+","+go[i].transform.position.z+",");
				writer2.Flush();
			}
			//if (sentflag == 0&&recflag==1) {
			byte[] occu = new byte[40];
			for (int i=0; i<4; i++) {
				for (int j=0; j<10; j++) {
					if (matrix [j, i].text.Equals ("0")) {
						occu [j + (i * 10)] = 0;
					} 
					else if (matrix [j, i].text.Equals ("-1")) {
						occu [j + (i * 10)] = 3;
					} else if (matrix [j, i].text.Equals ("2")) {
						occu [j + (i * 10)] = 2;
					}
					else{
						occu [j + (i * 10)] = (byte)(int.Parse(matrix[j,i].text[4]+"")+10*int.Parse(matrix[j,i].text[3]+""));
					} 
					
				}
			}
			recflag=0;
			sentflag=1;
			//Debug.Log ("sending data");
			if (maincar.GetComponent<moveplayer> ().collided == 1) {
				send.GetComponent<client2> ().sendData (System.Text.Encoding.UTF8.GetBytes("collided"));
			}
			else
				send.GetComponent<client2> ().sendData (occu);
			//}
			
			writer2.WriteLine();
			writer2.Flush();
			ScreenCapture.CaptureScreenshot("img"+fileno+".jpg");
			fileno++;
			//if (sentflag == 1 && recflag == 0) {
			recdata=send.GetComponent<client2> ().text;
			if(recdata!=null){
				sentflag=0;
				recflag=1;
				receiveddata.text=recdata;
				Debug.Log (recdata+" - received");
				send.GetComponent<client2> ().text=null;


				//part2
				maincar.GetComponent<moveplayer>().readMovement(recdata);
			}
			
			//}
			for(int k=0;k<4;k++){
				for(int j=0;j<10;j++){
					writer3.Write(matrix[j,k].text+" ");
					writer3.Flush();
				}
				writer3.WriteLine();
				writer3.Flush();
			}
			
		}
		void OnApplicationQuit(){
			//writer.Close ();
			//writer2.Close ();
		}
	}
}