#ifndef FACE_WEATHER_H
#define FACE_WEATHER_H

// TODO
struct faceWeatherData
{
	//char hostname[64];
	//int port;

	char current_icon[4];
	char current_description[20];

	char location[20];

	// current
	int current_temp;
	int today_min;
	int today_max;

	// forecast
};

void setupFaceWeather();
void loopFaceWeather();

#endif