using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;
public class obstacleavoid : MonoBehaviour {
	NavMeshAgent agent;
	Vector3 dest;
	// Use this for initialization
	void Start () {
		agent=GetComponent<NavMeshAgent>();
		agent.SetDestination (transform.position+40f*transform.forward);
		dest = transform.position + 40f * transform.forward;
	}
	
	// Update is called once per frame
	void Update () {
		if (Vector3.Distance (transform.position, dest) < 2f) {
			agent.isStopped=true;
			//GetComponent<obstacleavoid>().enabled=false;

		}
	}
}
