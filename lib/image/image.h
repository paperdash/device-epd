#ifndef IMAGE_H
#define IMAGE_H

#include <Arduino.h>

typedef struct
{
	size_t format;
	int x;
	int y;
	int w;
	int h;
	bool dithering;
} structImageProcess;

void setupImage();

void ImageNew(int x, int y, int w, int h, bool dithering);
void ImageWriteBuffer(uint8_t buff[], size_t c);
void ImageFlushBuffer();

void ImageProcessPixel(uint16_t x, uint16_t y, uint8_t rgba[4]);

#endif /* IMAGE_H */