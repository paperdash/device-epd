#include <Arduino.h>
#include <SPIFFS.h>
#include "device.h"
#include "wlan.h"
#include "display.h"
#include "settings.h"
#include "datetime.h"
#include "playlist.h"
#include "app.h"
#include "image.h"
#include "faceSplash.h"
#include "faceWifi.h"

void setup()
{
	// put your setup code here, to run once:

	Serial.begin(115200);
	delay(100); //Take some time to open up the Serial Monitor
	Serial.println();
	Serial.println("setup...");
	Serial.println();

	if (!SPIFFS.begin())
	{
		Serial.println("An Error has occurred while mounting SPIFFS");
		return;
	}

	setupDevice();
	setupDisplay();
	setupSettings();
	setupImage();

	setupWlan();
	setupApp();

	if (WiFi.isConnected())
	{
		setupDateTime();
		setupPlaylist();

		// show boot splash for x seconds
		showFaceSplash();
		playlistNextSwitchIn(3);
	}
	else
	{
		showFaceWifi();
	}

	Serial.println();
	Serial.println("setup - done");
}

void loop()
{
	loopWlan();

	if (WiFi.isConnected())
	{
		loopDateTime();

		loopPlaylist();
	}

	loopApp();
}