#include <Arduino.h>
#include <SPI.h>
#include "device.h"
#include "wlan.h"
#include "display.h"
#include "settings.h"
#include "cloud.h"
#include "app.h"


void gotoDeepSleep();
String getWakeupReason();


void setup()
{
	// put your setup code here, to run once:

	Serial.begin(115200);
	delay(100); //Take some time to open up the Serial Monitor
	Serial.println();
	Serial.println("setup...");
	Serial.println();

	setupDisplay();
	setupSettings();
	setupDevice();
	setupWlan();

	if (wlan_isConnected())
	{
		//setupCloud();
	}

	setupApp();

	Serial.println();
	Serial.println("setup - done");
}

void loop()
{
	// put your main code here, to run repeatedly:

	if (wlan_isConnected())
	{
		//loopCloud();
	}

	loopDevice();
}