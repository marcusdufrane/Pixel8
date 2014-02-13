//TestTerminal

#ifndef _TESTTERMINAL_HPP
#define _TESTTERMINAL_HPP

#include <stdint.h>

#define MAX_CMD_NAME 32
struct Command
{
  char name[MAX_CMD_NAME];
  uint8_t (*CommandFunction)(const char *args);
  struct Command *next;
};

// Set up the terminal
void initTerminal();

// Run the terminal application
void terminalRun();

// Add a command to the command list
// Takes a Command struct pointer
void addCommand(struct Command *command);

#endif //_TESTTERMINAL_HPP