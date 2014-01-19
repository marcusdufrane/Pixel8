//usr led
#ifndef _USRLED_H
#define _USRLED_H

#include "stdio.h"

#define STATUS_GOOD 			1
#define STATUS_UNKONWN_ERROR 	2
#define STATUS_PROG_ERROR		3
#define STATUS_PROGRAMMING		4
#define STATUS_PROG_DONE		5
#define USR_PORT	PORTC
#define USR_LED 	(1 << 2)

struct USR_LED_STRUCT
{
	uint8_t *state;
	uint8_t current;
};

static volatile struct USR_LED_STRUCT user_led;

static uint8_t good_status[20] =      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint8_t unknown_status[20] =   {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0};
static uint8_t prog_error[20] =		  {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
static uint8_t prog_status[20] =      {1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0};
static uint8_t prog_done_status[20] = {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1};

void setStatus(uint8_t status);
void updateLED();

#endif //_USRLED_H