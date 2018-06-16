/*
 * Utils.h
 *
 * Created: 2017/10/22 周日 1:50:51
 *  Author: Ruilx
 */ 


#ifndef UTILS_H_
#define UTILS_H_

#include "main.h"

class Utils{
public:
	enum Direction{
		Input = 0,
		Output = 1,
	};

public:
	static inline void DA(Direction dir){
		DDRA = dir? 0xFF: 0x00;
	}

	static inline void DA(uint8 key, Direction dir){
		DDRA = dir? (DDRA | (0x01 << key)): (DDRA & ~(0x01 << key));
	}

	static inline void WA(uint8 key, bool level){
		PORTA = level? (PORTA | (0x01 << key)): (PORTA & ~(0x01 << key));
	}

	static inline void WA(uint8 data){
		PORTA = data;
	}
	// ==================================================================

	static inline void DB(Direction dir){
		DDRB = dir? 0xFF: 0x00;
	}

	static inline void DB(uint8 key, Direction dir){
		DDRB = dir? (DDRB | (0x01 << key)): (DDRB & ~(0x01 << key));
	}

	static inline void WB(uint8 key, bool level){
		PORTB = level? (PORTB | (0x01 << key)): (PORTB & ~(0x01 << key));
	}

	static inline void WB(uint8 data){
		PORTB = data;
	}
	// ==================================================================

	static inline void DC(Direction dir){
		DDRC = dir? 0xFF: 0x00;
	}

	static inline void DC(uint8 key, Direction dir){
		DDRC = dir? (DDRC | (0x01 << key)): (DDRC & ~(0x01 << key));
	}

	static inline void WC(uint8 key, bool level){
		PORTC = level? (PORTC | (0x01 << key)): (PORTC & ~(0x01 << key));
	}

	static inline void WC(uint8 data){
		PORTC = data;
	}
	// ==================================================================

	static inline void DD(Direction dir){
		DDRD = dir? 0xFF: 0x00;
	}

	static inline void DD(uint8 key, Direction dir){
		DDRD = dir? (DDRD | (0x01 << key)): (DDRD & ~(0x01 << key));
	}

	static inline void WD(uint8 key, bool level){
		PORTD = level? (PORTD | (0x01 << key)): (PORTD & ~(0x01 << key));
	}

	static inline void WD(uint8 data){
		PORTD = data;
	}
	// ==================================================================

	static inline void DE(Direction dir){
		DDRE = dir? 0xFF: 0x00;
	}

	static inline void DE(uint8 key, Direction dir){
		DDRE = dir? (DDRE | (0x01 << key)): (DDRE & ~(0x01 << key));
	}

	static inline void WE(uint8 key, bool level){
		PORTE = level? (PORTE | (0x01 << key)): (PORTE & ~(0x01 << key));
	}

	static inline void WE(uint8 data){
		PORTE = data;
	}
	// ==================================================================

	static inline void DF(Direction dir){
		DDRF = dir? 0xFF: 0x00;
	}

	static inline void DF(uint8 key, Direction dir){
		DDRF = dir? (DDRF | (0x01 << key)): (DDRF & ~(0x01 << key));
	}

	static inline void WF(uint8 key, bool level){
		PORTF = level? (PORTF | (0x01 << key)): (PORTF & ~(0x01 << key));
	}

	static inline void WF(uint8 data){
		PORTF = data;
	}

	static inline bool RF(uint8 key){
		return (bool)(PORTF & key);
	}

	// ==================================================================

	static inline void DG(Direction dir){
		DDRG = dir? 0xFF: 0x00;
	}

	static inline void DG(uint8 key, Direction dir){
		DDRG = dir? (DDRG | (0x01 << key)): (DDRG & ~(0x01 << key));
	}

	static inline void WG(uint8 key, bool level){
		PORTG = level? (PORTG | (0x01 << key)): (PORTG & ~(0x01 << key));
	}

	static inline void WG(uint8 data){
		PORTG = data;
	}
	// ==================================================================

	static inline void DH(Direction dir){
		DDRH = dir? 0xFF: 0x00;
	}

	static inline void DH(uint8 key, Direction dir){
		DDRH = dir? (DDRH | (0x01 << key)): (DDRH & ~(0x01 << key));
	}

	static inline void WH(uint8 key, bool level){
		PORTH = level? (PORTH | (0x01 << key)): (PORTH & ~(0x01 << key));
	}

	static inline void WH(uint8 data){
		PORTH = data;
	}
	// ==================================================================

	static inline void DJ(Direction dir){
		DDRJ = dir? 0xFF: 0x00;
	}

	static inline void DJ(uint8 key, Direction dir){
		DDRJ = dir? (DDRJ | (0x01 << key)): (DDRJ & ~(0x01 << key));
	}

	static inline void WJ(uint8 key, bool level){
		PORTJ = level? (PORTJ | (0x01 << key)): (PORTJ & ~(0x01 << key));
	}

	static inline void WJ(uint8 data){
		PORTJ = data;
	}
	// ==================================================================

	static inline void DK(Direction dir){
		DDRK = dir? 0xFF: 0x00;
	}

	static inline void DK(uint8 key, Direction dir){
		DDRK = dir? (DDRK | (0x01 << key)): (DDRK & ~(0x01 << key));
	}

	static inline void WK(uint8 key, bool level){
		PORTK = level? (PORTK | (0x01 << key)): (PORTK & ~(0x01 << key));
	}

	static inline void WK(uint8 data){
		PORTK = data;
	}
	// ==================================================================

	static inline void DL(Direction dir){
		DDRL = dir? 0xFF: 0x00;
	}

	static inline void DL(uint8 key, Direction dir){
		DDRL = dir? (DDRL | (0x01 << key)): (DDRL & ~(0x01 << key));
	}

	static inline void WL(uint8 key, bool level){
		PORTL = level? (PORTL | (0x01 << key)): (PORTL & ~(0x01 << key));
	}

	static inline void WL(uint8 data){
		PORTL = data;
	}
	// ==================================================================
};



#endif /* UTILS_H_ */
