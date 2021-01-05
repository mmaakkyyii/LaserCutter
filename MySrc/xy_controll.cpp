#include "xy_controll.h"
#include "math.h"
#include "debug_print.h"

XYController::XYController(SteppingMotor Mx1,SteppingMotor My1,SteppingMotor My2,int _y_dir):
	motor_x(Mx1),
	motor_y1(My1),
	motor_y2(My2),
	y_dir(_y_dir),
	x_pos(0),y_pos(0),
	x_pos_init(0),y_pos_init(0){
}

void XYController::SetGcode(char* p, float length){

}
void XYController::SetLine(float x,float y,float v){//[mm] [mm] [mm/s]
	float l=sqrtf(x*x+y*y);
	float t=l/v; //[s]
	v_x=x/t;//[mm/s]
	v_y=y/t;//[mm/s]

	timer_ms=(int)(t*1000);
	counter_ms=0; //reset timer
	x_pos_init=motor_x.get_pos();
	y_pos_init=motor_y1.get_pos();

}
void XYController::Update(){
	x_pos=motor_x.get_pos()-x_pos_init;
	y_pos=motor_y1.get_pos()-y_pos_init;


	if(counter_ms>timer_ms){
		return;
	}
	if(v_x>0){
		if(v_x*counter_ms*0.001>x_pos){
			motor_x.step(1);
		}
	}else{
		if(v_x*counter_ms*0.001<x_pos){
			motor_x.step(-1);
		}
	}

	if(v_y>0){
		if(v_y*counter_ms*0.001>y_pos){
			motor_y1.step(1);
			motor_y2.step(y_dir);
		}
	}else{
		if(v_y*counter_ms*0.001<y_pos){
			motor_y1.step(-1);
			motor_y2.step(-y_dir);
		}
	}


	counter_ms++;
}
