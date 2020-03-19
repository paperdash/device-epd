#include <Arduino.h>
#include <SPIFFS.h>
#include <TJpg_Decoder.h>
#include "imageJPEG.h"
#include "display.h"

File tmpFileBuffer;
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap);

void setupImageJPEG()
{
	Serial.println("setupJPEG");

	// The jpeg image can be scaled by a factor of 1, 2, 4, or 8
	TJpgDec.setJpgScale(1);

	// The decoder must be given the exact name of the rendering function above
	TJpgDec.setCallback(tft_output);
}

void jpegOpenFramebuffer()
{
	displayOpen();

	SPIFFS.remove("/tmp.jpeg");

	tmpFileBuffer = SPIFFS.open("/tmp.jpeg", FILE_WRITE);
	if (!tmpFileBuffer)
	{
		Serial.println("Failed to open file for writing");
	}
}

void jpegWriteFramebuffer(int offset, uint8_t bitmap[], int c)
{
	//Serial.println("jpegWriteFramebuffer");
	if (tmpFileBuffer)
	{
		tmpFileBuffer.write(bitmap, c);
	}
}

void jpegFlushFramebuffer()
{
	//Serial.println("jpegFlushFramebuffer");
	if (tmpFileBuffer)
	{
		tmpFileBuffer.close();
		TJpgDec.drawFsJpg(0, 0, "/tmp.jpeg");
	}
}

// This next function will be called during decoding of the jpeg file to
// render each block to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap)
{
	if (true)
	{
		Serial.print("tft_output: x = ");
		Serial.print(x);
		Serial.print(" y = ");
		Serial.print(y);
		Serial.print(" w = ");
		Serial.print(w);
		Serial.print(" h = ");
		Serial.println(h);
	}

	// Stop further decoding as image is running off bottom of screen
	if (y >= display.height())
		return 0;

	// This might work instead if you adapt the sketch to use the Adafruit_GFX library
	display.drawRGBBitmap(x, y, bitmap, w, h);

	// Return 1 to decode next block
	return 1;
}