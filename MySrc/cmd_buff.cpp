/*
 * cmd_buff.cpp
 *
 *  Created on: 2021/01/15
 *      Author: kyoro
 */

#include "cmd_buff.h"

CmdBuffer::CmdBuffer():buff_addr(0),receiving_addr(0){
	ClearAllBuff();
}

void CmdBuffer::ClearBuff(int addr){
	for(int i=0;i<buff_length;i++)buffer[addr][i]=' ';
}
void CmdBuffer::ClearReceivingBuff(){
	for(int i=0;i<buff_length;i++)receiving_buff[i]=' ';
	receiving_addr=0;
}
void CmdBuffer::ClearAllBuff(){
	for(int i=0;i<buff_num;i++)ClearBuff(i);
	buff_addr=0;
}

bool CmdBuffer::SetCmd(){
	if(buff_addr==buff_num)return false;
	for(int i=0;i<buff_length;i++)buffer[buff_addr][i]=receiving_buff[i];
	buff_addr++;
	ClearReceivingBuff();
	return true;

}

bool CmdBuffer::GetCmd(char* cmd_p){
	if(buff_addr==0)return false;

	for(int i=0;i<buff_length;i++){
		cmd_p[i]=buffer[0][i];
	}
	for(int i=0;i<buff_num-1;i++){
		for(int j=0;j<buff_length;j++){
			buffer[i][j]=buffer[i+1][j];
		}
	}
	ClearBuff(buff_num-1);

	buff_addr--;
	return true;
}

bool CmdBuffer::SetChar(char c){
	receiving_buff[receiving_addr]=c;
	receiving_addr++;
}


