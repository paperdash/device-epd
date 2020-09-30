#ifndef IMAGE_JPEG_H
#define IMAGE_JPEG_H

#include "image.h"
extern structImageProcess ImageProcess;

// @see http://www.libpng.org/pub/png/spec/1.2/PNG-Structure.html
const char ImageHeaderJPEG[] = "\xFF\xD8\xFF\xE0\x00\x10\x4A\x46\x49\x46\x00";
// FF E0 00 10 4A 46 , 49 46 00

void setupImageJPEG();
void jpegOpenFramebuffer();
void jpegWriteFramebuffer(int offset, uint8_t bitmap[], int c);
void jpegFlushFramebuffer();

#endif