#include "debug_print.h"

#include "stdarg.h"
#include "stdio.h"
#include "usart.h"

#define HUARTx huart1 //利用するuartの指定

void print_uart(const char *format, ...){

	uint8_t buffer[128];
	va_list args;
	va_start(args,format);

	int n=vsprintf((char*)buffer,format,args);
	HAL_UART_Transmit_DMA(&HUARTx, buffer, n);

	va_end(args);

}
