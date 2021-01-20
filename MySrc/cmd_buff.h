/*
 * cmd_buff.h
 *
 *  Created on: 2021/01/15
 *      Author: kyoro
 */

#ifndef CMD_BUFF_H_
#define CMD_BUFF_H_

class CmdBuffer{
private:
	static const int buff_num=64;
	static const int buff_length=32;
	char buffer[buff_num][buff_length];
	char receiving_buff[buff_length];
	int receiving_addr;
	int buff_addr;

	void ClearAllBuff();
	void ClearBuff(int addr);



public:
	CmdBuffer();
	bool SetChar(char c);
	bool SetCmd();
	bool GetCmd(char* cmd_p);
	bool CanReceive(){return buff_addr<16;}
	int GetCmdType();
	int GetX();
	int GetY();
	int GetF();
	int GetR();
	int GetBuffAddr(){return buff_addr;}
	void ClearReceivingBuff();


};



#endif /* CMD_BUFF_H_ */
