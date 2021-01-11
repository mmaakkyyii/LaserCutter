#include "stepping_motor.h"

SteppingMotor::	SteppingMotor(GPIO_TypeDef* _dir_port, uint16_t _dir_pin,GPIO_TypeDef* _pulse_port, uint16_t _pulse_pin):
					dir_port(_dir_port),
					dir_pin(_dir_pin),
					pulse_port(_pulse_port),
					pulse_pin(_pulse_pin),
					pos(0),step_num(0){

}

void SteppingMotor::pulse(int dir){
	if(dir>0)HAL_GPIO_WritePin(dir_port, dir_pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(dir_port, dir_pin, GPIO_PIN_RESET);
	HAL_GPIO_TogglePin(pulse_port, pulse_pin);

}

void SteppingMotor::step(int dir){
	if(dir>1)dir=1;
	else if(dir<1)dir=-1;
	pulse(dir);
	step_num+=dir;
	pos+=dir*deg_par_step*3.14/180.0*R;
}

void SteppingMotor::move_mm(int length){
}

void SteppingMotor::move(int num){
}
