using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[ExecuteInEditMode]
public class CameraMaterialChanger : MonoBehaviour {
	
	public Color[] myColor; // color you want the camera to render it as
	public Material[] material; // material you want the camera to change
	public string colorPropertyName="_Color"; // name of the color property in the material's shader

	public string[] tags;
	public Material[] colors;
	bool flag=true;
	void awake(){
		tags=new string[]{"person","tree","building","Player2","road"};
		GameObject[] objects;
		Material[] mtemp;
		Dictionary<Material,string> materials=new Dictionary<Material, string>();
		for(int i=0;i<tags.Length;i++){
			objects=GameObject.FindGameObjectsWithTag(tags[i]);
			for(int j=0;j<objects.Length;j++){
				if(objects[j].transform.childCount==0&&objects[j].GetComponent<Renderer>()!=null){
					mtemp=objects[j].GetComponent<Renderer>().sharedMaterials;
					for(int k=0;k<mtemp.Length;k++){
						if(mtemp[k]!=null&&!materials.ContainsKey(mtemp[k])){
							materials.Add(mtemp[k],tags[i]);
						}
					}
				}
				else if(objects[j].transform.childCount>0){
					for(int l=1;l<objects[j].transform.childCount;l++){
						if(objects[j].transform.GetChild(l).GetComponent<Renderer>()!=null){
							mtemp=objects[j].transform.GetChild(l).GetComponent<Renderer>().sharedMaterials;
							for(int k=0;k<mtemp.Length;k++){
								if(mtemp[k]!=null&&!materials.ContainsKey(mtemp[k])){
									materials.Add(mtemp[k],tags[i]);
								}
							}
						}
					}
				}
			}

			//materials.Add();
		}
		material=new Material[materials.Count];
		myColor=new Color[materials.Count];
		int index=0;
		foreach(Material key in materials.Keys){
			material[index]=key;
			for(int i=0;i<tags.Length;i++){
				if(materials[key].Equals(tags[i])){
					myColor[index]=colors[i].GetColor(colorPropertyName);
					break;
				}
			}
			index++;
		}
		//Debug.Log("matlength-"+material.Length);
	}

	void OnPreRender() {
		if(flag){
			awake();
			flag=false;
		}
		_default=new Color[myColor.Length];
		for(int i=0;i<myColor.Length;i++){
			if(!material[i].HasProperty(colorPropertyName)){
				//Debug.Log(material[i].name);
				continue;
			}
			_default[i] = material[i].GetColor(colorPropertyName);
			material[i].SetColor(colorPropertyName, myColor[i]);
		}
	}
	
	void OnPostRender() {
		if(myColor==null){
			awake();
		}
		for(int i=0;i<myColor.Length;i++){
			if(!material[i].HasProperty(colorPropertyName)){
				continue;
			}
			material[i].SetColor(colorPropertyName, _default[i]);
		}
	}
	
	private Color[] _default;
}