//TestTerminal

#include "TestTerminal.h"
#include "stdio.h"

uint16_t currentPlace;

void terminalRun()
{
	currentPlace = 0;
	getCommand();
	processCommand();
}


void getCommand()
{
	uint8_t entry;
	uint8_t stillReading = 1;
	printf("pixel8->");
	while(stillReading)
	{		
		entry = (uint8_t)fgetc(stdin);
		command[currentPlace] = entry;	
		switch(entry)
		{
			case 13:
				command[currentPlace] = '\0';
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
		
		if(currentPlace == MAX_CMD)
		{
			command[MAX_CMD - 1] = '\0';
			stillReading = 0;
		}
	}		
}


void processCommand()
{
}


void addCommand(struct Command *command);