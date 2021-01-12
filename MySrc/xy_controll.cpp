#include "xy_controll.h"
#include "math.h"
#include "debug_print.h"

XYController::XYController(SteppingMotor Mx1,SteppingMotor My1,SteppingMotor My2,int _y_dir,float _period_controller_ms):
	motor_x(Mx1),
	motor_y1(My1),
	motor_y2(My2),
	y_dir(_y_dir),
	pos_x_current(0),pos_y_current(0),
	pos_x_init(0)   ,pos_y_init(0),
	pos_x_target(0) ,pos_y_target(0),
	period_controller_ms(_period_controller_ms),
	required_time_ms(0),elapsed_time_ms(0),
	v_x(0),v_y(0),
	cmd(Gcode::non){

}

void XYController::SetGcode(char* p, float length){

}
void XYController::CalPath(){
	switch(cmd){
	case Gcode::G01:
		pos_x_target=v_x*elapsed_time_ms;
		pos_y_target=v_y*elapsed_time_ms;
		break;
	case Gcode::G02:
		break;
	case Gcode::G03:
		break;
	default:
		break;

	}
}
void XYController::SetLine(float x,float y,float v){//[mm] [mm] [mm/s]
	elapsed_time_ms=0; //reset timer
	pos_x_target=x;
	pos_y_target=y;
	pos_x_init=motor_x.get_pos();
	pos_y_init=motor_y1.get_pos();
	cmd=Gcode::G01;

    float dx=pos_x_target;//[mm]
    float dy=pos_y_target;//[mm]
    float l=sqrtf(dx*dx+dy*dy); //[mm]
    if(v!=0 or l!=0){
        float required_time_s=l/v; //[s]
        v_x=dx/required_time_s; //[mm/s]
        v_y=dy/required_time_s; //[mm/s]
        required_time_ms=required_time_s*1000.0; //[ms]
    }else{
    	required_time_ms=0;
        v_x=0;
        v_y=0;
    }


}

float sinAplusB(float sinA,float sinB,float cosA,float cosB){
	return sinA*cosB+cosA*sinB;
}
float cosAplusB(float sinA,float sinB,float cosA,float cosB){
	return cosA*cosB-sinA*sinB;
}

void XYController::SetArc(float x,float y,float r,float v,int dir){//[mm] [mm] [mm/s] [cw(1) or ccw(-1)]
	elapsed_time_ms=0; //reset timer
	pos_x_init=motor_x.get_pos();
	pos_y_init=motor_y1.get_pos();
	if(dir==1)cmd=Gcode::G02;
	if(dir==-1)cmd=Gcode::G03;

	pos_x_target=x;
	pos_y_target=y;
	radius=r;


	float dx=pos_x_target;
	float dy=pos_y_target;
	float l1=sqrtf(dx*dx+dy*dy);
	sinA=dy/l1;
	cosA=dx/l1;
	float l2=sqrtf(radius*radius-(0.5*l1)*(0.5*l1));
	sinB=l2/radius;
	cosB=(0.5*l1)/radius;
	sinA_B=sinAplusB(sinA, sinB, cosA, cosB);
	cosA_B=cosAplusB(sinA, sinB, cosA, cosB);
	pos_x_center=radius*cosA_B;
	pos_y_center=radius*sinA_B;
	if(cmd==Gcode::G02)theta=2*(pi/2.0-acosf(cosB));
	if(cmd==Gcode::G03)theta=2*pi-2*(pi/2.0-acosf(cosB));
	required_time_ms=radius*theta/v*1000;
	omega=theta/required_time_ms;

}

void XYController::Update_target(){
	float theta_target;
	float dt;
	float theta_pre_target;
	float pos_x_pre_target;
	float pos_y_pre_target;

	switch(cmd){
	case Gcode::G01:
		pos_x_current=motor_x.get_pos() -pos_x_init;
		pos_y_current=motor_y1.get_pos()-pos_y_init;
		pos_x_target=v_x*elapsed_time_ms*0.001;
		pos_y_target=v_y*elapsed_time_ms*0.001;
		break;
	case Gcode::G02:
		pos_x_current=motor_x.get_pos() -pos_x_init;
		pos_y_current=motor_y1.get_pos()-pos_y_init;
		theta_target=omega*elapsed_time_ms;
		dt=0.1;
		theta_pre_target=omega*(elapsed_time_ms-dt);

		pos_x_target=pos_x_center-radius*cosAplusB(sinA_B, sinf(theta_target), cosA_B, cosf(theta_target));
		pos_y_target=pos_y_center-radius*sinAplusB(sinA_B, sinf(theta_target), cosA_B, cosf(theta_target));
		pos_x_pre_target=pos_x_center-radius*cosAplusB(sinA_B, sinf(theta_pre_target), cosA_B, cosf(theta_pre_target));
		pos_y_pre_target=pos_y_center-radius*sinAplusB(sinA_B, sinf(theta_pre_target), cosA_B, cosf(theta_pre_target));
		v_x=(pos_x_target-pos_x_pre_target)/dt;
		v_y=(pos_y_target-pos_y_pre_target)/dt;
		break;
	case Gcode::G03:
		pos_x_current=motor_x.get_pos() -pos_x_init;
		pos_y_current=motor_y1.get_pos()-pos_y_init;
		theta_target=omega*elapsed_time_ms;
		dt=0.1;
		theta_pre_target=omega*(elapsed_time_ms-dt);

		pos_x_target=pos_x_center-radius*cosAplusB(sinA_B, sinf(theta_target), cosA_B, cosf(theta_target));
		pos_y_target=pos_y_center-radius*sinAplusB(sinA_B, sinf(theta_target), cosA_B, cosf(theta_target));
		pos_x_pre_target=pos_x_center-radius*cosAplusB(sinA_B, sinf(theta_pre_target), cosA_B, cosf(theta_pre_target));
		pos_y_pre_target=pos_y_center-radius*sinAplusB(sinA_B, sinf(theta_pre_target), cosA_B, cosf(theta_pre_target));
		v_x=(pos_x_target-pos_x_pre_target)/dt;
		v_y=(pos_y_target-pos_y_pre_target)/dt;
		break;
	}

}

void XYController::Update(){


	if(elapsed_time_ms>required_time_ms){
		cmd=Gcode::non;
		return;
	}
	int step_num=100;
	for(int n=0;n<step_num;n++){
		Update_target();

		if(v_x>0){
			if(pos_x_target>pos_x_current){
				motor_x.step(1);
			}else{

			}
		}else if(v_x<0){
			if(pos_x_target<pos_x_current){
				motor_x.step(-1);
			}else{

			}
		}

		if(v_y>0){
			if(pos_y_target>pos_y_current){
				motor_y1.step(1);
				motor_y2.step(y_dir*1);
			}else{

			}
		}else if(v_y<0){
			if(pos_y_target<pos_y_current){
				motor_y1.step(-1);
				motor_y2.step(-y_dir*1);
			}else{

			}
		}
	}

	elapsed_time_ms+=period_controller_ms;
}
