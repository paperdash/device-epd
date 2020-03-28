#include <Arduino.h>
#include <SPIFFS.h>
#include <TJpg_Decoder.h>
#include "imageJPEG.h"
#include "display.h"

File tmpFileBuffer;
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap);

static constexpr int MAX_WIDTH = 640;
static constexpr uint8_t BLOCK_SIZE = 15;
static uint8_t blockDelta[BLOCK_SIZE * MAX_WIDTH + 1];

static int16_t curRowDeltaJ[MAX_WIDTH + 1];

unsigned int pixelCount = 0;

void setupImageJPEG()
{
	Serial.println("setupJPEG");

	// The jpeg image can be scaled by a factor of 1, 2, 4, or 8
	TJpgDec.setJpgScale(1);

	// The decoder must be given the exact name of the rendering function above
	TJpgDec.setCallback(tft_output);

	memset(blockDelta, 0, sizeof(blockDelta));
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

	pixelCount = 0;
}

void jpegWriteFramebuffer(int offset, uint8_t bitmap[], int c)
{
	Serial.print(".");
	if (tmpFileBuffer)
	{
		tmpFileBuffer.write(bitmap, c);
	}
}

void jpegFlushFramebuffer()
{
	if (tmpFileBuffer)
	{
		tmpFileBuffer.close();

		uint16_t w = 0, h = 0;
		TJpgDec.getFsJpgSize(&w, &h, "/tmp.jpeg");
		Serial.print("Image size: ");
		Serial.print(w);
		Serial.print("x");
		Serial.println(h);
		TJpgDec.drawFsJpg(0, 0, "/tmp.jpeg");
	}
}

void draw_XXX(uint32_t x, uint32_t y, uint8_t color)
{
	//uint8_t r = ((color >> 11) & 0x1F);
	//uint8_t g = ((color >> 5) & 0x3F);
	//uint8_t b = (color & 0x1F);
	/*
	uint8_t r = ((((color >> 11) & 0x1F) * 527) + 23) >> 6;
	uint8_t g = ((((color >> 5) & 0x3F) * 259) + 33) >> 6;
	uint8_t b = (((color & 0x1F) * 527) + 23) >> 6;
	*/
	uint32_t blue = color & 0x001F;	 // 5 bits blue
	uint32_t green = color & 0x07E0; // 6 bits green
	uint32_t red = color & 0xF800;	 // 5 bits red

	int16_t gray = round(red * 0.3 + green * 0.59 + blue * 0.11);
	int16_t blackOrWhite;

	// Add errors to color if there are
	if (ImageProcess.dithering)
	{
		gray += curRowDeltaJ[x];
	}

	if (gray <= 127)
	{
		blackOrWhite = 0;
	}
	else
	{
		blackOrWhite = 255;
	}

	displayWritePixel(ImageProcess.x + x, ImageProcess.y + y, blackOrWhite);
}

void on_drawPixel(uint32_t x, uint32_t y, uint32_t color)
{
	pixelCount++;

	uint32_t blockPageY = y - ((y / 16) * 16);
	blockDelta[(blockPageY * MAX_WIDTH) + x] = color;

	// TODO bessere lösung finden !
	if (pixelCount == 10240)
	{
		// new block
		uint32_t originOffsetY = ((y / 16) * 16);
		//Serial.println("new block");

		for (uint16_t _y = 0; _y <= BLOCK_SIZE; _y++)
		{
			for (uint16_t _x = 0; _x < MAX_WIDTH; _x++)
			{
				uint32_t originX = _x;
				uint32_t originY = originOffsetY + _y + (blockPageY * 16);
				uint8_t originColor = blockDelta[(_y * MAX_WIDTH) + _x];
				//draw_XXX(originX, originY, originColor);

				uint8_t blue = originColor & 0x001F;  // 5 bits blue
				uint8_t green = originColor & 0x07E0; // 6 bits green
				uint8_t red = originColor & 0xF800;	  // 5 bits red

				if (false && originX <= 70)
				{
					//uint8_t r = ((((originColor >> 11) & 0x1F) * 527) + 23) >> 6;
					//uint8_t g = ((((originColor >> 5) & 0x3F) * 259) + 33) >> 6;
					//uint8_t b = (((originColor & 0x1F) * 527) + 23) >> 6;
					uint8_t r = red;
					uint8_t g = green;
					uint8_t b = blue;

					Serial.print("Pixel @ y: ");
					Serial.print(y);
					Serial.print(" x: ");
					Serial.print(x);

					Serial.print("  Color, R:");
					Serial.print(r);
					Serial.print(", G:");
					Serial.print(g);
					Serial.print(", B:");
					Serial.println(b);
				}

				uint8_t rgba[4] = {red, green, blue, 0};
				ImageProcessPixel(originX, originY, rgba);
			}
		}

		// clean buffer
		memset(blockDelta, 0, sizeof(blockDelta));

		pixelCount = 0;
	}
}

// This next function will be called during decoding of the jpeg file to
// render each block to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap)
{
	if (false && y == 0)
	{
		//tft_output: x = 0 y = 0 w = 16 h = 16
		//tft_output: x = 16 y = 0 w = 16 h = 16
		//tft_output: x = 384 y = 0 w = 6 h = 16
		//tft_output: x = 0 y = 16 w = 16 h = 16
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
	{
		Serial.println("y is out of display range!");
		return 0;
	}

	// This might work instead if you adapt the sketch to use the Adafruit_GFX library
	display.drawRGBBitmap(x, y, bitmap, w, h);

	int16_t _y = y;
	for (int16_t j = 0; j < h; j++, _y++)
	{
		for (int16_t i = 0; i < w; i++)
		{
			// geht richtig
			//display.writePixel(x + i, y, bitmap[j * w + i]);

			if (_y == 0 && x + i <= 70)
			{
				uint32_t originColor = bitmap[j * w + i];

				uint8_t blue = originColor & 0x001F;  // 5 bits blue
				uint8_t green = originColor & 0x07E0; // 6 bits green
				uint8_t red = originColor & 0xF800;	  // 5 bits red

				//uint8_t r = ((((originColor >> 11) & 0x1F) * 527) + 23) >> 6;
				//uint8_t g = ((((originColor >> 5) & 0x3F) * 259) + 33) >> 6;
				//uint8_t b = (((originColor & 0x1F) * 527) + 23) >> 6;
				uint8_t r = red;
				uint8_t g = green;
				uint8_t b = blue;

				Serial.print("Pixel @ y: ");
				Serial.print(_y);
				Serial.print(" x: ");
				Serial.print(x + i);

				Serial.print("  Color, R:");
				Serial.print(r);
				Serial.print(", G:");
				Serial.print(g);
				Serial.print(", B:");
				Serial.println(b);
			}

			on_drawPixel(x + i, _y, bitmap[j * w + i]);
		}
	}
	if (false && y == 0)
	{
		Serial.println(pixelCount);
	}

	// Return 1 to decode next block
	return 1;
}