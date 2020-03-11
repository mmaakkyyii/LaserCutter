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
}
