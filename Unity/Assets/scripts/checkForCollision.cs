using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class checkForCollision : MonoBehaviour {
	public GameObject parent;
	void OnTriggerEnter(Collider other){
		parent.GetComponent<moveplayer2> ().collided = 1;
	}
	void OnCollisionEnter(Collision other){
		parent.GetComponent<moveplayer2> ().collided = 1;
	}

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
