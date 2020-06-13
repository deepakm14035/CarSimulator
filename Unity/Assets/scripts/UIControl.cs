using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UIControl : MonoBehaviour {
	Text[] nearby;
	GameObject[] near;
	public GameObject car;
	public Text template;

	Vector3 sliderposition = new Vector3 (60,(Screen.height)-10,0);
	// Use this for initialization
	void Start () {
		nearby=new Text[10];
		for (int i=0; i<10; i++) {
			nearby [i] = (Text)Instantiate (template, new Vector3 (-10, -50 - (i * 20), 0) + sliderposition, Quaternion.identity);
			nearby[i].transform.SetParent(transform);
			nearby[i].text="";
		}
	}
	
	// Update is called once per frame
	void Update () {
		int index = 0;
		for (int i=0; i<10; i++) {
			nearby[i].text="";
		}
		near = GameObject.FindGameObjectsWithTag ("obstacle");
		for (int i=0; i<near.Length; i++) {
			if(Vector3.Distance(near[i].transform.position,car.transform.position)<7){
				nearby[index].text=near[i].name+",position-"+near[i].transform.position;
				index++;
			}

		}
	}
}
