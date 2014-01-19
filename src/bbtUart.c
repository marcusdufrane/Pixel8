//Bit Built Tech ATMEGA UART Library

#include "bbtUart.h"
#include <avr/io.h>
#include <stdio.h>

void configure_UART(uint32_t baud_rate)
{
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
	UBRR0 = (14745600 / (baud_rate * 16UL)) - 1;	
}

void writeChar(uint8_t value)
{
	while ((UCSR0A & (1 << UDRE0)) == 0) {};
	UDR0 = value;
}

uint8_t getChar()
{
	uint8_t tmp;
	while ((UCSR0A & (1 << RXC0)) == 0) {}; 
    tmp = UDR0; 
	return tmp;
}

uint8_t checkAppLoad()
{
	if(UCSR0A & (1 << RXC0))
	{
		if(getChar() == 'A')
		{
			return 1;
		}
	}
	return 0;
}