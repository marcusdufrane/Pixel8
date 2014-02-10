//Light messaging "interface"

#include "stdint.h"

void sendLight(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

void sendClearMessage();

void sendByte(uint8_t data);
