using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.AI;
public class myWander3: MonoBehaviour {
	int start=0;
	RaycastHit hit;
	GameObject target;
	public int move=0;
	Animator anim;
	Vector3 movement;
	float time=0;
	public float speed=0f;
	NavMeshAgent agent;
	void Start () {
		move = 0;
		agent=gameObject.GetComponent<NavMeshAgent>();
		agent.speed=speed;
		agent.enabled=true;
		anim=gameObject.GetComponent<Animator>();
	}
	
	// Update is called once per frame
	void FixedUpdate () {
		//Debug.DrawRay(transform.position, transform.forward);
		if (move == 0) {
			Vector3 randomDirection = Random.insideUnitSphere * 2f;
			randomDirection.y=transform.position.y;
			randomDirection += transform.position;
			NavMeshHit hit;
			NavMesh.SamplePosition(randomDirection, out hit, 2f, 1);
			float angle=Random.value*60f-30f;
			movement=transform.position+Quaternion.AngleAxis(angle,transform.up)*transform.forward*2.5f;
			//NavMeshHit hit;
			NavMesh.SamplePosition(movement, out hit, 4f, 1);

			movement = hit.position;
			agent.enabled=true;
			agent.SetDestination(movement);
			//GameObject go=GameObject.CreatePrimitive(PrimitiveType.Cube);
			//go.transform.position=movement;
			//go.transform.position+=go.transform.up*10f;
			//go.transform.localScale=new Vector3(0.2f,0.2f,0.2f);
			move=1;
			//Debug.Log(" dirn");
		}
		if (move == 1) {
			/*Quaternion q=Quaternion.LookRotation(movement-transform.position+0.01f*transform.forward);
			transform.rotation=Quaternion.Lerp(transform.rotation,q,Time.deltaTime*2f);
			//anim.SetFloat("turn",0.5f);
			if(float.IsNaN(Vector3.Angle(transform.forward,movement-transform.position))){
				move=0;
				return;
			}
			if(gameObject.name.Equals("Ethan"))
				Debug.Log(Vector3.Angle(transform.forward,movement-transform.position));
			if(Vector3.Angle(transform.forward,movement-transform.position)<=2f)
			*/	move=2;
		}
		if (move == 2) {
			anim.SetFloat("walk",0.25f);
			//Debug.DrawLine(transform.position,movement);
			if(Vector3.Distance(transform.position,movement)<1f||outOffootpath()){
				move=0;
				//Debug.Log("changing dirn");
				anim.SetFloat("walk",0f);
			}
		}
	}
	bool outOffootpath(){
		NavMeshHit hit;
		NavMesh.SamplePosition(transform.position+(movement-transform.position).normalized,out hit,4f,1);
		if(Vector3.Distance(transform.position+(movement-transform.position).normalized,hit.position)>0.1f){
			return true;
		}
		return false;
	}
}
