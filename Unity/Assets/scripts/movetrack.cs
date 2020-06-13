using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class movetrack : MonoBehaviour {
	public GameObject go;
	// Use this for initialization
	void Start () {
		for (int i = 0; i < 5; i++) {
			float val = 1 + (Random.value * 3);
			float val2 = (Random.value * 15) - 7;

			Vector3 pos = new Vector3 (val2, 0, val);
			Instantiate (go, pos,Quaternion.identity);
		}
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
