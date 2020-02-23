#ifndef DISPLAY_H
#define DISPLAY_H

#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>

//#define ENABLE_GxEPD2_GFX 0

extern GxEPD2_BW<GxEPD2_750, GxEPD2_750::HEIGHT> display;
//extern GxEPD2_BW<GxEPD2_750, GxEPD2_750::HEIGHT> display(GxEPD2_750(/*CS=*/5, /*DC=*/17, /*RST=*/16, /*BUSY=*/4));
//GxEPD2_BW<GxEPD2_750, GxEPD2_750::HEIGHT> displayGet();


void setupDisplay();

void displayOpen();

void displayWritePixel(int16_t x, int16_t y, uint16_t color);
void displayWriteFramebuffer(uint8_t bitmap[]);

void displayFlush();

#endif