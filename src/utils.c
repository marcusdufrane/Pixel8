//Utilities

#include <avr/io.h>
#include <avr/delay.h>
#include "utils.h"

void configure_io()
{	
	DDRB &= ~(1 << 0); //usb detection
	PORTB = 0;
	DDRC |= (1 << 2); //set up user led	
	PORTC = 0;
	//setup light comm pins
	DDRD |= 0xFC;
	PORTD &= 0x03;
	_delay_ms(5);
}

uint8_t hasUsb()
{
	if(PINB & 0x01)
		return 1;
	return 0;
}
