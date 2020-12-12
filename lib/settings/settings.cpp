#include "settings.h"

uint16_t COLOR_FG = 0x0000;
uint16_t COLOR_BG = 0xFFFF;

void setupSettings()
{
	NVS.begin();

	loadSettingsColor();
}

void settingsFactoryReset()
{
	NVS.eraseAll();

	// TODO remove also json db file
}

void loadSettingsColor()
{
	if (NVS.getString("device.theme") == "black")
	{
		COLOR_FG = 0xFFFF;
		COLOR_BG = 0x0000;
	}
	else
	{
		COLOR_FG = 0x0000;
		COLOR_BG = 0xFFFF;
	}
}