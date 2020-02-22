#include <Arduino.h>
#include "imagePNG.h"
#include "pngle.h"
#include "SPIFFS.h"
#include "display.h"

pngle_t *pngle;

void on_draw(pngle_t *pngle, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t rgba[4]);

void setupImagePNG()
{
	Serial.println("setupPNG");

	Serial.println("setupPNG done");
}

void pngOpenFramebuffer()
{
	//Serial.println("pngOpenFramebuffer");
	displayOpen();

	pngle = pngle_new();
	pngle_set_draw_callback(pngle, on_draw);
}

void pngWriteFramebuffer(int offset, uint8_t bitmap[], int c)
{
	int fed = pngle_feed(pngle, bitmap, c);
	if (fed < 0)
	{
		Serial.println(pngle_error(pngle));
	}
}

void pngFlushFramebuffer()
{
	//Serial.println("pngFlushFramebuffer");
	pngle_destroy(pngle);
	displayFlush();
/*
	SPIFFS.begin();
	displayOpen();

	pngle_t *pngle = pngle_new();
	pngle_set_draw_callback(pngle, on_draw);

	File file = SPIFFS.open("/blackPNG.png", "r");
	if (!file)
	{
		Serial.println(" file not found");
	}

	// get filesize
	long size = file.size();

	// read contents of the file into the vector
	char *buffer = (char *)malloc((unsigned long)size);
	if (!buffer)
	{
		file.close();
		Serial.println(" allow failedd");
	}
	file.readBytes(buffer, (size_t)size);
	file.close();

	Serial.println("  read png");
	int ret = pngle_feed(pngle, buffer, size);

	//Serial.println("  return: " + ret);
	//Serial.println("  width: " + pngle_get_width(pngle));
	//Serial.println("  height: " + pngle_get_height(pngle));

	Serial.println("  read png done");
	pngle_destroy(pngle);

	displayFlush();
	*/
}
/*
void setupImagePNG__demo()
{
	SPIFFS.begin();
	displayOpen();

	pngle_t *pngle = pngle_new();
	pngle_set_draw_callback(pngle, on_draw);

	File file = SPIFFS.open("/blackPNG.png", "r");
	if (!file)
	{
		Serial.println(" file not found");
	}

	// get filesize
	long size = file.size();

	// read contents of the file into the vector
	char *buffer = (char *)malloc((unsigned long)size);
	if (!buffer)
	{
		file.close();
		Serial.println(" allow failedd");
	}
	file.readBytes(buffer, (size_t)size);
	file.close();

	Serial.println("  read png");
	int ret = pngle_feed(pngle, buffer, size);

	//Serial.println("  return: " + ret);
	//Serial.println("  width: " + pngle_get_width(pngle));
	//Serial.println("  height: " + pngle_get_height(pngle));

	Serial.println("  read png done");
	pngle_destroy(pngle);

	displayFlush();
}
*/

void on_draw(pngle_t *pngle, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t rgba[4])
{
	uint8_t r = rgba[0]; // 0 - 255
	//uint8_t g = rgba[1]; // 0 - 255
	//uint8_t b = rgba[2]; // 0 - 255
	uint8_t a = rgba[3]; // 0: fully transparent, 255: fully opaque

	if (a)
	{
		// 640 x 384
		if (true) // x == 0
		{
			//printf("put pixel at (%d, %d) with color #%02x%02x%02x\n", x, y, r, g, b);
		}

		displayWritePixel(x, y, r);
	}
}