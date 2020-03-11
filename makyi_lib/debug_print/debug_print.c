/*
 * debug_print.cp
 *
 *  Created on: Mar 3, 2020
 *      Author: kyoro
 */
#include "debug_print.h"
#include "usart.h"
#include "stdint.h"
//TODO befor use this
//check NVIC Setting USARTX global interrupt

//#define Debug_Print(fmt, ...) std::printf(fmt, __VA_ARGS__)

void Debug(const char *fromat, ...){
	uint8_t aa[6]={'x','y','z','\n'};
	HAL_UART_Transmit_IT(&huart1, aa, 4);
}



