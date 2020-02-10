#ifndef DISPLAY_H
#define DISPLAY_H

#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>



void setupDisplay();

void displayOpenFramebuffer();
void displayWriteFramebuffer(int offset, uint8_t buff[], int c);
void displayFlushFramebuffer();


#endif