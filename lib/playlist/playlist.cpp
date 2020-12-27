#include "playlist.h"
#include "settings.h"
#include "faceWeather.h"
#include "faceCalendar.h"
#include "faceSplash.h"
#include "faceToday.h"

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

typedef void (*Callable)();
typedef struct
{
	String name;
	Callable show;
	Callable setup;
	Callable loop;
} FaceAndName;
typedef FaceAndName FaceAndNameList[];

// List of faces to cycle through
FaceAndNameList faces = {
	{"Weather", showFaceWeather, setupFaceWeather, loopFaceWeather},
	{"Today", showFaceToday, setupFaceToday, loopFaceToday},
	// {"Splash", showFaceSplash, setupFaceSplash, loopFaceSplash},
	{"Calendar", showFaceCalendar, setupFaceCalendar, loopFaceCalendar},
};

const uint8_t faceCount = ARRAY_SIZE(faces);

unsigned long lastSwitch = 0;
int32_t timer;
uint8_t currentFaceIndex = 0;
//bool autoplayEnabled = false;

void setupPlaylist()
{
	Serial.println("setupPlaylist...");

	// load timer
	timer = NVS.getInt("playlist.timer") * 1000;
	if (timer < 30000)
	{
		timer = 30000;
	}

	// setup faces
	for (uint8_t i = 0; i < faceCount; i++)
	{
		faces[i].setup();
	}

	// force instant update
	lastSwitch = millis() - timer;
}

void loopPlaylist()
{
	// loop faces
	for (uint8_t i = 0; i < faceCount; i++)
	{
		faces[i].loop();
	}

	if (playlistGetRemainingTimeMs() <= 0) // && autoplayEnabled
	{
		playlistResetTimer();
		playlistNextFace();

		Serial.println("switch face: " + faces[currentFaceIndex].name);
		faces[currentFaceIndex].show();
	}
}

void playlistNextFace()
{
	currentFaceIndex++;

	if (currentFaceIndex < 0)
	{
		currentFaceIndex = 0;
	}

	// wrap around at the ends
	if (currentFaceIndex >= faceCount)
	{
		currentFaceIndex = 0;
	}
}

String playlistGetCurrentFace()
{
	return faces[currentFaceIndex].name;
}

int32_t playlistGetRemainingTimeMs()
{
	return timer - (millis() - lastSwitch);
}

void playlistResetTimer()
{
	lastSwitch = millis();
}

void playlistShow(const char name[])
{
	for (uint8_t i = 0; i < faceCount; i++)
	{
		if (faces[i].name.equalsIgnoreCase(name))
		{
			Serial.println("switch to face " + faces[i].name);

			currentFaceIndex = i -1;
			lastSwitch = 0;
			break;
		}
	}
}