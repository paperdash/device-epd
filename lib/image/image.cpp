#include "image.h"
#include "imagePNG.h"
#include "imageJPEG.h"
#include "display.h"

structImageProcess ImageProcess;
// TODO use dynamic display width
constexpr uint16_t MAX_WIDTH = 640;
int16_t curRowDelta[MAX_WIDTH + 1];
int16_t nextRowDelta[MAX_WIDTH + 1];

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
		if (memcmp(buff, ImageHeaderPNG, sizeof(ImageHeaderPNG) - 1) == 0)
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

void ImageProcessPixel(uint16_t x, uint16_t y, uint8_t rgba[4])
{
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