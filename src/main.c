//Motor Speed Driver

#include "stdio.h"
#include "stdint.h"
#include <stdlib.h>
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
#include "InfoSender.h"
#include "spi.h"
#include <string.h>
#include "FileRx.h"

void turnOffWatchDog() __attribute__((naked)) __attribute__((section(".init3")));

static FILE mystdout = FDEV_SETUP_STREAM(writeChar, NULL, _FDEV_SETUP_WRITE);
static FILE mystdin = FDEV_SETUP_STREAM(NULL, getChar, _FDEV_SETUP_READ);

typedef struct task 
{
  unsigned char running; // 1 indicates task is running
  int state;         // Current state of state machine
  unsigned long period; // Rate at which the task should tick
  unsigned long elapsedTime; // Time since task's previous tick
  int (*TickFct)(int); // Function to call for task's tick
} task;

task tasks[3];

const unsigned char tasksNum = 4;
const unsigned long tasksPeriodGCD = 10;
const unsigned long period1 = 10;
const unsigned long period2 = 100;
const unsigned long period3 = 100;

int TickFct_1(int state);
int TickFct_2(int state);
int TickFct_3(int state);
unsigned char runningTasks[4] = {255}; // Track running tasks, [0] always idleTask
const unsigned long idleTask = 255; // 0 highest priority, 255 lowest
unsigned char currentTask = 0; // Index of highest priority task in runningTasks

unsigned schedule_time = 0;
ISR(TIMER1_COMPA_vect) 
{
  unsigned char i;
  for (i=0; i < tasksNum; ++i) 
  { // Heart of scheduler code
    if (  (tasks[i].elapsedTime >= tasks[i].period) // Task ready
      && (runningTasks[currentTask] > i) // Task priority > current task priority
      && (!tasks[i].running) // Task not already running (no self-preemption)
      ) 
    { 
      SREG &= 0x7F;
      tasks[i].elapsedTime = 0; // Reset time since last tick
      tasks[i].running = 1; // Mark as running
      currentTask += 1;
      runningTasks[currentTask] = i; // Add to runningTasks
      SREG |= 0x80;
      
      tasks[i].state = tasks[i].TickFct(tasks[i].state); // Execute tick
      
      SREG &= 0x7F;
      tasks[i].running = 0; // Mark as not running
      runningTasks[currentTask] = idleTask; // Remove from runningTasks
      currentTask -= 1;
      SREG |= 0x80;
    }
    tasks[i].elapsedTime += tasksPeriodGCD;
  }
}

void init_processor() 
{
  // copied from http://www.cs.ucr.edu/~vahid/rios/rios_avr.htm
  /*Set up timer*/
  TCCR1B = (1<<WGM12)|(1<<CS11); // CTC mode (clear timer on compare). Prescaler=8
  // AVR output compare register OCR0.
  OCR1A = 18480;
  // AVR timer interrupt mask register         
  TIMSK1 = (1<<OCIE1A); //enables compare match interrupt
  TCNT1 = 0;
      
  /*Enable global interrupts*/
  SREG |= 0x80;
  
  // from configure_10ms_timer, no idea what this does
  //TCCR0A = 0x02;
  //TCCR0B = 0x00;
  //TIMSK0 = 0x02;
  ////Prescalar - 1024 = .0000694s
  ////Target Timer Count = 144 (10ms)
  //
  //OCR0A = 144;
  //TCCR0B |= 0x05;
}

uint8_t testCommand(const char *args)
{
  printf("i am called with '%s'\r\n", args);
  return 0;
}

uint8_t value = 0x01;
uint8_t lightsCommand(const char *args)
{
  if (strlen(args))
  {
    value = atoi(args);
  }
  else
  {
    printf("current lights value: %d\r\n", (int)value);
  }
  return 0;
}

int main()
{
  init_processor();
  wdt_disable();
  configure_io();
  configure_UART(115200);
  configure_spi();
  lockMemory();
  setStatus(STATUS_GOOD);
  stdout = &mystdout;
  stdin = &mystdin;
  initFileRx();
  printf("\r\nPixel8\r\n");
  printf("A Bit Built Technology\r\n");
  
  PORTD &= ~DATA3;

  // Priority assigned to lower position tasks in array
  unsigned char i = 0;
  tasks[i].state = -1;
  tasks[i].period = period1;
  tasks[i].elapsedTime = tasks[i].period;
  tasks[i].running = 0;
  tasks[i].TickFct = &TickFct_1;
  ++i;
  tasks[i].state = -1;
  tasks[i].period = period2;
  tasks[i].elapsedTime = tasks[i].period;
  tasks[i].running = 0;
  tasks[i].TickFct = &TickFct_2;
  ++i;
  tasks[i].state = -1;
  tasks[i].period = period3;
  tasks[i].elapsedTime = tasks[i].period;
  tasks[i].running = 0;
  tasks[i].TickFct = &TickFct_3;
  
  struct Command test;
  strcpy(test.name, "test");
  test.CommandFunction = testCommand;
  addCommand(&test);
  
  struct Command lights;
  strcpy(lights.name, "lights");
  lights.CommandFunction = lightsCommand;
  addCommand(&lights);
  
  while(1)
  {
  }
}

// Light strip task
int lightstrip_counter = 0;
int TickFct_1(int state)
{
  if (lightstrip_counter == 0)
  {
    sendLight(value, 0, 0, 0);
    sendLight(0, value, 0, 0);
    sendLight(0, 0, value, 0);
    sendLight(value, 0, 0, 0);
    sendLight(0, value, 0, 0);
    sendLight(0, 0, value, 0);
    sendLight(value, 0, 0, 0);
    sendLight(0, value, 0, 0);
    sendLight(0, 0, value, 0);
    sendLight(value, 0, 0, 0);
    sendClearMessage();
  } 
  else if (lightstrip_counter == 50)
  {
    sendLight(0, 0, value, 0);
    sendLight(value, 0, 0, 0);
    sendLight(0, value, 0, 0);
    sendLight(0, 0, value, 0);
    sendLight(value, 0, 0, 0);
    sendLight(0, value, 0, 0);
    sendLight(0, 0, value, 0);
    sendLight(value, 0, 0, 0);
    sendLight(0, value, 0, 0);
    sendLight(0, 0, value, 0);
    
    sendClearMessage();
  }
  else if (lightstrip_counter == 100)
  {
    sendLight(0, value, 0, 0);
    sendLight(0, 0, value, 0);
    sendLight(value, 0, 0, 0);
    sendLight(0, value, 0, 0);
    sendLight(0, 0, value, 0);
    sendLight(value, 0, 0, 0);
    sendLight(0, value, 0, 0);
    sendLight(0, 0, value, 0);
    sendLight(value, 0, 0, 0);
    sendLight(0, value, 0, 0);
    
    sendClearMessage();
  } 
  
  if (++lightstrip_counter >= 150)
  {
    lightstrip_counter = 0;
  }

  return 0;
}

// LED task
int TickFct_2(int state)
{
  updateLED();
  return 0;
}

// terminal task
int TickFct_3(int state)
{
  if (!runDownload())
  {
    terminalRun();
  }

  return 0;
}
