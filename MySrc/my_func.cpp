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
SteppingMotor motor2(SM2dir_GPIO_Port,SM2dir_Pin,SM2pulse_GPIO_Port,SM2pulse_Pin);
SteppingMotor motor3(SM3dir_GPIO_Port,SM3dir_Pin,SM3pulse_GPIO_Port,SM3pulse_Pin);

void Init(){
	lcd.init();
}

void Loop(){
	for(int i=0;i<200;i++){
		motor1.step(1);
		motor2.step(1);
		motor3.step(1);
		HAL_Delay(1);
	}
	HAL_Delay(500);
	for(int i=0;i<200;i++){
		motor1.step(-1);
		motor2.step(-1);
		motor3.step(-1);
		HAL_Delay(1);
	}
	HAL_Delay(500);

}

