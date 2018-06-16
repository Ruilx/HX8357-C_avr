/*
 * HX8357-C_320x480.cpp
 *
 * Created: 2017/10/24 ?¨ä? 0:50:35
 * Author : Ruilx
 */ 

#include <avr/io.h>
#include "hx8357_c.h"
#include "alg.h"
#include "gt20l16s1y.h"

HX8357C lcd(7, 2, 1, 0);
Alg alg;
//GT20L16S1Y rom(0, 1, 2, 3);

/*
void HX8357C::* drawPoint = &HX8357C::drawPoint;
void HX8357C::* drawHLine = &HX8357C::drawHLine;
void HX8357C::* drawVLine = &HX8357C::drawVLine;
*/

/*
 RS = PD7
 RW = PG2
 CS = PG1
 RST= PG0
*/

inline void drawPoint(uint16 x, uint16 y){
	lcd.drawPoint(x, y);
}

inline void drawHLine(uint16 x, uint16 y, uint16 length){
	lcd.drawHLine(x, y, length);
}

inline void drawVLine(uint16 x, uint16 y, uint16 length){
	lcd.drawVLine(x, y, length);
}

int main(void)
{
    /* Replace with your application code */
	alg.setDrawPointFunc(drawPoint);
	alg.setDrawHLineFunc(drawHLine);
	alg.setDrawVLineFunc(drawVLine);

	lcd.begin();
	lcd.setForeColor(255, 255, 255);
	lcd.setBackColor(0, 0, 0);
/*
	uint16 x = 0;
	uint16 y = 0;
	for(x = 0; x < 480; x+=16){
		for(y = 0; y < 320; y+=16){
			lcd.setRect(x, y, 16, 16);
			lcd.fillColor(y << 8 | x, 16, 16);
		}
	}
*/
	char str[64] = {0};
	sprintf(str, "Size: (%03d, %03d)", lcd.getLcdWidth(), lcd.getLcdHeight());
	lcd.draw6x8Str(str, 0, 0);
	lcd.draw6x8Str("[ OK ]", lcd.getLcdWidth() - 16 - 6 * 6, lcd.getLcdHeight() - 16 - 8);
	//lcd.draw6x8Str("[ OK ]", 428, 296);


	//const uint8 width = 50;
	//const uint8 height = 50;
	int16 pos[2] = { 0, 0 };
	int16 step[2] = { 2, 1 };

    while (1) 
    {
		//alg.drawEllipse(x, y, width, height);
		lcd.drawPoint(pos[0], pos[1]);
		//lcd.setRect(pos[0], pos[1], 1, 1);
		//lcd.fillColor(lcd.getForeColor(), 1, 1);
		lcd.memoryWrite(lcd.getForeColor().getRgb565(), 1);
		pos[0] += step[0];
		pos[1] += step[1];
		if(pos[0] >= lcd.getLcdWidth() -1 || pos[0] <= 0){
			step[0] = -step[0];
		}
		if(pos[1] >= lcd.getLcdHeight() -1 || pos[1] <= 0){
			step[1] = -step[1];
		}
		sprintf(str, "Pos: (%03d, %03d) Step: (%d, %d)", pos[0], pos[1], step[0], step[1]);
		lcd.draw6x8Str(str, lcd.getLcdWidth() - 16 - 48 * 6, lcd.getLcdHeight() - 16 - 8);
    }

	lcd.end();
}

//D:\arduino-1.8.2\hardware\tools\avr\bin\avrdude.exe -CD:\arduino-1.8.2\hardware\tools\avr/etc/avrdude.conf -v -patmega2560 -cwiring -PCOM3 -b115200 -D -Uflash:w:E:\My51\HX8357-C_320x480\HX8357-C_320x480\Release\HX8357-C_320x480.hex:i