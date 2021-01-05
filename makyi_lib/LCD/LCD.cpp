/*
 * LCD.cpp
 *
 *  Created on: Mar 7, 2020
 *      Author: kyoro
 */
#include "LCD.h"
#include "i2c.h"
uint8_t data[2];


void LCD::init(){

	HAL_Delay(50);
	send(0x38);//Function set
	HAL_Delay(100);
	send(0x39);//Function set enable
	HAL_Delay(100);
	send(0x14);//OSC frequency
	HAL_Delay(100);
	send(0x70);//contrast set
	HAL_Delay(100);
	send(0x56);//power ICON contrust
	HAL_Delay(100);
	send(0x6C);//follower control
	HAL_Delay(200);
	send(0x38);//Function set disable
	HAL_Delay(100);
	send(0x0e);//display on
	HAL_Delay(100);
	send(0x01);//clear display
	HAL_Delay(20);

}

void LCD::print(char *p,int length,int col){
//	send(0x01);
	if(col==0)send(0x80);
	if(col==1)send(0x80+0x40);
	uint8_t a[length];

	for(int i=0;i<length;i++){
		write(p[i],0);
		//HAL_Delay(5);
		a[i]=p[i];
	}
//
	//uint16_t LCD_addr=0x7c;
	//HAL_I2C_Mem_Write_DMA(&hi2c1, LCD_addr, 0x04, 8, a, 8);

	//send(0x02);

}
void LCD::write(uint8_t cmd,uint8_t pos){
	uint16_t LCD_addr=0x7c;
	data[0]= 0x40;
	data[1]= cmd;
	HAL_I2C_Master_Transmit(&hi2c1,LCD_addr,data,2,1000);
	//HAL_I2C_Master_Transmit_DMA(&hi2c1,LCD_addr,data,2);
}

void LCD::send(uint8_t cmd){
	uint16_t LCD_addr=0x7c;
	data[0] = 0x00;
	data[1]	= cmd;
	HAL_I2C_Master_Transmit(&hi2c1,LCD_addr,data,2,1000);
	//HAL_I2C_Master_Transmit_DMA(&hi2c1,LCD_addr,data,2);
}



