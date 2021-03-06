#ifndef DISPLAY_H
#define DISPLAY_H

#define ENABLE_GxEPD2_GFX 0
#include <GxEPD2_BW.h>
#include <Adafruit_GFX.h>


void setupDisplay();
GFXcanvas1 *displayGetCanvas();

void displayFlush();
uint16_t displayGetWidth();
uint16_t displayGetHeight();

int displaySnapshotBMPStream(uint8_t *buffer, size_t maxLen, size_t index);

#endif