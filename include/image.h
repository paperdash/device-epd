#ifndef IMAGE_H
#define IMAGE_H

#include <Arduino.h>

#include "imageWBMP.h"
#include "imagePNG.h"

void ImageNew(int x, int y, int w, int h, bool dithering);
void ImageWriteBuffer(uint8_t buff[], size_t c);
void ImageFlushBuffer();

#endif