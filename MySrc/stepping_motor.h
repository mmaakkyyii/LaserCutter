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
	const float deg_par_step=1.8/16.0; //[deg/step] 1/16
	const float R = 7; //[mm] //7
	float pos;
	int step_num;
	const int delay_time=2;
	void pulse(int dir);

public:
	SteppingMotor(GPIO_TypeDef* _dir_port, uint16_t _dir_pin,GPIO_TypeDef* _pulse_port, uint16_t _pulse_pin);
	void step(int dir);
	void move_mm(int length);
	void move(int num);
	void init(){pos=0;}
	float get_pos(){return pos;}
	int get_step_num(){return step_num;}


};



#endif /* STEPPING_MOTOR_H_ */
