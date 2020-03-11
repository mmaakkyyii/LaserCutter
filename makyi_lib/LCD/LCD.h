/*
 * LCD.h
 *
 *  Created on: Mar 7, 2020
 *      Author: kyoro
 */

#ifndef LCD_LCD_H_
#define LCD_LCD_H_

#include "stdint.h"

class LCD{
public:
	LCD(){};
	void init();
	void write(uint8_t cmd,uint8_t pos);
	void print(char *p,int length);

private:
	void send(uint8_t cmd);
};



#endif /* LCD_LCD_H_ */
