/*
 * my_func.h
 *
 *  Created on: Mar 7, 2020
 *      Author: kyoro
 */

#ifndef MY_FUNC_H_
#define MY_FUNC_H_


#if __cplusplus
extern "C"{
#endif

void Init();

void Loop();

void Interrupt_1ms();
void Interrupt_100ms();

#if __cplusplus
};
#endif


#endif /* MY_FUNC_H_ */
