//Memory functions
#ifndef _MEMORY_FONCTIONS_HPP
#define _MEMORY_FONCTIONS_HPP

#include "stdio.h"

#define PAGE_SIZE 256

struct MEM_ID
{
	uint8_t manufId;
	uint8_t memType;
	uint8_t memCap;
	uint8_t length;
	uint16_t extendedID;
	uint8_t customData[14];
};

void configure_memory();
void writeMemoryData(uint32_t address, uint8_t *data, uint16_t numOfBytes);
void readMemoryData(uint32_t address, uint8_t *data, uint16_t numOfBytes);
void readStatus(uint8_t *status);
void waitForWIP();
void waitForEnable();
void readMemoryId();
void enableMemory();
void disableMemory();
void enableWrite();
void disableWrite();
void lockMemory();
void unlockMemory();
void eraseMemory();
void resetEnable();
void resetDevice();
void eraseSubSector(uint32_t subsector);
void displayMemoryCommand(const char *args);

#endif //_MEMORY_FONCTIONS_HPP