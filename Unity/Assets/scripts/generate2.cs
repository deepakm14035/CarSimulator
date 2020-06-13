using System.Collections;
using System.Collections.Generic;
using UnityEngine;
public class generate2 : MonoBehaviour {
	public GameObject carmodel,carmodel2;
	public int noOfCars;
	public GameObject track;
	int index=0;
	public GameObject[] cars;
	// Use this for initialization
	void Start () {


		Vector3[] dists=new Vector3[noOfCars];
		for (int i=0; i<noOfCars; i++) {
			Vector3 pos=new Vector3((35*Random.value)-32f,0.47f,(1.7f*Random.value)+9.3f);
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
			//pos.y=2f;
			//if (Physics.CheckSphere(pos, 1f)){
				//i--;
				//continue;
			//}
			//pos.y=0.45f;
			GameObject go;
			int ind=(int)Mathf.Floor(Random.value*4f);
			if(speed>1.3f){
				go=Instantiate(carmodel2,pos,carmodel.transform.rotation);
				go.name="car"+5+""+i;
			}
			else{
				go=Instantiate(cars[ind],pos,carmodel.transform.rotation);
				go.name="car"+(ind+2)+""+i;
			}
			go.GetComponent<movetrack2>().speed=speed;
			go.GetComponent<movetrack2>().track=track;
			//go.GetComponent<movetrack2>().tangents=tangents;

			//if(Random.value>0.5f)
				//go.tag="Player";
			//else{
			go.tag="Player1";
			go.transform.Rotate(transform.eulerAngles+(180*transform.up));

			//}
		}
	}
	// Update is called once per frame
	void Update () {
		//Application.CaptureScreenshot("Screenshot"+index+".png");
		//index++;
	}
}
