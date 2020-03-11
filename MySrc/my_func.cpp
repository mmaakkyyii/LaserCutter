/*
 * my_func.cpp
 *
 *  Created on: Mar 7, 2020
 *      Author: kyoro
 */
#include "my_func.h"
#include "LCD.h"
#include "stepping_motor.h"

LCD lcd;
SteppingMotor motor1(SM1dir_GPIO_Port,SM1dir_Pin,SM1pulse_GPIO_Port,SM1pulse_Pin);

void Init(){
	lcd.init();
}

void Loop(){
	for(int i=0;i<200;i++){
		motor1.step(1);
		HAL_Delay(1);
	}
	HAL_Delay(500);
	for(int i=0;i<200;i++){
		motor1.step(-1);
		HAL_Delay(1);
	}
	HAL_Delay(500);

}

