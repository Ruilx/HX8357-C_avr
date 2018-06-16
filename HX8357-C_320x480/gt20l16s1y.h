/*
 * gt20l16s1y.h
 *
 * Created: 2018/01/22 周一 1:11:10
 *  Author: Ruilx
 */ 


#ifndef GT20L16S1Y_H_
#define GT20L16S1Y_H_

#include "main.h"
#include "utils.h"

class GT20L16S1Y{
	uint8 out = 0;
	uint8 in = 0;
	uint8 sck = 0;
	uint8 cs = 0;
	ulong fontAddr = 0;

	void sendCommand(uchar dat){
		for(uchar i = 0; i < 8; i++){
			Utils::WF(this->in, (bool)(dat & 0x80));
			dat <<= 1;
			Utils::WF(this->sck, false);
			Utils::WF(this->sck, true);
		}
	}

	uchar readData(){
		uchar dat = 0;
		Utils::WF(this->sck, true);
		for(uchar i = 0; i < 8; i++){
			Utils::WF(this->out, true);
			Utils::WF(this->sck, false);
			dat <<= 1;
			dat |= (Utils::RF(this->out) ? 1 : 0);
			Utils::WF(this->sck, true);
		}
		return dat;
	}

	void readBytes(uchar hiAddr, uchar midAddr, uchar loAddr, uchar *buf, uchar len){
		Utils::WF(this->cs, false);
		Utils::WF(this->sck, false);
		this->sendCommand(0x03);
		this->sendCommand(hiAddr);
		this->sendCommand(midAddr);
		this->sendCommand(loAddr);
		for(uchar i = 0; i < len; i++){
			*buf = readData();
			buf++;
		}
		Utils::WF(this->cs, true);
	}
public:
	uchar getGB2312Char(char *text, uchar *buf, uchar len){
		uchar i = 0;
		uchar hiAddr, midAddr, loAddr;
		if(len < 16){
			return 0;
		}
		while(text[i] > 0x00){
			if((text[i] >= 0xB0) && (text[i] <= 0xF7) && (text[i + 1] >= 0xA1)){
				/*国标简体（GB2312）汉字在字库IC中的地址由以下公式来计算：*/
				/*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0*/
				/*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
				this->fontAddr = (text[i] - 0xB0) * 94;
				this->fontAddr += (text[i+1] - 0xA1) + 846;
				this->fontAddr = (ulong)(this->fontAddr * 32);

				hiAddr = (this->fontAddr & 0xFF0000) >> 16; /*地址的高8位,共24位*/
				midAddr = (this->fontAddr & 0xFF00) >> 8; /*地址的中8位,共24位*/
				loAddr = (this->fontAddr & 0xFF); /*地址的低8位,共24位*/
				this->readBytes(hiAddr, midAddr, loAddr, buf, 32);
				return 32;
				i++;
				i++;
			}else if((text[i] >= 0xA1) && (text[i] <= 0xA3) && (text[i+1] >= 0xA1)){
				/*国标简体（GB2312）15x16点的字符在字库IC中的地址由以下公式来计算：*/
				/*Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0*/
				/*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
				fontAddr = (text[i] - 0xA1) * 94;
				fontAddr = (text[i+1] - 0xA1);
				fontAddr = (ulong)(fontAddr * 32);

				hiAddr = (fontAddr & 0xFF0000) >> 16;
				midAddr = (fontAddr & 0xFF00) >> 8;
				loAddr = (fontAddr & 0xFF);
				this->readBytes(hiAddr, midAddr, loAddr, buf, 32);
				return 32;
				i++;
				i++;
			}else if((text[i] >= 0x20) && (text[i] <= 0x7E)){
				fontAddr = (text[i] - 0x20);
				fontAddr = (ulong)(fontAddr * 16);
				fontAddr = (ulong)(fontAddr + 0x3CF80);

				hiAddr = (fontAddr & 0xFF0000) >> 16;
				midAddr = (fontAddr & 0xFF00) >> 8;
				loAddr = (fontAddr & 0xFF);
				this->readBytes(hiAddr, midAddr, loAddr, buf, 16);
				return 16;
				i++;
			}else{
				return 0;
				i++;
			}
		}
		return 0;
	}

public:
	GT20L16S1Y(){

	}
	GT20L16S1Y(uint8 out, uint8 in, uint8 sck, uint8 cs){
		this->out = out;
		this->in = in;
		this->sck = sck;
		this->cs = cs;

		Utils::DF(this->out, Utils::Input);
		Utils::DF(this->in, Utils::Output);
		Utils::DF(this->sck, Utils::Output);
		Utils::DF(this->cs, Utils::Output);
	}
};



#endif /* GT20L16S1Y_H_ */