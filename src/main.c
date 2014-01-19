//Motor Speed Driver

#include "stdio.h"
#include "stdint.h"
#include <avr\io.h>
#include <avr\interrupt.h>
#include <avr\wdt.h>
#include <util\delay.h>
#include "usrLed.h"
#include "timer_1.h"
#include "bbtUart.h"
#include "stdio.h"
#include "utils.h"
#include "memoryFunctions.h"
#include "TestTerminal.h"

void turnOffWatchDog() __attribute__((naked)) __attribute__((section(".init3")));

static FILE mystdout = FDEV_SETUP_STREAM(writeChar, NULL, _FDEV_SETUP_WRITE);
static FILE mystdin = FDEV_SETUP_STREAM(NULL, getChar, _FDEV_SETUP_READ);

int main()
{
	uint8_t i = 0;
	wdt_disable();
	configure_io();
	configure_10ms_timer();	
	configure_UART(115200);
	configure_spi();
	lockMemory();
	setStatus(STATUS_GOOD);
	stdout = &mystdout;
	stdin = &mystdin;
	sei();
	printf("\r\nPixel8\r\n");
	printf("A Bit Built Technology\n");		
	while(1)
	{	
		#ifdef DEBUG
		terminalRun();
		#else
		if(hasUsb() && checkAppLoad())
		{				
			writeChar('A');
			setStatus(STATUS_PROGRAMMING);
			if(receiveLightData())
			{
				setStatus(STATUS_PROG_DONE);
			}
			else
			{
				setStatus(STATUS_PROG_ERROR);
			}
			//program mode
			_delay_ms(10000);			
		}
		else
		{
			
		}
		#endif
	}
}
