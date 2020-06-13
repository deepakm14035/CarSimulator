using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class moveperson : MonoBehaviour {
	Animator anim;
	float time=0f;
	// Use this for initialization
	void Start () {
		anim=GetComponent<Animator>();
		anim.SetFloat ("walk",0.5f);
	}
	
	// Update is called once per frame
	void Update () {
		time += Time.deltaTime;
		if (time > 15f) {
			transform.Rotate(Vector3.up*180f);
			time=0f;
		}
	}
}
