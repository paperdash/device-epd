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

	if (PlaylistGetRemainingTimeMs() <= 0) // && autoplayEnabled
	{
		PlaylistResetTimer();
		PlaylistNextFace();

		Serial.println("switch face: " + faces[currentFaceIndex].name);
		faces[currentFaceIndex].show();
	}
}

void PlaylistNextFace()
{
	currentFaceIndex++;

	// wrap around at the ends
	if (currentFaceIndex < 0)
	{
		currentFaceIndex = faceCount - 1;
	}

	if (currentFaceIndex >= faceCount)
	{
		currentFaceIndex = 0;
	}
}

String PlaylistGetCurrentFace()
{
	return faces[currentFaceIndex].name;
}

int32_t PlaylistGetRemainingTimeMs()
{
	return timer - (millis() - lastSwitch);
}

void PlaylistResetTimer()
{
	lastSwitch = millis();
}