//TestTerminal

#ifndef _TESTTERMINAL_HPP
#define _TESTTERMINAL_HPP

#define MAX_CMD_NAME 32
struct Command
{
  char name[MAX_CMD_NAME];
  int (*CommandFunction)(const char *args);
  struct Command *next;
};

// Run the terminal application
void terminalRun();

// Add a command to the command list
// Takes a Command struct pointer
void addCommand(struct Command *command);

#endif //_TESTTERMINAL_HPP