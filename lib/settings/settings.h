#ifndef SETTINGS_H
#define SETTINGS_H

#include "ArduinoNvs.h"

extern uint16_t COLOR_FG;
extern uint16_t COLOR_BG;

void setupSettings();
void settingsFactoryReset();
void loadSettingsColor();

#endif