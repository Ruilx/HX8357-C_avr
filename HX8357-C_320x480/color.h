/*
 * color.h
 *
 * Created: 2017/10/25 周三 0:05:33
 *  Author: Ruilx
 */ 


#ifndef COLOR_H_
#define COLOR_H_

#include "main.h"

#define ConC16I(C16) (((C16) & 0xF800)>>11) + ((C16) & 0x07E0) + (((C16) & 0x001F) << 11)
#define ConC24To16(C24) ((((C24) & 0x00F80000) >> 8) | (((C24) & 0x0000FC00) >> 5) | (((C24) & 0x000000F8) >> 3))
#define ConC24To16_RGB(R,G,B) ((((uint16)((R) & 0xF8)) << 8) | (((uint16)((G) & 0xFC)) << 3) | (((uint16)((B) & 0xF8)) >> 3 ))
#define ConC16To24_R(C16) ((((C16) >> 8) & 0xF8) | 0x07)
#define ConC16To24_G(C16) ((((C16) >> 3) & 0xFC) | 0x03)
#define ConC16To24_B(C16) (((C16) << 3) | 0x07)

#define ConC24To16_R(C,R) ( ((C) & 0x07FF) | (((uint16)((R) & 0xF8)) << 8) )
#define ConC24To16_G(C,G) ( ((C) & 0xF81F) | (((uint16)((G) & 0xFC)) << 3) )
#define ConC24To16_B(C,B) ( ((C) & 0x001F) | (((uint16)((B) & 0xF8)) >> 3) )

class Color{
	enum ColorMode{
		Color_Rgb888 = 0,
		Color_Rgb565 = 1,
	};

	uint8 red = 0x00;
	uint8 green = 0x00;
	uint8 blue = 0x00;
	uint16 color565 = 0x0000;
	ColorMode mode;
public:
	Color(uint8 r, uint8 g, uint8 b){
		this->red = r;
		this->green = g;
		this->blue = b;
		this->mode = Color_Rgb888;
	}

	Color(uint16 color){
		this->color565 = color;
		this->mode = Color_Rgb565;
	}

	Color(){
		this->mode = Color_Rgb565;
	}

	uint16 getRgb565(){
		switch(this->mode){
			case Color_Rgb565:
				return this->color565;
			case Color_Rgb888:
				return ConC24To16_RGB(this->red, this->green, this->blue);
			default:
				return 0x0000;
		}
	}

	void  getRgb888(uint8 *r, uint8 *g, uint8 *b){
		switch(this->mode){
			case Color_Rgb565:
				*r = ConC16To24_R(this->color565);
				*g = ConC16To24_G(this->color565);
				*b = ConC16To24_B(this->color565);
				break;
			case Color_Rgb888:
				*r = this->red;
				*g = this->green;
				*b = this->blue;
				break;
			default:
				return;
		}
	}

	uint8 getRed(){
		switch(this->mode){
			case Color_Rgb565:
				return ConC16To24_R(this->color565);
			case Color_Rgb888:
				return this->red;
			default:
				return 0x00;
		}
	}

	void setRed(uint8 red){
		switch(this->mode){
			case Color_Rgb565:
				this->color565 = ConC24To16_R(this->color565, red);
				break;
			case Color_Rgb888:
				this->red = red;
				break;
			default:
				return;
		}
	}

	uint8 getGreen(){
		switch(this->mode){
			case Color_Rgb565:
				return ConC16To24_G(this->color565);
			case Color_Rgb888:
				return this->green;
			default:
				return 0x00;
		}
	}

	void setGreen(uint8 green){
		switch(this->mode){
			case Color_Rgb565:
				//this->color565 = ConC24To16_G(this->color565, green);
				this->color565 = ( (this->color565 & 0xF81F) | (((uint16)(green & 0xFC)) << 3) );
				break;
			case Color_Rgb888:
				this->green = green;
				break;
			default:
				return;
		}
	}

	uint8 getBlue(){
		switch(this->mode){
			case Color_Rgb565:
				return ConC16To24_B(this->color565);
			case Color_Rgb888:
				return this->blue;
			default:
				return 0x00;
		}
	}

	void setBlue(uint8 blue){
		switch(this->mode){
			case Color_Rgb565:
				this->color565 = ConC24To16_B(this->color565, blue);
				break;
			case Color_Rgb888:
				this->blue = blue;
				break;
			default:
				return;
		}
	}
};



#endif /* COLOR_H_ */