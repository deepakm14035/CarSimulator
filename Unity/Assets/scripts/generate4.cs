using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
public class generate4 : MonoBehaviour {
	public GameObject carmodel,carmodel2;
	public int noOfCars;
	public GameObject track;
	int index=0;
	public GameObject[] cars;
	// Use this for initialization
	void Start () {
		
		StreamWriter sw = new StreamWriter ("velocities.txt");
		Vector3[] dists=new Vector3[noOfCars];
		for (int i=0; i<noOfCars; i++) {
			Vector3 pos=new Vector3((35*Random.value)-32f,0.47f,(3.7f*Random.value)+8.3f);
			float speed=(Random.value)+0.4f;

			int flag=0;
			for(int j=0;j<i;j++){
				if(Vector3.Distance(pos,dists[j])<3f){
					i--;
					flag=1;
					break;
				}
			}
			if(flag==1)
				continue;
			GameObject go;
			int ind=(int)Mathf.Floor(Random.value*4f);
			if(speed>1.3f){
				go=Instantiate(carmodel2,pos,carmodel.transform.rotation);
				go.name="car"+5+""+i;
			}
			else{
				go=Instantiate(cars[ind],pos,carmodel.transform.rotation);
				go.name="car"+(ind+1)+""+i;
			}
			sw.WriteLine(go.name+","+speed);
			sw.Flush();
			go.GetComponent<movetrack2>().speed=speed;
			go.GetComponent<movetrack2>().track=track;
			//go.GetComponent<movetrack2>().tangents=tangents;

			go.tag="Player1";
			go.transform.Rotate(transform.eulerAngles+(180*transform.up));
			//}
		}
		sw.Close ();


	}
	// Update is called once per frame
	void Update () {
		//Application.CaptureScreenshot("Screenshot"+index+".png");
		//index++;
	}
}
