#ifndef _FRAM_H
#define _FRAM_H

#include <spi.h> //spi.h must define spi commands (hardware dependent)
#include "hardware.h" //pin definitions (CS_FRAM, etc)

//Commands
typedef enum
{
	FRAM_WRSR = 0x01,
	FRAM_WRITE = 0x02,
	FRAM_READ = 0x03,
	FRAM_WRDI = 0x04,
	FRAM_RDSR = 0x05,
	FRAM_WREN = 0x06
} FRAMCommand_t;

typedef enum
{
	FRAM_WRITE_AND_CHECK,
	FRAM_WRITE_BLIND
} FRAMWriteType_t;

void FRAM_EnableWrite(void);
void FRAM_DisableWrite(void);

unsigned char FRAM_ReadStatus(void);
void FRAM_WriteStatus(unsigned char stat);

void FRAM_WriteChar(unsigned short addr, unsigned char byte, FRAMWriteType_t check);
unsigned char FRAM_ReadChar(unsigned short addr);

void FRAM_WriteData(unsigned short addr, unsigned char *data, unsigned short nBytes, FRAMWriteType_t check);
void FRAM_ReadData(unsigned short addr, unsigned char *data, unsigned short nBytes);

#endif
