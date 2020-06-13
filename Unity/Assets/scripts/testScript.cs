using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class testScript : MonoBehaviour {

	// Use this for initialization
	void Start () {
		GameObject[] trees=GameObject.FindGameObjectsWithTag("tree");
		for(int i=0;i<trees.Length;i++)
			Debug.Log("tree--"+trees[i]);
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
