#include "image.h"
#include "imageWBMP.h"
#include "imagePNG.h"

structImageProcess ImageProcess;

void ImageNew(int x, int y, int w, int h, bool dithering)
{
	Serial.printf("ImageNew: x: %d, y: %d, dithering: %d \n", x, y, dithering);

	ImageProcess.x = x;
	ImageProcess.y = y;
	ImageProcess.w = w;
	ImageProcess.h = h;
	ImageProcess.dithering = dithering;
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
	}

	// clear settings
	ImageProcess.format = 0;
	ImageProcess.x = 0;
	ImageProcess.y = 0;
	ImageProcess.w = 0;
	ImageProcess.h = 0;
	ImageProcess.dithering = false;
}