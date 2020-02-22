#ifndef IMAGE_WBMP_H
#define IMAGE_WBMP_H

#include <Arduino.h>

// @see https://en.wikipedia.org/wiki/Wireless_Application_Protocol_Bitmap_Format
// 0 0 133 0 131 0
const char ImageHeaderWBMP[] = "\x0\x0\x85\x0\x83\x0";

void setupImageWBMP();

void wbmpOpenFramebuffer();
void wbmpWriteFramebuffer(int offset, uint8_t bitmap[], int c);
void wbmpFlushFramebuffer();

#endif