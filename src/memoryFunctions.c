//Memory Functions

#include "memoryFunctions.h"
#include "spi.h"
#include "avr/io.h"
#include "stdio.h"

#define READ_BUFFER 16

void configure_memory()
{
  DDRC &= ~(1 << 0); //set Memory CS as input(Hi-Z)
}

void writeMemoryData(uint32_t address, uint8_t *data, uint16_t numOfBytes)
{
  uint8_t addressBytes[4];
  uint16_t counter;
  addressBytes[1] = (address & 0x00FF0000) >> 16;
  addressBytes[2] = (address & 0x0000FF00) >> 8;
  addressBytes[3] = (address & 0x000000FF);  
  enableWrite();
  enableMemory();
  spiWrite(0x02);
  spiWrite(addressBytes[1]);
  spiWrite(addressBytes[2]);
  spiWrite(addressBytes[3]);
  for(counter = 0; counter < numOfBytes; counter++)
  {
    spiWrite(data[counter]);
  }
  disableMemory();
}

void readMemoryData(uint32_t address, uint8_t *data, uint16_t numOfBytes)
{
  uint8_t addressBytes[4];
  uint16_t counter;
  addressBytes[1] = (address & 0x00FF0000) >> 16;
  addressBytes[2] = (address & 0x0000FF00) >> 8;
  addressBytes[3] = (address & 0x000000FF);
  enableMemory();
  spiWrite(0x03);
  spiWrite(addressBytes[1]);
  spiWrite(addressBytes[2]);
  spiWrite(addressBytes[3]);
  for(counter = 0; counter < numOfBytes; counter++)
  {
    data[counter] = spiRead();
  }
  disableMemory();
}

void readMemoryId(uint8_t *data)
{

}

void enableMemory()
{
  DDRC |= (1 << 0);
}

void displayMemoryCommand(const char *args)
{
  uint32_t current;//beginning address
  uint32_t bytesToRead;
  sscanf(args, "%u %u", &current, &bytesToRead);
  if(0xFFFFFFFF - current < bytesToRead)
    bytesToRead = 0xFFFFFFFF - current;  
  current &= 0xFFFFFF00;
  uint8_t readsLeft = (bytesToRead/READ_BUFFER) + 1;
  uint8_t buffer[READ_BUFFER];
  uint8_t i = 0;
  
  printf("Address    |     0    |    4     |    8     |    C\r\n");
  printf("------------------------------------------------------\r\n");
  while(readsLeft)
  {    
    readMemoryData(current, buffer, READ_BUFFER);
    printf("0x%08x |", current);
    for(i = 0; i < READ_BUFFER; i += 4)
    {
      printf("0x%08x ", ((uint32_t)(buffer[i] << 24) + (buffer[i+2] << 16) + (buffer[i+1] << 8) + buffer[i+3]));
    }
    printf("\r\n");
    current += 16;
    readsLeft--;
  } 
}

void disableMemory()
{
  DDRC &= ~(1 << 0); //set Memory CS as input(Hi-Z)
}

void enableWrite()
{
  enableMemory();
  spiWrite(0x06);
  disableMemory();
}

void disableWrite()
{
  enableMemory();
  spiWrite(0x04);
  disableMemory();
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
  enableWrite();
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