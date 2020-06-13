using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CarServiceResponse {

	public static void ServiceCallBack(string service, string response) {
		if(response == null)
			Debug.Log ("ServiceCallback for service " + service);
		else
			Debug.Log ("ServiceCallback for service " + service + " response " + response);
	}
}
