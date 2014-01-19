//SPI Comm
#ifndef _SPI_H
#define _SPI_H

#include "stdio.h"

void configure_spi();
uint8_t spiRead();
uint8_t spiWrite(uint8_t byte);

#endif //_SPI_H