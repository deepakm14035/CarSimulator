using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
public class startgame : MonoBehaviour {
	public int selection=0;
	public int port;
	public string ipaddress;
	public InputField ip,portno;
	public GameObject loadingscreen;
	public Slider slider;
	public void buttonpressed(){
		Debug.Log ("button pressed");
		ipaddress = ip.text;
		port = int.Parse (portno.text);
		StartCoroutine(loadscreen());
	}

	IEnumerator loadscreen(){
		AsyncOperation ao=SceneManager.LoadSceneAsync (1);
		loadingscreen.SetActive(true);
		while(ao.isDone){
			Debug.Log("%%%%%%%%%"+ao.progress);
			float progress=Mathf.Clamp01(ao.progress/0.9f);
			slider.value=progress;
		}
		yield return null;
	}
	
	// Use this for initialization
	void Start () {
		DontDestroyOnLoad (transform.gameObject);
	}
	// Update is called once per frame
	void Update () {
		
	}
}
