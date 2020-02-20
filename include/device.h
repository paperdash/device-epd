#ifndef DEVICE_H
#define DEVICE_H

#include <Arduino.h>
#include <pgmspace.h>


void setupDevice();
void loopDevice();

void deviceSetSleepInterval(long interval);
long deviceGetSleepInterval();
unsigned int deviceGetBootCount();


#endif