#ifndef DISPLAY_H
#define DISPLAY_H

#define ENABLE_GxEPD2_GFX 0
#include <GxEPD2_BW.h>
#include <Adafruit_GFX.h>


void setupDisplay();
GFXcanvas1 *displayGetCanvas();

void displayOpen();
void displayWritePixel(int16_t x, int16_t y, uint16_t color);
void displayFlush();

void exportJPG(GFXcanvas1 *_canvas, const char *fileName, uint8_t q);
void exportBMP(GFXcanvas1 *_canvas, const char *fileName);

uint8_t displayPixelBWRatio();
void displayPrintScreenJPG(const char *fileName, uint8_t q);
void displayPrintScreenBMP(const char *fileName);

int displayStreamPrintScreenBMP(uint8_t *buffer, size_t maxLen, size_t index);

#endif