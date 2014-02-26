
#include "FileRx.h"
#include "TestTerminal.h"
#include "memoryFunctions.h"
#include "stdio.h"
#include "stdint.h"
#include <string.h>
#include <util/crc16.h>

#define BUFFER_SIZE 256
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

uint8_t calcCrc(const char *args)
{
  uint8_t data_buffer[BUFFER_SIZE];
  int data_bytes = 0;
  int i,j = 0;
  
  for (i = 0; i < strlen(args); ++i)
  {
    if (args[i] <= 0x39 && args[i] >= 0x30)
    {
      data_buffer[data_bytes] = args[i] - 0x30;
      ++data_bytes;
    }
    else if (args[i] <= 0x46 && args[i] >= 0x41)
    {
      data_buffer[data_bytes] = args[i] - 0x31;
      ++data_bytes;
    }
    
    if (data_bytes > BUFFER_SIZE)
    {
      printf("too many characters\r\n");
      return 1;
    }
  }
  
  printf("data_buffer size %d:", data_bytes);
  for (i = 0; i < data_bytes; ++i)
  {
    printf(" 0x%X", data_buffer[i]);
  }
  printf("\r\n");
  
  writePage(0, data_buffer, data_bytes);
  uint8_t read_buffer[BUFFER_SIZE];
  //readMemoryData(0, read_buffer, BUFFER_SIZE);
  //
  //printf("read back:");
  //for (int i = 0; i < BUFFER_SIZE; ++i)
  //{
  //  printf("0x%X ", read_buffer[i]);
  //}
  //printf("\r\n");
  
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

  if (buffer_size > BUFFER_SIZE)
  {
    printf("NACK");
    return downloading_flag = 0;
  }
  else
  {
    printf("PACK");
  }
  
  char block_id[4] = {'\0'};
  char buffer[BUFFER_SIZE];
  uint16_t crc = 0xFFFF;
  
  data_index = 0;
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
    
    // fgets returns (n+1) bytes.  since we don't care about enforcing a
    // trailing '\0', just write right up to the end of the buffer
    fgets(buffer, buffer_size + 1, stdin);
    
    // then get the two-byte crc
    uint16_t actual_crc = RX() | (RX() << 8);
    
    //crc = 0xFFFF;
    //for (int i = 0; i < buffer_size; ++i)
    //{
    //  crc = _crc_ccitt_update(crc, buffer[i]);
    //}
    //
    //if (0 != crc)
    //{
    //  printf("NACK");
    //  break;
    //}

    writePage(data_index, buffer);
    data_index += 1;
    
    printf("PACK");
  }
  
  return downloading_flag = 0;
}

struct Command rx;
struct Command crc;

void initFileRx()
{
  strcpy(rx.name, "rx");
  rx.CommandFunction = rxFile;
  addCommand(&rx);
  
  strcpy(crc.name, "crc");
  crc.CommandFunction = calcCrc;
  addCommand(&crc);
}


