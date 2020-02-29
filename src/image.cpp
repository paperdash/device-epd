#include "image.h"

typedef struct
{
	size_t format;
	int x;
	int y;
	int w;
	int h;
	bool dithering;
} ImageProcess;
ImageProcess Image;

void ImageNew(int x, int y, int w, int h, bool dithering)
{
	Image.x = x;
	Image.y = y;
	Image.w = w;
	Image.h = h;
	Image.dithering = dithering;
}

void ImageWriteBuffer(uint8_t buff[], size_t c)
{
	// initial detect format
	if (Image.format == 0)
	{
		if (memcmp(buff, ImageHeaderWBMP, sizeof(ImageHeaderWBMP) - 1) == 0)
		{
			Serial.println(" image format: WBMP");
			Image.format = 2;

			wbmpOpenFramebuffer();
		}
		else if (memcmp(buff, ImageHeaderPNG, sizeof(ImageHeaderPNG) - 1) == 0)
		{
			Serial.println(" image format: PNG");
			Image.format = 3;

			pngOpenFramebuffer();
		}
		else
		{
			Image.format = 1;
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
	switch (Image.format)
	{
	// WBMP
	case 2:
		wbmpWriteFramebuffer(0, buff, c);
		break;
	// PNG
	case 3:
		pngWriteFramebuffer(0, buff, c);
		break;
	}
}

void ImageFlushBuffer()
{
	// update display
	switch (Image.format)
	{
	// WBMP
	case 2:
		wbmpFlushFramebuffer();
		break;
	// PNG
	case 3:
		pngFlushFramebuffer();
		break;
	}

	// clear settings
	Image.format = 0;
	Image.x = 0;
	Image.y = 0;
	Image.w = 0;
	Image.h = 0;
	Image.dithering = false;
}