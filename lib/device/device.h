#ifndef DEVICE_H
#define DEVICE_H

#include <Arduino.h>
#include <pgmspace.h>
#include <ArduinoUniqueID.h>


void setupDevice();
void loopDevice();

void deviceSetSleepInterval(long interval);
long deviceGetSleepInterval();
unsigned int deviceGetBootCount();
bool deviceIsConfigured();

extern char DeviceId[21 +1];

#endif