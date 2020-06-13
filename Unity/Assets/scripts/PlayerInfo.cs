using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[System.Serializable]
public class playerInfo{
	int[,] grid;
	float car_position_x,car_position_y;
	float target_x,target_y;
	float pose,velocity;
	int reward;
	float gas_pedal, steer, brake;
}