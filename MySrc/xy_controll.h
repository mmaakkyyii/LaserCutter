#ifndef XY_CONTROLL_H_
#define XY_CONTROLL_H_

#include "stepping_motor.h"

class XYController{
private:
	SteppingMotor motor_x;
	SteppingMotor motor_y1;
	SteppingMotor motor_y2;
	float x_pos;
	float y_pos;
	float x_pos_init;
	float y_pos_init;
	int y_dir;
	int timer_ms; //[ms]
	int counter_ms; //[ms]
	float v_x; //[mm/s]
	float v_y; //[mm/s]
public:
	XYController(SteppingMotor Mx1,SteppingMotor My1,SteppingMotor My2,int _y_dir);
	void SetGcode(char* p, float length);
	void SetLine(float x,float y,float v);
	void Update();
	bool isBusy(){return (counter_ms<timer_ms);};//return true when stop
};

#endif //XY_CONTROLL_H_
