#include <Arduino.h>
#include "imagePNG.h"
#include "pngle.h"
#include "display.h"

pngle_t *pngle;

void on_draw(pngle_t *pngle, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t rgba[4])
{
	ImageProcessPixel(x, y, rgba);
}

void setupImagePNG()
{
	Serial.println("setupPNG");

	pngle = pngle_new();
	pngle_set_draw_callback(pngle, on_draw);
}

void pngOpenFramebuffer()
{
	displayOpen();
	pngle_reset(pngle);
}

void pngWriteFramebuffer(int offset, uint8_t bitmap[], int c)
{
	Serial.print(".");

	int fed = pngle_feed(pngle, bitmap, c);
	if (fed < 0)
	{
		Serial.println(pngle_error(pngle));
	}
}

void pngFlushFramebuffer()
{
	Serial.println("pngFlushFramebuffer");
	pngle_reset(pngle);
}