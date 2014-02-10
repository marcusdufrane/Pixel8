//Timer 1

#include "timer_1.h"
#include "usrLed.h"
#include <avr\io.h>
#include <avr\interrupt.h>

//void configure_10ms_timer()
//{
//	TCCR0A = 0x02;
//	TCCR0B = 0x00;
//	TIMSK0 = 0x02;
//	//Prescalar - 1024 = .0000694s
//	//Target Timer Count = 144 (10ms)
//	
//	OCR0A = 144;	
//	TCCR0B |= 0x05;	
//}
//
//ISR(TIMER0_COMPA_vect)
//{
//	timer_100ms_counter++;
//	if(timer_100ms_counter > 10)
//	{
//		updateLED();
//		timer_100ms_counter = 0;
//	}
//}