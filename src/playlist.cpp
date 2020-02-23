#include "playlist.h"
#include "settings.h"
#include "faceWeather.h"
#include "faceCalendar.h"


unsigned long lastSwitch = 0;
int64_t timer;

void setupPlaylist()
{
	Serial.println("setupPlaylist...");
	lastSwitch = millis();
	timer = NVS.getInt("playlist.timer") * 1000;
	if (timer < 30000)
	{
		timer = 30000;
	}


	// setup faces
	//setupFaceWeather();
	setupFaceCalendar();
}


void loopPlaylist()
{
	// return millis() - bootTime >= 60000;
	if (millis() - lastSwitch >= timer)
	{
		lastSwitch = millis();
		Serial.println("switch face");

		// TODO
	}
}