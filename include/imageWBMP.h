#ifndef IMAGE_WBMP_H
#define IMAGE_WBMP_H

#include <Arduino.h>

void setupImageWBMP();

void wbmpOpenFramebuffer();
void wbmpWriteFramebuffer(int offset, uint8_t bitmap[], int c);
void wbmpFlushFramebuffer();

#endif