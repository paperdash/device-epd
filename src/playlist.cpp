#include "playlist.h"
#include "settings.h"
#include "faceWeather.h"
#include "faceCalendar.h"

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

typedef void (*Face)();
typedef Face FaceList[];
typedef struct
{
	Face face;
	String name;
} FaceAndName;
typedef FaceAndName FaceAndNameList[];

// List of faces to cycle through
FaceAndNameList faces = {
	{playlistFaceWeather, "Weather"},
	{playlistFaceCalendar, "Calendar"},
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

	setupFaceWeather();
	setupFaceCalendar();

	//Serial.print("  Timeout is: ");
	//Serial.print(timer);
	//Serial.println("");

	// force instant update
	lastSwitch = millis() - timer;
}

void loopPlaylist()
{
	loopFaceWeather();
	loopFaceCalendar();

	if (PlaylistGetRemainingTimeMs() <= 0) // && autoplayEnabled
	{
		PlaylistResetTimer();
		PlaylistNextFace();

		Serial.println("switch face: " + faces[currentFaceIndex].name);
		faces[currentFaceIndex].face();
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