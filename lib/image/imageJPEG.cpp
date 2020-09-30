#include <Arduino.h>
#include <SPIFFS.h>
#include <JPEGDecoder.h>
#include "imageJPEG.h"
#include "display.h"

File tmpFileBuffer;
void renderMcuBlock(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap);

static constexpr int MAX_WIDTH = 640;	  // TODO get info from display
static constexpr uint8_t BLOCK_SIZE = 16; // max MCU block size
static uint32_t blockDelta[BLOCK_SIZE * MAX_WIDTH + 1];

#define minimum(a, b) (((a) < (b)) ? (a) : (b))

void setupImageJPEG()
{
	Serial.println("setupJPEG");
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

	memset(blockDelta, 0, sizeof(blockDelta));
}

void jpegWriteFramebuffer(int offset, uint8_t bitmap[], int c)
{
	Serial.print(".");
	if (tmpFileBuffer)
	{
		tmpFileBuffer.write(bitmap, c);
	}
}

void jpegInfo()
{
	Serial.println(F("==============="));
	Serial.println(F("JPEG image info"));
	Serial.println(F("==============="));
	Serial.print(F("Width      :"));
	Serial.println(JpegDec.width);
	Serial.print(F("Height     :"));
	Serial.println(JpegDec.height);
	Serial.print(F("Components :"));
	Serial.println(JpegDec.comps);
	Serial.print(F("MCU / row  :"));
	Serial.println(JpegDec.MCUSPerRow);
	Serial.print(F("MCU / col  :"));
	Serial.println(JpegDec.MCUSPerCol);
	Serial.print(F("Scan type  :"));
	Serial.println(JpegDec.scanType);
	Serial.print(F("MCU width  :"));
	Serial.println(JpegDec.MCUWidth);
	Serial.print(F("MCU height :"));
	Serial.println(JpegDec.MCUHeight);
	Serial.println(F("==============="));
}

void renderJPEG(int xpos, int ypos)
{
	// retrieve infomration about the image
	uint16_t *pImg;
	uint16_t mcu_w = JpegDec.MCUWidth;
	uint16_t mcu_h = JpegDec.MCUHeight;
	uint32_t max_x = JpegDec.width;
	uint32_t max_y = JpegDec.height;

	// Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
	// Typically these MCUs are 16x16 pixel blocks
	// Determine the width and height of the right and bottom edge image blocks
	uint32_t min_w = minimum(mcu_w, max_x % mcu_w);
	uint32_t min_h = minimum(mcu_h, max_y % mcu_h);

	// save the current image block size
	uint32_t win_w = mcu_w;
	uint32_t win_h = mcu_h;

	// record the current time so we can measure how long it takes to draw an image
	uint32_t drawTime = millis();

	// save the coordinate of the right and bottom edges to assist image cropping
	// to the screen size
	max_x += xpos;
	max_y += ypos;

	// read each MCU block until there are no more
	while (JpegDec.read())
	{

		// save a pointer to the image block
		pImg = JpegDec.pImage;

		// calculate where the image block should be drawn on the screen
		int mcu_x = JpegDec.MCUx * mcu_w + xpos;
		int mcu_y = JpegDec.MCUy * mcu_h + ypos;

		// check if the image block size needs to be changed for the right and bottom edges
		if (mcu_x + mcu_w <= max_x)
			win_w = mcu_w;
		else
			win_w = min_w;
		if (mcu_y + mcu_h <= max_y)
			win_h = mcu_h;
		else
			win_h = min_h;

		// draw image block if it will fit on the screen
		if ((mcu_x + win_w) <= display.width() && (mcu_y + win_h) <= display.height())
		{
			renderMcuBlock(mcu_x, mcu_y, win_w, win_h, pImg);
		}

		// stop drawing blocks if the bottom of the screen has been reached
		// the abort function will close the file
		else if ((mcu_y + win_h) >= display.height())
		{
			JpegDec.abort();
		}
	}

	// calculate how long it took to draw the image
	drawTime = millis() - drawTime; // Calculate the time it took

	// print the results to the serial port
	Serial.print("Total render time was    : ");
	Serial.print(drawTime);
	Serial.println(" ms");
	Serial.println("=====================================");
}

void jpegFlushFramebuffer()
{
	if (tmpFileBuffer)
	{
		tmpFileBuffer.close();

		if (!SPIFFS.exists("/tmp.jpeg"))
		{
			Serial.println("=== /tmp.jpeg missing");
		}

		// initialise the decoder to give access to image information
		int ret = JpegDec.decodeFile("/tmp.jpeg");
		if (ret == 1)
		{
			// print information about the image to the serial port
			//jpegInfo();

			// render the image onto the screen at coordinate 0,0
			renderJPEG(0, 0);
		}
		else
		{
			Serial.println("!!!! unkown jpeg format !!!!");
		}

		// reset decoder
		JpegDec.abort();
	}
}

void renderMcuBlockPixel(uint32_t x, uint32_t y, uint32_t color)
{
	// collect all mcu blocks for current row
	uint32_t blockPageY = y - ((y / JpegDec.MCUHeight) * JpegDec.MCUHeight);
	blockDelta[(blockPageY * MAX_WIDTH) + x] = color;

	// full mcu row is complete now
	if (x == JpegDec.width - 1 && (y + 1) % JpegDec.MCUHeight == 0)
	{
		// MCU block sizes: 8x8, 16x8 or 16x16

		uint32_t originOffsetY = ((y / JpegDec.MCUHeight) * JpegDec.MCUHeight);

		for (uint16_t _y = 0; _y < JpegDec.MCUHeight; _y++)
		{
			for (uint16_t _x = 0; _x < JpegDec.width; _x++)
			{
				uint32_t originX = _x;
				uint32_t originY = originOffsetY + _y;
				uint32_t originColor = blockDelta[(_y * MAX_WIDTH) + _x];

				uint8_t r = ((((originColor >> 11) & 0x1F) * 527) + 23) >> 6;
				uint8_t g = ((((originColor >> 5) & 0x3F) * 259) + 33) >> 6;
				uint8_t b = (((originColor & 0x1F) * 527) + 23) >> 6;

				uint8_t rgba[4] = {r, g, b, 0};
				ImageProcessPixel(originX, originY, rgba);
			}
		}

		// clean buffer
		memset(blockDelta, 0, sizeof(blockDelta));
	}
}

void renderMcuBlock(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap)
{
	// Stop further decoding as image is running off bottom of screen
	if (y >= display.height())
	{
		Serial.println("y is out of display range!");
		return;
	}

	int16_t _y = y;
	for (int16_t j = 0; j < h; j++, _y++)
	{
		for (int16_t i = 0; i < w; i++)
		{
			renderMcuBlockPixel(x + i, _y, bitmap[j * w + i]);
		}
	}
}