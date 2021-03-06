#ifndef XY_CONTROLL_H_
#define XY_CONTROLL_H_

#include "stepping_motor.h"

enum class Gcode{
	non,
	G00,
	G01,
	G02,
	G03
};
class XYController{
private:
	SteppingMotor motor_x;
	SteppingMotor motor_y1;
	SteppingMotor motor_y2;
	int y_dir; //2nd stepping motor dir 1 or -1
	float pos_x_current; 		//[mm]
	float pos_y_current;		//[mm]
	float pos_x_init;			//[mm]
	float pos_y_init;			//[mm]
	float pos_x_target;			//[mm]
	float pos_y_target;			//[ms]
	float period_controller_ms; //[ms]
	float required_time_ms; 	//[ms]
	float elapsed_time_ms; 		//[ms]
	float v_x; 					//[mm/s]
	float v_y; 					//[mm/s]

	float sinA;					//
	float cosA;					//
	float sinB;					//
	float cosB;					//
	float sinA_B;				//
	float cosA_B;				//
	float pos_x_center;			//
	float pos_y_center;			//
	float radius;				//[mm]
	float theta;				//[rad/ms]
	float omega;				//[rad/ms]

	Gcode cmd;
	const float pi=3.141592;

public:
	XYController(SteppingMotor Mx1,SteppingMotor My1,SteppingMotor My2,int _y_dir,float _period_controller_ms);
	void SetGcode(char* p, float length);
	void SetLine(float x,float y,float v);
	void SetArc(float x,float y,float r,float v, int dir);
	void Update_target();
	void Update();
	void CalPath();
	float GetPosX(){return motor_x.get_pos();}
	float GetPosY(){return motor_y1.get_pos();}
	bool isBusy(){
		if(elapsed_time_ms<=required_time_ms)return true;
		else return false;};//return true when stop
};



#endif //XY_CONTROLL_H_
