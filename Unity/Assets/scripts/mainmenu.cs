using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class mainmenu : MonoBehaviour {
	public RawImage background;
	Camera mcamera;

	// Use this for initialization
	void Start () {
		background.rectTransform.sizeDelta=new Vector2(Screen.width,Screen.height);
		mcamera=Camera.main;
		//mcamera.orthographicSize;
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
