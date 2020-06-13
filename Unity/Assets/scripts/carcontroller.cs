using System.Collections;
using System.Collections.Generic;
using UnityEngine;
namespace UnityStandardAssets.Vehicles.Car
{
	[RequireComponent(typeof (CarController))]

	public class carcontroller : MonoBehaviour {

	private CarController m_Car; // the car controller we want to use
	
	
	private void Awake()
	{
		// get the car controller
		m_Car = GetComponent<CarController>();
	}
	
	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		m_Car.Move(0f, 0.1f, 0.1f, 0f);
	}
}
}