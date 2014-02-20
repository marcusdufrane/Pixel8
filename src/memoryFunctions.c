//Memory Functions

#include "memoryFunctions.h"
#include "spi.h"
#include "avr/io.h"
#include "stdio.h"
#include "util/delay.h"

#define READ_BUFFER 16
#define MAX_SUBSECTOR 4095

void configure_memory()
{
  DDRC &= ~(1 << 0); //set Memory CS as input(Hi-Z)
}

void writeMemoryData(uint32_t address, uint8_t *data, uint16_t numOfBytes)
{  
  printf("address: %i\r\n", address);
  uint16_t counter;
  enableWrite();  
  waitForEnable();
  
  enableMemory();
  spiWrite(0x02);
  spiWrite((address & 0xFF0000) >> 16);
  spiWrite((address & 0xFF00) >> 8);
  spiWrite(address & 0xFF);
  
  for(counter = 0; counter < numOfBytes; counter++)
  {
    spiWrite(data[counter]);
  }
  disableMemory();
  waitForWIP();
}

void waitForEnable()
{  
  enableMemory();
  spiWrite(0x05);
  while(!(spiRead() & 0x02)){}
  disableMemory();
}

void readMemoryData(uint32_t address, uint8_t *data, uint16_t numOfBytes)
{
  uint16_t counter;  
  enableMemory();
  spiWrite(0x03);
  spiWrite((address & 0x00FF0000) >> 16);
  spiWrite((address & 0x0000FF00) >> 8);
  spiWrite(address & 0x000000FF);
  
  for(counter = 0; counter < numOfBytes; ++counter)
  {
    data[counter] = spiRead();
  }
  
  disableMemory();
}

void readStatus(uint8_t *status)
{
  enableMemory();
  spiWrite(0x05);
  *status = spiRead();
  #if DEBUG  
  printf("Status: %u\r\n", *status); 
  #endif
  disableMemory();
}

void readFlags(uint8_t *flags)
{
  enableMemory();
  spiWrite(0x70);
  *flags = spiRead();
  #if DEBUG  
  printf("Flags: %u\r\n", *flags); 
  #endif
  disableMemory();
}

void readMemoryId()
{
  enableMemory();
  spiWrite(0x9E);
  uint8_t i = 0;
  for(;i < 20; ++i)
  {
    printf("0x%02X\r\n", spiRead());
  }
  disableMemory();
}

void displayMemoryCommand(const char *args)
{
  uint8_t status;
  readStatus(&status);
  readFlags(&status);
  if(status & 0x01)
  {
    printf("Busy...\r\n");
    return;
  }
   
  uint32_t current;//beginning address
  uint32_t bytesToRead;
  sscanf(args, "%lu %lu", &current, &bytesToRead);  
  current &= 0xFFFFFFF0;  
  uint8_t readsLeft = (bytesToRead/16) + 1;
  uint8_t buffer[16];
  uint8_t i = 0;
  
  printf("Address    |     0    |    4     |    8     |    C\r\n");
  printf("------------------------------------------------------\r\n");
  while(readsLeft)
  {    
    uint16_t value;
    readMemoryData(current, buffer, 16);
    printf("0x%08x |", current);
    for(i = 0; i < 16; i += 4)
    {
      printf("0x%02X%02X%02X%02X ", buffer[i], buffer[i+1], buffer[i+2], buffer[i+3]);
    }
    printf("\r\n");
    current += 16;
    readsLeft--;
  } 
}

void eraseSubSector(uint32_t subsector)
{
  if(subsector > MAX_SUBSECTOR)
  {
    printf("failed: %i\r\n", subsector);
    return;
  }  
  enableWrite();
  enableMemory();
  spiWrite(0x20);
  spiWrite((subsector & 0xFF00) >> 8);
  spiWrite(subsector & 0xFF);
  spiWrite(0x00);
  disableMemory();  
}

void enableMemory()
{
  DDRC |= (1 << 0);
}

void disableMemory()
{
  DDRC &= ~(1 << 0); //set Memory CS as input(Hi-Z)
}

void enableWrite()
{
  uint8_t status;
  enableMemory();
  spiWrite(0x06);
  disableMemory();
  
  do
  {
    readStatus(&status);
  }while(~status & 0x02);
}

void waitForWIP()
{
  uint8_t status;
  do
  {
    readStatus(&status);
    #if DEBUG
    printf("waiting...\r\n");
    #endif
  }while(status & 0x01);
}

void disableWrite()
{
  spiWrite(0x04);
}

void resetEnable()
{
  spiWrite(0x66);
}

void resetDevice()
{
  spiWrite(0x99);
}

void lockMemory()
{
  enableWrite();
  enableMemory();
  spiWrite(0x01);
  spiWrite(0x3C);
  disableMemory();
  disableWrite();
}

void unlockMemory()
{
  enableMemory();
  enableWrite();
  disableMemory();
  enableMemory();
  spiWrite(0x01);
  spiWrite(0x00);
  disableMemory();
  disableWrite();
}

void eraseMemory()
{
  enableWrite();
  spiWrite(0xC7);
  disableWrite();
}