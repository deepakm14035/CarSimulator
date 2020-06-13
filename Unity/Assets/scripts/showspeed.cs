using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class showspeed : MonoBehaviour {
	public Text speed;
	public Rigidbody car;
	// Use this for initialization
	void Start () {
		car=gameObject.GetComponent<Rigidbody>();
	}
	
	// Update is called once per frame
	void Update () {
		speed.text="Speed\n"+Mathf.RoundToInt(Vector3.Dot(car.velocity, transform.forward)*20f)+"\n"+Mathf.RoundToInt(Vector3.Dot(car.velocity,transform.right)*20f);
	}
}
