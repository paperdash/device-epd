#include <Arduino.h>
#include "imagePNG.h"
#include "pngle.h"
#include "display.h"

pngle_t *pngle;

// TODO use dynamic display width
static constexpr int MAX_WIDTH = 640;
static int16_t curRowDelta[MAX_WIDTH + 1];
static int16_t nextRowDelta[MAX_WIDTH + 1];

bool dithering = true;

void on_draw(pngle_t *pngle, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t rgba[4]);

void setupImagePNG()
{
	Serial.println("setupPNG");

	Serial.println("setupPNG done");
}

void pngOpenFramebuffer()
{
	displayOpen();

	if (pngle)
	{
		pngle_destroy(pngle);
	}

	pngle = pngle_new();
	pngle_set_draw_callback(pngle, on_draw);
}

void pngWriteFramebuffer(int offset, uint8_t bitmap[], int c)
{
	if (pngle)
	{
		int fed = pngle_feed(pngle, bitmap, c);
		if (fed < 0)
		{
			Serial.println(pngle_error(pngle));
		}
	}
	else
	{
		Serial.println("forgot pngle_new() ?");
	}
}

void pngFlushFramebuffer()
{
	pngle_destroy(pngle);
	displayFlush();
}

void on_draw(pngle_t *pngle, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t rgba[4])
{
	uint8_t r = rgba[0]; // 0 - 255
	uint8_t g = rgba[1]; // 0 - 255
	uint8_t b = rgba[2]; // 0 - 255

	int16_t gray = round(r * 0.3 + g * 0.59 + b * 0.11);
	int16_t blackOrWhite;

	// Add errors to color if there are
	if (dithering)
	{
		gray += curRowDelta[x];
	}

	if (gray <= 127)
	{
		blackOrWhite = 0;
	}
	else
	{
		blackOrWhite = 255;
	}

	if (dithering)
	{
		int16_t oldPixel = gray;
		int16_t newPixel = blackOrWhite;

		int err = oldPixel - newPixel;

		if (x > 0)
		{
			nextRowDelta[x - 1] += err * 3 / 16;
		}
		nextRowDelta[x] += err * 5 / 16;
		nextRowDelta[x + 1] += err * 1 / 16;
		curRowDelta[x + 1] += err * 7 / 16;

		if (x == 0 && y > 0)
		{
			// new line
			memcpy(curRowDelta, nextRowDelta, sizeof(&curRowDelta));
			memset(nextRowDelta, 0, sizeof(&nextRowDelta));
		}
	}

	displayWritePixel(x, y, blackOrWhite);
}