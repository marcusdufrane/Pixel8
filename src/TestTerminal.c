//TestTerminal

#include "TestTerminal.h"
#include "stdio.h"
#include <string.h>

struct Command *head;

void addCommand(struct Command *command)
{
  if (NULL == head)
  {
    // initial command
    head = command;
    head->next = NULL;
  }
  else
  {
    struct Command *current = head;
    while (current->next != NULL)
    {
      current = current->next;
    }
    current->next = command;
    current->next->next = NULL;
  }
}

#define MAX_CMD_STRING 255
char command_string[MAX_CMD_STRING];

uint16_t currentPlace;
void getCommand()
{
  uint8_t entry;
  uint8_t stillReading = 1;
  printf("pixel8->");
  while(stillReading)
  {    
    entry = (uint8_t)fgetc(stdin);
    command_string[currentPlace] = entry;  
    switch(entry)
    {
      case 13:
        command_string[currentPlace] = '\0';
        printf("\r\n");
        stillReading = 0;
        break;
      case 8:
        if(currentPlace > 0)
        {
          fputc(entry, stdout);
          currentPlace--;
        }
        
        break;
      default:
        if(entry >= 32 && entry < 127)
        {
          fputc(entry, stdout);
          currentPlace++;
        }
        break;
    }
    
    if(currentPlace == MAX_CMD_STRING)
    {
      command_string[MAX_CMD_STRING - 1] = '\0';
      stillReading = 0;
    }
  }    
}

void processCommand()
{
  char command_name[MAX_CMD_STRING] = {'\0'};
  int end_of_command = strcspn(command_string, " \0");
  strncpy(command_name, command_string, end_of_command);
  
  // look through each command, starting with head
  struct Command *current = head;
  while (current != NULL)
  {
    //printf("checking '%s'...\r\n", current->name);
    if (0 == strcmp(current->name, command_name))
    {
      if (command_string[end_of_command] == '\0')
      {
        current->CommandFunction('\0');
      }
      else
      {
        current->CommandFunction(&command_string[end_of_command + 1]);
      }
      return;
    }
    current = current->next;
  }
}

void terminalRun()
{
  currentPlace = 0;
  getCommand();
  processCommand();
}
