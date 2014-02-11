
#include "FileRx.h"
#include "TestTerminal.h"
#include "memoryFunctions.h"
#include "stdio.h"
#include "stdint.h"
#include <string.h>

#define MAX_BUFFER_SIZE 1024
uint8_t downloading_flag = 0;
uint16_t buffer_size;
uint32_t data_index;

const uint8_t ack = 0x81;
const uint8_t nack = 0xFF;
const uint8_t eot = 0x55;

uint8_t rxFile(const char *args)
{
  buffer_size = 0;
  data_index = 0;
  downloading_flag = 1;
  return 0;
}

#define RX() (uint8_t)fgetc(stdin)
uint8_t runDownload()
{
  if (0 == downloading_flag)
  {
    return 0;
  }
  
  buffer_size = RX() | (RX() << 8);
  
  if (buffer_size > MAX_BUFFER_SIZE)
  {
    printf("NACK");
    return downloading_flag = 0;
  }
  else
  {
    printf("PACK");
  }
  
  char block_id[3];
  char buffer[MAX_BUFFER_SIZE];
  uint16_t crc;
  while (1)
  {
    fgets(block_id, sizeof(block_id), stdin);
    if (0 == strcmp(block_id, "EOT"))
    {
      printf("PACK");
      break;
    }
    else if (0 != strcmp(block_id, "BLK"))
    {
      printf("NACK");
      break;
    }
    
    fgets(buffer, buffer_size, stdin);
    writeMemoryData(data_index, buffer, buffer_size);
    data_index += buffer_size;
  }
  
  return downloading_flag = 0;
}

struct Command rx;

void initFileRx()
{
  strcpy(rx.name, "rx");
  rx.CommandFunction = rxFile;
  addCommand(&rx);
}


