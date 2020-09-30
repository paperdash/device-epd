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

void ImageProcessPixel(uint32_t x, uint32_t y, uint8_t rgba[4]);

uint16_t drawRGB24toRGB565(uint8_t r, uint8_t g, uint8_t b);
uint32_t drawRGB565toBGRA32(uint16_t color);

#endif /* IMAGE_H */