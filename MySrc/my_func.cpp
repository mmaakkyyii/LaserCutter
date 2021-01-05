/*
 * my_func.cpp
 *
 *  Created on: Mar 7, 2020
 *      Author: kyoro
 */
#include "my_func.h"
#include "LCD.h"
#include "stepping_motor.h"
#include "xy_controll.h"
#include "gpio.h"
#include "tim.h"
#include "stdio.h"
#include "stdlib.h"
#include "debug_print.h"
#include "usart.h"


LCD lcd;
SteppingMotor motor1(SM1dir_GPIO_Port,SM1dir_Pin,SM1pulse_GPIO_Port,SM1pulse_Pin);
SteppingMotor motor2(SM2dir_GPIO_Port,SM2dir_Pin,SM2pulse_GPIO_Port,SM2pulse_Pin);
SteppingMotor motor3(SM3dir_GPIO_Port,SM3dir_Pin,SM3pulse_GPIO_Port,SM3pulse_Pin);

XYController xy_controller(motor3,motor1,motor2,-1);


void PWM(int duty){//0~99
	if(duty<0)duty=0;
	else if(duty>999)duty=999;
	if(duty==0)HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PinState::GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PinState::GPIO_PIN_SET);

/*
	TIM_OC_InitTypeDef sConfigOC;

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = duty;
	sConfigOC.OCPolarity = TIM_OCPOLAR	ITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

	HAL_TIM_PWM_ConfigChannel(&htim3,&sConfigOC,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
*/
}

const int uart_data_num=32;
uint8_t UART1_Data[uart_data_num];

void Init(){
	lcd.init();
	char a[8];
	int n = 0;
	n=sprintf(a,"Hello!");
	lcd.print(a, n,0);
	HAL_Delay(200);
	n=sprintf(a,"world!");
	lcd.print(a, n,1);
	HAL_Delay(200);
	n=sprintf(a,"moapachi");
	lcd.print(a, n,0);
	HAL_Delay(200);
    HAL_UART_Receive_IT(&huart1, (uint8_t*)UART1_Data, uart_data_num);
	HAL_TIM_Base_Start_IT(&htim16);
	HAL_TIM_Base_Start_IT(&htim17);

}

int x=0;
int t=0;
void Loop(){
	if(xy_controller.isBusy()){
		PWM(500);
	}else{
		PWM(0);
	}

/*
	//Debug("");
	while(HAL_GPIO_ReadPin(IN1_GPIO_Port, IN1_Pin)==GPIO_PinState::GPIO_PIN_RESET){
		x++;
		if(HAL_GPIO_ReadPin(IN3_GPIO_Port, IN3_Pin)==GPIO_PinState::GPIO_PIN_RESET){
			motor1.step(1);
			motor2.step(-1);
		}else{
			motor3.step(1);
		}
		HAL_Delay(2);
//		break;
	}
	while(HAL_GPIO_ReadPin(IN2_GPIO_Port, IN2_Pin)==GPIO_PinState::GPIO_PIN_RESET){
		x--;
		if(HAL_GPIO_ReadPin(IN3_GPIO_Port, IN3_Pin)==GPIO_PinState::GPIO_PIN_RESET){
			motor1.step(-1);
			motor2.step(1);
		}else{
			motor3.step(-1);
		}
		HAL_Delay(2);
//		motor1.move_mm(-50);
//		motor2.move_mm(50);
//		motor3.move_mm(-50);
//		HAL_Delay(1);
//		break;
	}
	t++;
*/
}

void Interrupt_1ms(){
	xy_controller.Update();
}


void Interrupt_100ms(){
	if(HAL_GPIO_ReadPin(IN1_GPIO_Port, IN1_Pin)==GPIO_PinState::GPIO_PIN_RESET){
		if(!xy_controller.isBusy()){
			xy_controller.SetLine(-10, 0, 5);
		}
	}
	if(HAL_GPIO_ReadPin(IN2_GPIO_Port, IN2_Pin)==GPIO_PinState::GPIO_PIN_RESET){
		if(!xy_controller.isBusy()){
			xy_controller.SetLine(0, -10, 5);
		}
	}
	if(HAL_GPIO_ReadPin(IN3_GPIO_Port, IN3_Pin)==GPIO_PinState::GPIO_PIN_RESET){
		if(!xy_controller.isBusy()){
			xy_controller.SetLine(10, 10, 5);
		}
	}
	char a[8];
	int n = sprintf(a,"t=%6d",t);
	//lcd.print(a, 8,0);
	//print_uart("t=%d\r\n",t);
	t++;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    HAL_UART_Receive_IT(&huart1, (uint8_t*)UART1_Data, uart_data_num);
    char *end1,*end2,*end3,*end4;
    int num1=strtol((char*)UART1_Data,&end1,10);
    int num2=strtol(end1,&end2,10);
    int num3=strtol(end2,&end3,10);
    int num4=strtol(end3,&end4,10);
    int x=num2;
    int y=num3;
    int v=num4;
	print_uart("x=%d0y=%d0\r\n",x,y);
	char a[8];
	int n =0;
	n= sprintf(a,"x=%d   ",x);
	lcd.print(a, 8,0);
	n= sprintf(a,"y=%d   ",y);
	lcd.print(a, 8,1);
	xy_controller.SetLine(x, y, v);
}


