/*
 * alg.h
 *
 * Created: 2017/10/27 周五 13:44:33
 *  Author: Ruilx
 */ 


#ifndef ALG_H_
#define ALG_H_

#include "main.h"
#include "hx8357_c.h"
/*
typedef void(HX8357C::* DrawPoint)(uint16 x, uint16 y);
typedef void(HX8357C::* DrawHLine)(uint16 x, uint16 y, uint16 length);
typedef void(HX8357C::* DrawVLine)(uint16 x, uint16 y, uint16 length);
*/
typedef void(*DrawPoint)(uint16 x, uint16 y);
typedef void(*DrawHLine)(uint16 x, uint16 y, uint16 length);
typedef void(*DrawVLine)(uint16 x, uint16 y, uint16 length);

typedef uint16(*Max)(uint16 x, uint16 y);
typedef uint16(*Min)(uint16 x, uint16 y);

class Alg{
	DrawPoint drawPoint = NULL;
	DrawHLine drawHLine = NULL;
	DrawVLine drawVLine = NULL;

	Max max = [](uint16 a, uint16 b) -> uint16 { return a > b? a: b; };
	Min min = [](uint16 a, uint16 b) -> uint16 { return a > b? b: a; };
public:
	Alg(){

	}

	void setDrawPointFunc(DrawPoint drawPoint){
		this->drawPoint = drawPoint;
	}

	void setDrawHLineFunc(DrawHLine drawHLine){
		this->drawHLine = drawHLine;
	}

	void setDrawVLineFunc(DrawVLine drawVLine){
		this->drawVLine = drawVLine;
	}

	void drawLine(uint16 x0, uint16 y0, uint16 x1, uint16 y1){
		if(!this->drawPoint){
			return;
		}
		/*if(x0 == x1){
			if(this->drawVLine){
				this->drawVLine(x0, this->min(y0, y1), this->max(y1, y0) - this->min(y0, y1));
				return;
			}
		}else if(y0 == y1){
			if(this->drawHLine){
				this->drawHLine(this->min(x0, x1), y0, this->max(x1, x0) - this->min(x0, x1));
				return;
			}
		}*/
		uint16 dx = this->max(x1, x0) - this->min(x0, x1);
		uint16 dy = this->max(y1, y0) - this->min(y0, y1);
		int8 sx = x0 < x1? 1: -1;
		int8 sy = y0 < y1? 1: -1;
		int16 err = dx - dy;
		int16 e2 = 0;
		while(1){
			this->drawPoint(x0, y0);
			if(x0 == x1 && y0 == y1){
				break;
			}
			e2 = err << 1;
			if(e2 > -(int16)(dy)){
				err = err - dy;
				x0 = x0 + sx;
			}
			if(x0 == x1 && y0 == y1){
				this->drawPoint(x0, y0);
				break;
			}
			if(e2 < (int16)dx){
				err = err + dx;
				y0 = y0 + sy;
			}
		}
	}

	void drawEllipse(int16 xx, int16 yy, int16 w, int16 h){
		int16 x, y;
		float d1, d2;
		float a2 = (w / 2) * (w / 2);
		float b2 = (h / 2) * (h / 2);

		if(w == 0 || h == 0){ return; }
		if(w < 0){ w = -w; xx -= w; }
		if(h < 0){ h = -h; yy -= h; }
		xx += w / 2;
		yy += h / 2;
		x = 0;
		y = (uint16)(h / 2);
		d1 = b2 - a2 * ((h / 2) + 0.25);
		this->drawPoint(xx + x, yy - y);
		this->drawPoint(xx - x, yy - y);
		this->drawPoint(xx - x, yy + y);
		this->drawPoint(xx + x, yy + y);
		while(a2 * (y - 0.25) > b2 * (x + 0.5)){
			if(d1 < 0){
				d1 = d1 + b2 * (3.0 + 2 * x);
			}else{
				d1 = d1 + b2 * (3.0 + 2 * x) + 2.0 * a2 * (1 - y);
				y--;
			}
			x++;
			this->drawPoint(xx + x, yy - y);
			this->drawPoint(xx - x, yy - y);
			this->drawPoint(xx + x, yy + y);
			this->drawPoint(xx - x, yy + y);
		}
		d2 = b2 * (x + 0.5) * (x + 0.5) + a2 * (y - 1) * (y - 1) - a2 * b2;
		//d2 = (b2 * (x + 0.5)) * 2 + (a2 * (y - 1)) * 2 - (a2 * b2) * 2;
		while(y > 0){
			if(d2 < 0){
				//d2 = d2 + 2.0 * b2 * (x + 1) + a2 * (3 - 2 * y);
				d2 = d2 + b2 * (2 * x + 2) + a2 * (3 - 2 * y);
				x++;
			}else{
				d2 = d2 + a2 * (3 - 2 * y);	
			}
			y--;
			this->drawPoint(xx + x, yy - y);
			this->drawPoint(xx - x, yy - y);
			this->drawPoint(xx + x, yy + y);
			this->drawPoint(xx - x, yy + y);
		}
	}
};

#endif /* ALG_H_ */
