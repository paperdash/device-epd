#include "settings.h"

void setupSettings()
{
	NVS.begin();
}

void settingsFactoryReset()
{
	NVS.eraseAll();

	// TODO remove also json db file
}