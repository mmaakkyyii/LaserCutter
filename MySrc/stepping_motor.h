/*
 * stepping_motor.h
 *
 *  Created on: 2020/03/11
 *      Author: kyoro
 */

#ifndef STEPPING_MOTOR_H_
#define STEPPING_MOTOR_H_

#include "gpio.h"

class SteppingMotor{
private:
	GPIO_TypeDef* dir_port;
	uint16_t dir_pin;
	GPIO_TypeDef* pulse_port;
	uint16_t pulse_pin;

public:
	SteppingMotor(GPIO_TypeDef* _dir_port, uint16_t _dir_pin,GPIO_TypeDef* _pulse_port, uint16_t _pulse_pin);
	void pulse(int dir);
	void step(int dir);
};



#endif /* STEPPING_MOTOR_H_ */
