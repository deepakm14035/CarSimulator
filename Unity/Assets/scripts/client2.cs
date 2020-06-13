using UnityEngine;
using UnityEngine.UI;
using System.Net.Sockets;
using System.Collections.Generic;
using System;
using System.Net;
using System.IO;
using System.Threading;
using System.Text;
public class client2 : MonoBehaviour {
	bool socketReady;
	public UdpClient socket,socket2;
	public InputField message;
	Thread receiveThread;
	public String text=null;
	public GameObject mainmenu;
	int send_port,recv_port;
	string send_ip;
	byte[] ip=null;
	public void sendData(Byte[] data){
		if(socketReady)
			socket.Send (data,data.Length);
	}
	public void getData(){
		if(ip==null)
			ip=new Byte[]{192,168,55,245};
		while (true) {
			if (socketReady) {
				try {
					// Bytes empfangen.
					IPEndPoint anyIP = new IPEndPoint (new System.Net.IPAddress (ip), 15002);
					byte[] data = socket2.Receive (ref anyIP);
					text = Encoding.UTF8.GetString (data);
					//text="1";
				} catch (Exception err) {
					print (err.ToString ());
				}
			}
		}
	}
	public void connectToServer(){
		if (socketReady)
			return;
		int port = 15001;
		if(send_port!=0||send_port!=null)
			port=send_port;
		if(send_ip==null||send_ip.Length==0)
			send_ip="192.168.55.245";
		try{
			socket=new UdpClient(send_ip,port);
			socketReady=true;
			Debug.Log ("connected to server");
		}
		catch(Exception e){
			Debug.Log("socket error:"+e.Message);
		}
		//socket.Send (System.Text.Encoding.UTF8.GetBytes("Hello!"),6);
	}
	
	// Use this for initialization
	void Start () {
		//IPEndPoint anyIP = new IPEndPoint (new System.Net.IPAddress (new Byte[]{192,168,55,245}), 4002);
		mainmenu=GameObject.Find("mainmenu");
		if(mainmenu!=null){
			send_ip= mainmenu.GetComponent<startgame>().ipaddress;
			send_port= mainmenu.GetComponent<startgame>().port;
		}
		else{
			send_port=15001;
			send_ip="192.168.55.245";
		}
		string[] ipa = send_ip.Split ('.');
		byte[] arr = new byte[4];
		for (int i=0; i<4; i++) {
			arr [i] = byte.Parse (ipa [i]);
		}
		socket2 = new UdpClient (15002);
		receiveThread = new Thread(new ThreadStart(getData));
		receiveThread.IsBackground = true;
		receiveThread.Start();
	}
	
	// Update is called once per frame
	void Update () {
		if (!socketReady) {
			connectToServer ();
		}
		
	}
	private void onincomingdata(string data){
		Debug.Log ("client:"+data);
	}
	void OnApplicationQuit(){
		receiveThread.Abort ();
		if(socket!=null)
			socket.Close ();
		if(socket2!=null)
			socket2.Close ();
		
	}
	
	
}
