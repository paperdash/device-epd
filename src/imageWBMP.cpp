#include "imageWBMP.h"
#include "display.h"

uint8_t _buffer[(GxEPD2_750::WIDTH / 8) * GxEPD2_750::HEIGHT];


void setupImageWBMP()
{
	Serial.println("setupWBMP");


	Serial.println("setupWBMP done");
}

void wbmpOpenFramebuffer()
{
	Serial.println("wbmpOpenFramebuffer");
	displayOpen();
	memset(_buffer, 0, sizeof(_buffer));
}


// TODO do it better :-)
void wbmpWriteFramebuffer(int offset, uint8_t bitmap[], int c)
{
	Serial.println("wbmpWriteFramebuffer");
	for (int i = 0; i < c; i++)
	{
		_buffer[offset + i] = bitmap[i];
	}

	/*
	if (offset + sizeof(*bitmap) <= sizeof(_buffer))
	if (true)
	{
		memcpy((&_buffer) + offset, bitmap, 128);
	}
	else
	{
		Serial.println("!!!!! displayWriteFramebuffer overflow");
	}
	*/
}


void wbmpFlushFramebuffer()
{
	Serial.println("wbmpFlushFramebuffer");
	displayWriteFramebuffer(_buffer);
	displayFlush();
}