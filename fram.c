#include "fram.h"

void FRAM_EnableWrite(void)
{
	FRAM_CS_LOW();

	SPI_Transfer(FRAM_WREN);

	FRAM_CS_HIGH();
}

void FRAM_DisableWrite(void)
{
	FRAM_CS_LOW();

	SPI_Transfer(FRAM_WRDI);

	FRAM_CS_HIGH();
}

unsigned char FRAM_ReadStatus(void)
{
	unsigned char stat = 0;

	FRAM_CS_LOW();

	SPI_Transfer(FRAM_RDSR);

	stat = SPI_Transfer(0x00);

	FRAM_CS_HIGH();

	return stat;
}

void FRAM_WriteStatus(unsigned char stat)
{
	FRAM_CS_LOW();

	SPI_Transfer(FRAM_WRSR);
	SPI_Transfer(stat);

	FRAM_CS_HIGH();
}

void FRAM_WriteChar(unsigned short addr, unsigned char byte, FRAMWriteType_t check)
{
	unsigned char tries = 5;

	while (tries--)
	{
		FRAM_EnableWrite();

		FRAM_CS_LOW();

		SPI_Transfer(FRAM_WRITE);

		//Address
		SPI_Transfer(addr >> 8);
		SPI_Transfer(addr & 0xFF);

		SPI_Transfer(byte);

		FRAM_CS_HIGH();

		if (check == FRAM_WRITE_AND_CHECK)
		{
			//Data is good!
			if (FRAM_ReadChar(addr) == byte)
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
}

unsigned char FRAM_ReadChar(unsigned short addr)
{
	unsigned char data = 0;

	FRAM_CS_LOW();

	SPI_Transfer(FRAM_READ);

	SPI_Transfer(addr >> 8);
	SPI_Transfer(addr & 0xFF);

	data = SPI_Transfer(0x00);

	FRAM_CS_HIGH();

	return data;
}

void FRAM_WriteData(unsigned short addr, unsigned char *data, unsigned short nBytes, FRAMWriteType_t check)
{
	unsigned char tries = 5;
	unsigned short i = 0;
	unsigned char dataOk = 0;

	while (tries--)
	{
		FRAM_EnableWrite();

		FRAM_CS_LOW();

		SPI_Transfer(FRAM_WRITE);

		SPI_Transfer(addr >> 8);
		SPI_Transfer(addr & 0xFF);

		for (i=0;i<nBytes;i++)
		{
			SPI_Transfer(data[i]);
		}

		FRAM_CS_HIGH();

		//Now, read out the data, to see if it is good..

		if (check == FRAM_WRITE_AND_CHECK)
		{

			dataOk = 1;

			FRAM_CS_LOW();

			SPI_Transfer(FRAM_READ);
			SPI_Transfer(addr >> 8);
			SPI_Transfer(addr & 0xFF);

			for (i=0;i<nBytes;i++)
			{
				if (data[i] != SPI_Transfer(0x00)) //read out next byte and compare
				{
					dataOk = 0;
					break;
				}
			}

			FRAM_CS_HIGH();

			if (dataOk == 1) //all the bytes are the same
			{
				break; //ok, break out of the 'while' loop
			}
		}
		else
		{
			break;
		}
	}
}



void FRAM_ReadData(unsigned short addr, unsigned char *data, unsigned short nBytes)
{
	unsigned short i = 0;

	FRAM_CS_LOW();

	SPI_Transfer(FRAM_READ);

	SPI_Transfer(addr >> 8);
	SPI_Transfer(addr & 0xFF);

	for (i=0;i<nBytes;i++)
	{
		data[i] = SPI_Transfer(0x00);
	}

	FRAM_CS_HIGH();
}
