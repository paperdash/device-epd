#include <Arduino.h>
#include <SPI.h>
#include "wlan.h"
#include "display.h"
#include "cloud.h"

// TODO client settings
//#define config_PullServer "http://smart-sign-server/satellite/get-data" // pull server address
//RTC_DATA_ATTR long config_DeepSleepInterval = 300;						// 5 min pull intervall
//String config_UUID = "";
// TODO client settings

void setup()
{
	// put your setup code here, to run once:

	Serial.begin(115200);
	delay(100); //Take some time to open up the Serial Monitor
	Serial.println();

	// setup hardware
	//setupConfig();
	//setupDeepSleep();
	setupWlan();
	setupCloud();

	// SPIFFS.begin(true);   // formatOnFail

	Serial.println("setup done");
}

void loop()
{
	// put your main code here, to run repeatedly:
}