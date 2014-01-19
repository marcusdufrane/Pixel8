//UART Library
#ifndef _BBTUART_H
#define _BBTUART_H

#include "stdio.h"

// configure hardware settings for UART use
void configure_UART(uint32_t baud_rate);

// put a byte on the UART tx buffer
void writeChar(uint8_t value);

// wait for and receive a byte on the rx buffer
uint8_t getChar();

// check for serial comm during runtime
uint8_t checkAppLoad();

#endif //_BBTUART_H