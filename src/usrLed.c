#include "usrLed.h"
#include <avr\io.h>


void setStatus(uint8_t status)
{
	switch(status)
	{
		case STATUS_GOOD:
			user_led.state = good_status;
			user_led.current = 0;
			break;
		default:
		case STATUS_UNKONWN_ERROR:
			user_led.state = unknown_status;
			user_led.current = 0;
			break;
		case STATUS_PROG_ERROR:
			user_led.state = prog_error;
			user_led.current = 0;
			break;
		case STATUS_PROGRAMMING:
			user_led.state = prog_status;
			user_led.current = 0;
			break;
		case STATUS_PROG_DONE:
			user_led.state = prog_done_status;
			user_led.current = 0;
			break;
	}
}

void updateLED()
{
	if(user_led.state[user_led.current])
	{
		USR_PORT |= USR_LED;
	}
	else
	{
		USR_PORT &= ~USR_LED;
	}
	user_led.current++;
	if(user_led.current >= 20)
	{
		user_led.current = 0;
	}
}
