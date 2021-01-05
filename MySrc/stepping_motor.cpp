#include "stepping_motor.h"

SteppingMotor::	SteppingMotor(GPIO_TypeDef* _dir_port, uint16_t _dir_pin,GPIO_TypeDef* _pulse_port, uint16_t _pulse_pin):
					dir_port(_dir_port),
					dir_pin(_dir_pin),
					pulse_port(_pulse_port),
					pulse_pin(_pulse_pin){

}

void SteppingMotor::pulse(int dir){
	if(dir>0)HAL_GPIO_WritePin(dir_port, dir_pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(dir_port, dir_pin, GPIO_PIN_RESET);
	HAL_GPIO_TogglePin(pulse_port, pulse_pin);

}

//1.8 [deg/step]
void SteppingMotor::step(int dir){
	pulse(dir);
	pulse(dir);
	if(dir>1)dir=1;
	else if(dir<0)dir=-1;
	step_num+=dir;
	pos+=dir*deg_par_step*3.14/180.0*R;
}

void SteppingMotor::move_mm(int length){
	int num=(length/R)*180.0/3.141592/deg_par_step;
	move(num);
}

void SteppingMotor::move(int num){
	if(num>0){
		for(int i=0;i<num;i++){
			step(1);
			HAL_Delay(delay_time);
		}
	}else if(num<0){
		for(int i=0;i<-num;i++){
			step(-1);
			HAL_Delay(delay_time);
		}

	}
}
