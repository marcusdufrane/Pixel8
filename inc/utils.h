//utils

#ifndef _UTILS_HPP
#define _UTILS_HPP

#define DATA1 0x40 //PIN D6
#define DATA2 0x20 //PIN D5
#define DATA3 0x08 //PIN D3
#define CLK1  0x80 //PIN D7
#define CLK2  0x10 //PIN D3
#define CLK3  0x04 //PIN D2

void configure_io();
uint8_t hasUsb();

#endif //_UTILS_HPP