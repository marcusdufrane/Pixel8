//Info Sender

#include <avr\io.h>
#include "utils.h"
#include <util\delay.h>

void sendLight(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{	
	sendByte(green | 0x80);
	sendByte(red | 0x80);
	sendByte(blue | 0x80);
}

void sendClearMessage()
{
	sendByte(0);
}

//bit bang for now...will need to be updated most likely
void sendByte(uint8_t data)
{
	int mask = 1;
	for(mask = 0x80; mask; mask >>= 1)
	{	
		PORTD = (data & mask)?(PORTD | DATA3):(PORTD & ~DATA3);
		
		PORTD |= CLK3;
		PORTD &= ~CLK3;
	}
}