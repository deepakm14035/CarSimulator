using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
public class generate5 : MonoBehaviour {
	public int noOfCars;
	int index=0;
	public GameObject cars;
	public GameObject intersections1,directions1;
	public float length,width;
	// Use this for initialization
	void Start () {
		Vector3[] dists=new Vector3[noOfCars];
		for (int i=0; i<noOfCars; i++) {
			Vector3 pos=new Vector3((width*Random.value),0.47f,(length*Random.value));
			pos+=transform.position;
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

			go=Instantiate(cars,pos,Quaternion.AngleAxis(-90f,transform.up));
			go.GetComponent<carcontrol>().intersections1=intersections1;
			go.GetComponent<carcontrol>().directions1=directions1;

			go.tag="Player1";
			//go.transform.Rotate(transform.eulerAngles+(180*transform.up));
			//}
		}

	}
	// Update is called once per frame
	void Update () {
		//Application.CaptureScreenshot("Screenshot"+index+".png");
		//index++;
	}
}
