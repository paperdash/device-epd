#ifndef DISPLAY_H
#define DISPLAY_H

#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>



void setupDisplay();
void updateDisplay_Neu(const unsigned char *bitmap);

void drawBitmaps640x384();
void helloWorld();
void helloFullScreenPartialMode();
void helloArduino();
void helloEpaper();
void showFont(const char name[], const GFXfont* f);
void drawFont(const char name[], const GFXfont* f);
void drawBitmaps();

void deepSleepTest();

#endif