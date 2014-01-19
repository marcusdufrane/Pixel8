//TestTerminal

#ifndef _TESTTERMINAL_HPP
#define _TESTTERMINAL_HPP

#include "bbtUart.h"

struct Command
{
	//command function pointer
	void *param1;
	void *param2;
	void *param3;
	void *param4;
	void *param5;
};

#define MAX_CMD	255
char command[MAX_CMD];

// Run the terminal application
void terminalRun();

// Gets a command from the user
// The command is stored in the command buffer
void getCommand();

// Processes the command and calls the correct command from the
// command list
void processCommand();

// Add a command to the command list
// Takes a Command struct pointer
void addCommand(struct Command *command);

#endif //_TESTTERMINAL_HPP