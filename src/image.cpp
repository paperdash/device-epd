#include "image.h"
#include "imageWBMP.h"
#include "imagePNG.h"
#include "imageJPEG.h"
#include "display.h"

structImageProcess ImageProcess;
// TODO use dynamic display width
static constexpr int MAX_WIDTH = 640;
static int16_t curRowDelta[MAX_WIDTH + 1];
static int16_t nextRowDelta[MAX_WIDTH + 1];

void setupImage()
{
	setupImagePNG();
	setupImageJPEG();
}

void ImageNew(int x, int y, int w, int h, bool dithering)
{
	Serial.printf("ImageNew: x: %d, y: %d, dithering: %d \n", x, y, dithering);

	ImageProcess.x = x;
	ImageProcess.y = y;
	ImageProcess.w = w;
	ImageProcess.h = h;
	ImageProcess.dithering = dithering;

	memset(curRowDelta, 0, sizeof(curRowDelta));
	memset(nextRowDelta, 0, sizeof(nextRowDelta));
}

void ImageWriteBuffer(uint8_t buff[], size_t c)
{
	// initial detect format
	if (ImageProcess.format == 0)
	{
		if (memcmp(buff, ImageHeaderWBMP, sizeof(ImageHeaderWBMP) - 1) == 0)
		{
			Serial.println(" image format: WBMP");
			ImageProcess.format = 2;

			wbmpOpenFramebuffer();
		}
		else if (memcmp(buff, ImageHeaderPNG, sizeof(ImageHeaderPNG) - 1) == 0)
		{
			Serial.println(" image format: PNG");
			ImageProcess.format = 3;

			pngOpenFramebuffer();
		}
		else if (memcmp(buff, ImageHeaderJPEG, sizeof(ImageHeaderJPEG) - 1) == 0)
		{
			Serial.println(" image format: JPEG");
			ImageProcess.format = 4;

			jpegOpenFramebuffer();
		}
		else
		{
			ImageProcess.format = 1;
			Serial.println(" unkown image format. first header are:");
			Serial.println(buff[0]);
			Serial.println(buff[1]);
			Serial.println(buff[2]);
			Serial.println(buff[3]);
			Serial.println(buff[4]);
			Serial.println(buff[5]);
		}
	}

	// write display frame
	switch (ImageProcess.format)
	{
	// WBMP
	case 2:
		wbmpWriteFramebuffer(0, buff, c);
		break;
	// PNG
	case 3:
		pngWriteFramebuffer(0, buff, c);
		break;
	// JPEG
	case 4:
		jpegWriteFramebuffer(0, buff, c);
		break;
	}
}

void ImageFlushBuffer()
{
	// update display
	switch (ImageProcess.format)
	{
	// WBMP
	case 2:
		wbmpFlushFramebuffer();
		break;
	// PNG
	case 3:
		pngFlushFramebuffer();
		break;
	// JPEG
	case 4:
		jpegFlushFramebuffer();
		break;
	}

	// clear settings
	ImageProcess.format = 0;
	ImageProcess.x = 0;
	ImageProcess.y = 0;
	ImageProcess.w = 0;
	ImageProcess.h = 0;
	ImageProcess.dithering = false;
}

void ImageProcessPixel(uint32_t x, uint32_t y, uint8_t rgba[4])
{
	//uint32_t b = color & 0x001F; // 5 bits blue
	//uint32_t g = color & 0x07E0; // 6 bits green
	//uint32_t r = color & 0xF800; // 5 bits red
	uint8_t r = rgba[0]; // 0 - 255
	uint8_t g = rgba[1]; // 0 - 255
	uint8_t b = rgba[2]; // 0 - 255

	int16_t gray = round(r * 0.3 + g * 0.59 + b * 0.11);
	int16_t blackOrWhite;

	// Add errors to color if there are
	if (ImageProcess.dithering)
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

	if (ImageProcess.dithering)
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
			memcpy(curRowDelta, nextRowDelta, sizeof(curRowDelta));
			memset(nextRowDelta, 0, sizeof(nextRowDelta));
		}
	}

	displayWritePixel(ImageProcess.x + x, ImageProcess.y + y, blackOrWhite);
}

uint16_t drawRGB24toRGB565(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r / 8) << 11) | ((g / 4) << 5) | (b / 8);
}

uint32_t drawRGB565toBGRA32(uint16_t color)
{
	uint32_t bits = (uint32_t)color;
	uint32_t blue = bits & 0x001F;	// 5 bits blue
	uint32_t green = bits & 0x07E0; // 6 bits green
	uint32_t red = bits & 0xF800;	// 5 bits red

	// Return shifted bits with alpha set to 0xFF
	return (red << 8) | (green << 5) | (blue << 3) | 0xFF000000;
}