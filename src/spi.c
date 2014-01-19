//SPI Comm

#include "spi.h"
#include "avr/io.h"
#include "util/delay.h"

void configure_spi()
{
	uint8_t tmp;
	DDRB |= (1 << 2); //configure SS as output		
	DDRB |= (1 << 3) | (1 << 5);	
	PORTC &= ~(1 << 0);
	SPCR |= (1 << SPE) | (1 << MSTR);
	tmp = SPSR;
	tmp = SPDR;
}

inline uint8_t spiRead()
{
	spiWrite(0x00);
	while(!SPSR & (1 << SPIF));
	uint8_t status = SPSR;
	return SPDR;
}

inline uint8_t spiWrite(uint8_t byte)
{	
	SPDR = byte;
	while(!(SPSR & (1 << SPIF)));
}
