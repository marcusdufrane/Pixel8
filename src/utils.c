//Utilities

#include <avr/io.h>

void configure_io()
{	
	DDRB &= ~(1 << 0); //usb detection
	PORTB = 0;
	DDRC |= (1 << 2); //set up user led	
	PORTC = 0;
}

uint8_t hasUsb()
{
	if(PINB & 0x01)
		return 1;
	return 0;
}

uint8_t receiveLightData()
{
	return 0;
}