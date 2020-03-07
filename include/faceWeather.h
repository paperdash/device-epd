#ifndef FACE_WEATHER_H
#define FACE_WEATHER_H

struct faceWeatherData
{
	// global
	char location[20];

	// current condition
	char current_icon[4];
	int current_temp;
	int current_min;
	int current_max;

	// forecast +1 (tomorrow)
	char forecast_1_icon[4];
	int forecast_1_min;
	int forecast_1_max;

	// forecast +2
	char forecast_2_icon[4];
	int forecast_2_min;
	int forecast_2_max;

	// forecast +3
	char forecast_3_icon[4];
	int forecast_3_min;
	int forecast_3_max;

	// forecast +4
	char forecast_4_icon[4];
	int forecast_4_min;
	int forecast_4_max;
};

void setupFaceWeather();
void loopFaceWeather();

bool updateWeatherData();

#endif