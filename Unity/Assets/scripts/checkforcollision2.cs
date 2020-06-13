using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class checkforcollision2 : MonoBehaviour {
	public GameObject parent;
	void OnTriggerEnter(Collider other){
		Debug.Log ("colli!!!");
//		gameObject.GetComponentInParent<moveplayer> ().collided = 1;
	}
	void OnCollisionEnter(Collision other){
		Debug.Log ("colli2!!!");
//		gameObject.GetComponentInParent<moveplayer> ().collided = 1;
		//parent.GetComponent<movetrack2> ().collided = 1;
	}
	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
