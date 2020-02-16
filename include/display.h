#ifndef DISPLAY_H
#define DISPLAY_H

#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>

void setupDisplay();

void displayOpen();

void displayWritePixel(int16_t x, int16_t y, uint16_t color);
void displayWriteFramebuffer(uint8_t bitmap[]);

void displayFlush();

#endif