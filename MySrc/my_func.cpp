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
#include "string.h"
#include "debug_print.h"
#include "usart.h"


LCD lcd;
SteppingMotor motor1(SM1dir_GPIO_Port,SM1dir_Pin,SM1pulse_GPIO_Port,SM1pulse_Pin);
SteppingMotor motor2(SM2dir_GPIO_Port,SM2dir_Pin,SM2pulse_GPIO_Port,SM2pulse_Pin);
SteppingMotor motor3(SM3dir_GPIO_Port,SM3dir_Pin,SM3pulse_GPIO_Port,SM3pulse_Pin);

XYController xy_controller(motor3,motor1,motor2,-1,1);


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

const int cmd_data_num=32;
int cmd_addr=0;
bool cmd_received=false;
bool cmd_receiving=false;
bool cmd_next_empty=true;
uint8_t cmd_data[cmd_data_num];
uint8_t next_cmd_data[cmd_data_num];
uint8_t UART1_Data[1];

void clear_buff(){
	for(int i=0;i<cmd_data_num;i++)cmd_data[i]=' ';
}
void clear_next_buff(){
	for(int i=0;i<cmd_data_num;i++)next_cmd_data[i]=' ';
}


void Init(){
	lcd.init();
	lcd.print_lcd(0, "makyi ");
	lcd.print_lcd(1, "world!");
	//HAL_Delay(200);
    HAL_UART_Receive_DMA(&huart1, (uint8_t*)UART1_Data, 1);
	HAL_TIM_Base_Start_IT(&htim16);
	HAL_TIM_Base_Start_IT(&htim17);

}

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
	if(!xy_controller.isBusy() && cmd_next_empty && !cmd_receiving){
		print_uart("o");
	}
//	if(!xy_controller.isBusy()){
//		if(!cmd_next_empty){
//			memcpy(cmd_data,next_cmd_data,cmd_data_num);
//			cmd_received=true;
//			cmd_next_empty=true;
//			clear_next_buff();
//		}else{
//		}
//	}
	if(HAL_GPIO_ReadPin(IN1_GPIO_Port, IN1_Pin)==GPIO_PinState::GPIO_PIN_RESET){
		if(HAL_GPIO_ReadPin(IN3_GPIO_Port, IN3_Pin)==GPIO_PinState::GPIO_PIN_RESET){
			xy_controller.SetLine(-10, 0, 100);
		}else{
			xy_controller.SetLine(10, 0, 100);
		}
	}
	if(HAL_GPIO_ReadPin(IN2_GPIO_Port, IN2_Pin)==GPIO_PinState::GPIO_PIN_RESET){
		if(HAL_GPIO_ReadPin(IN3_GPIO_Port, IN3_Pin)==GPIO_PinState::GPIO_PIN_RESET){
			xy_controller.SetLine(0, -10, 100);
		}else{
			xy_controller.SetLine(0, 10, 100);
		}
	}

	if(cmd_received){
		int code;
		sscanf((char*)cmd_data,"G%d%s",&code,cmd_data);
		float x,y,r,v;
		switch(code){
		case 0:
			break;
		case 1:
			sscanf((char*)cmd_data,"X%f%s",&x,cmd_data);
			sscanf((char*)cmd_data,"Y%f%s",&y,cmd_data);
			sscanf((char*)cmd_data,"F%f%s",&v,cmd_data);
			xy_controller.SetLine(x, y, v);
			break;
		case 2:
			sscanf((char*)cmd_data,"X%f%s",&x,cmd_data);
			sscanf((char*)cmd_data,"Y%f%s",&y,cmd_data);
			sscanf((char*)cmd_data,"R%f%s",&r,cmd_data);
			sscanf((char*)cmd_data,"F%f%s",&v,cmd_data);
			xy_controller.SetArc(x, y, r, v, 1);
			break;
		case 3:
			sscanf((char*)cmd_data,"X%f%s",&x,cmd_data);
			sscanf((char*)cmd_data,"Y%f%s",&y,cmd_data);
			sscanf((char*)cmd_data,"R%f%s",&r,cmd_data);
			sscanf((char*)cmd_data,"F%f%s",&v,cmd_data);
			xy_controller.SetArc(x, y, r, v, -1);
			break;
		}
		cmd_next_empty=true;
		cmd_received=false;
		//clear_buff();
		//lcd.print_lcd(0, "x=%4d",x);
		//lcd.print_lcd(1, "y=%4d",y);

	}

	lcd.print_lcd(0, "x=%4d",(int)xy_controller.GetPosX());
	lcd.print_lcd(1, "y=%4d",(int)xy_controller.GetPosY());


}

char sum=0;
bool check_sum=false;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    HAL_UART_Receive_DMA(&huart1, (uint8_t*)UART1_Data, 1);
    cmd_receiving=true;
    if(check_sum){
    	check_sum=false;
    	if(sum==UART1_Data[0]){
    		sum=0;
        	cmd_addr=0;
        	cmd_receiving=false;
        	cmd_received=true;
        	lcd.print_lcd(0, "       o");
    		print_uart("c");
    	}else{
    		sum=0;
        	cmd_addr=0;
    		lcd.print_lcd(0, "       x");
    		print_uart("x");
    	}
    }else if(UART1_Data[0]!=';'){
    	cmd_data[cmd_addr]=UART1_Data[0];
    	sum+=UART1_Data[0];
    	cmd_addr++;
    }else{
    	cmd_data[cmd_addr]=UART1_Data[0];
    	sum+=UART1_Data[0];
    	check_sum=true;
    }
 }


