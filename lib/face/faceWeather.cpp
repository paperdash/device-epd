#include <ArduinoJson.h>
#include <SPIFFS.h>
#include "faceWeather.h"
#include "faceWeather/Icons.h"
#include "display.h"
#include "download.h"
#include "settings.h"
#include "datetime.h"

#include <Fonts/FreeSansBold24pt7b.h> // current day
#include <Fonts/FreeSansBold18pt7b.h> // current day

const char faceWeatherCurrent[] = "/weatherCurrent.json";
const char faceWeatherForecast[] = "/weatherForecast.json";
faceWeatherData weatherData;

unsigned long lastWeatherDataUpdate = 0;

void render_current();
void render_forecast();
bool readWeatherData();
bool isFaceWeatherSetupCompleted();
bool updateWeatherData();

void setupFaceWeather()
{
	if (!isFaceWeatherSetupCompleted())
	{
		Serial.println("weather not configured");
	}

	readWeatherData();
	lastWeatherDataUpdate = millis();
}

void loopFaceWeather()
{
	// update every 10 min
	if ((millis() - lastWeatherDataUpdate) >= 600000)
	{
		Serial.println(&now, "update weather data @ %A, %B %d %Y %H:%M:%S");
		invalidFaceWeatherCache(true);
	}
}

void showFaceWeather()
{
	GFXcanvas1 *canvas = displayGetCanvas();

	canvas->setRotation(0);
	canvas->fillScreen(COLOR_BG);
	canvas->setTextColor(COLOR_FG);
	canvas->setTextSize(1);

	render_current();
	render_forecast();

	displayFlush();
}

void invalidFaceWeatherCache(bool warmUp)
{
	if (warmUp)
	{
		updateWeatherData();
		lastWeatherDataUpdate = millis();
	}
	else
	{

		lastWeatherDataUpdate = 0;
	}
}

bool isFaceWeatherSetupCompleted()
{
	return !(NVS.getString("weather.api").isEmpty() || NVS.getString("weather.lang").isEmpty() || NVS.getString("weather.lang").isEmpty());
}

void render_current()
{
	GFXcanvas1 *canvas = displayGetCanvas();

	// icon
	const unsigned char *icon = getIconById(weatherData.current_icon, 256);
	if (icon)
	{
		canvas->drawBitmap(192, 0, icon, 256, 256, COLOR_BG, COLOR_FG);
	}

	// name
	canvas->setFont(&FreeSansBold18pt7b);
	canvas->setTextSize(1);
	canvas->setCursor(20, 220);
	canvas->println(weatherData.location);

	// temperature
	canvas->setFont(&FreeSansBold24pt7b);
	canvas->setTextSize(2);
	canvas->setCursor(50, 120);
	canvas->println(weatherData.current_temp);

	// 250 height
	// high
	canvas->setTextSize(1);
	canvas->setCursor(500, 100);
	canvas->println(weatherData.current_max);

	// low
	canvas->setCursor(500, 180);
	canvas->println(weatherData.current_min);
}

void render_forecast()
{
	GFXcanvas1 *canvas = displayGetCanvas();
	const unsigned char *icon;

	// line forecast
	canvas->drawRect(0, 250, 640, 2, COLOR_FG);

	canvas->drawLine(160, 250, 160, 384, COLOR_FG);
	canvas->drawLine(320, 250, 320, 384, COLOR_FG);
	canvas->drawLine(480, 250, 480, 384, COLOR_FG);
	// 160 per block

	canvas->setTextSize(1);
	canvas->setFont(&FreeSansBold18pt7b);

	char label[20];
	int16_t tbx, tby;
	uint16_t tbw, tbh, x;
	uint16_t tempRangeY = 260 + 64 + 40;

	// day +1
	icon = getIconById(weatherData.forecast_1_icon, 64);
	if (icon)
	{
		canvas->drawBitmap(0 + 48, 260, icon, 64, 64, COLOR_BG, COLOR_FG);

		sprintf(label, "%2d ... %2d", weatherData.forecast_1_min, weatherData.forecast_1_max);
		canvas->getTextBounds(label, 0, 0, &tbx, &tby, &tbw, &tbh);
		x = ((160 - tbw) / 2) - tbx;
		canvas->setCursor(x, tempRangeY);
		canvas->print(label);
	}

	// day +2
	icon = getIconById(weatherData.forecast_2_icon, 64);
	if (icon)
	{
		canvas->drawBitmap(160 + 48, 260, icon, 64, 64, COLOR_BG, COLOR_FG);

		sprintf(label, "%2d ... %2d", weatherData.forecast_2_min, weatherData.forecast_2_max);
		canvas->getTextBounds(label, 0, 0, &tbx, &tby, &tbw, &tbh);
		x = ((160 - tbw) / 2) - tbx;
		canvas->setCursor(160 + x, tempRangeY);
		canvas->print(label);
	}

	// day +3
	icon = getIconById(weatherData.forecast_3_icon, 64);
	if (icon)
	{
		canvas->drawBitmap(320 + 48, 260, icon, 64, 64, COLOR_BG, COLOR_FG);

		sprintf(label, "%2d ... %2d", weatherData.forecast_3_min, weatherData.forecast_3_max);
		canvas->getTextBounds(label, 0, 0, &tbx, &tby, &tbw, &tbh);
		x = ((160 - tbw) / 2) - tbx;
		canvas->setCursor(320 + x, tempRangeY);
		canvas->print(label);
	}

	// day +4
	icon = getIconById(weatherData.forecast_4_icon, 64);
	if (icon)
	{
		canvas->drawBitmap(480 + 48, 260, icon, 64, 64, COLOR_BG, COLOR_FG);

		sprintf(label, "%2d ... %2d", weatherData.forecast_4_min, weatherData.forecast_4_max);
		canvas->getTextBounds(label, 0, 0, &tbx, &tby, &tbw, &tbh);
		x = ((160 - tbw) / 2) - tbx;
		canvas->setCursor(480 + x, tempRangeY);
		canvas->print(label);
	}
}

// @see https://openweathermap.org/appid#work
// @note no more than one time every 10 minutes for one location
bool downloadWeatherData()
{
	String url;

	// https://openweathermap.org/current
	url = "http://api.openweathermap.org/data/2.5/weather?";
	url += "APPID=" + NVS.getString("weather.api");
	url += "&id=";
	url.concat((unsigned long)NVS.getInt("weather.loc"));
	url += "&lang=" + NVS.getString("weather.lang");
	url += "&units=" + NVS.getString("weather.unit");
	if (!downloadFile(url, faceWeatherCurrent))
	{
		return false;
	}

	// https://openweathermap.org/forecast5
	url = "http://api.openweathermap.org/data/2.5/forecast/daily?";
	url += "APPID=" + NVS.getString("weather.api");
	url += "&id=";
	url.concat((unsigned long)NVS.getInt("weather.loc"));
	url += "&lang=" + NVS.getString("weather.lang");
	url += "&units=" + NVS.getString("weather.unit");
	url += "&cnt=4";
	if (!downloadFile(url, faceWeatherForecast))
	{
		return false;
	}

	return true;
}

/**
 * download and update weather data
 */
bool updateWeatherData()
{
	if (isFaceWeatherSetupCompleted())
	{
		if (downloadWeatherData())
		{
			readWeatherData();

			return true;
		}
	}

	return false;
}

/**
 * read weather data from json file
 */
bool readWeatherData()
{
	Serial.println("  readWeatherData");
	File file;
	DeserializationError error;

	// current weather
	file = SPIFFS.open(faceWeatherCurrent);
	if (!file)
	{
		Serial.print("Failed to open file: ");
		Serial.println(faceWeatherCurrent);

		return false;
	}

	StaticJsonDocument<976> docCurrent; // Use arduinojson.org/v6/assistant to compute the capacity.
	error = deserializeJson(docCurrent, file);
	file.close();
	if (error)
	{
		Serial.print("Failed to read file: ");
		Serial.println(faceWeatherCurrent);

		Serial.println(error.c_str());
		return false;
	}

	//serializeJsonPretty(docCurrent, Serial);
	//Serial.println(docCurrent["weather"][0]["icon"].as<char *>());
	//Serial.println(docCurrent["weather"][0]["main"].as<char *>());
	//Serial.println(docCurrent["sys"]["country"].as<char *>());
	//Serial.println(docCurrent["main"]["temp"].as<int>());

	// copy required values

	strlcpy(weatherData.location, docCurrent["name"] | "?", sizeof(weatherData.location));

	weatherData.current_temp = round(docCurrent["main"]["temp"].as<int>());
	weatherData.current_min = floor(docCurrent["main"]["temp_min"].as<int>());
	weatherData.current_max = ceil(docCurrent["main"]["temp_max"].as<int>());
	strlcpy(weatherData.current_icon, docCurrent["weather"][0]["icon"] | "50n", sizeof(weatherData.current_icon));

	// forecast
	file = SPIFFS.open(faceWeatherForecast);
	if (!file)
	{
		Serial.print("Failed to open file: ");
		Serial.println(faceWeatherForecast);

		return false;
	}

	StaticJsonDocument<3000> docForecast; // Use arduinojson.org/v6/assistant to compute the capacity.
	error = deserializeJson(docForecast, file);
	file.close();
	if (error)
	{
		Serial.print("Failed to read file: ");
		Serial.println(faceWeatherForecast);

		Serial.println(error.c_str());

		return false;
	}

	// copy required values
	weatherData.forecast_1_min = round(docForecast["list"][0]["temp"]["min"].as<int>());
	weatherData.forecast_1_max = round(docForecast["list"][0]["temp"]["max"].as<int>());
	strlcpy(weatherData.forecast_1_icon, docForecast["list"][0]["weather"][0]["icon"] | "50n", sizeof(weatherData.forecast_1_icon));

	weatherData.forecast_2_min = round(docForecast["list"][1]["temp"]["min"].as<int>());
	weatherData.forecast_2_max = round(docForecast["list"][1]["temp"]["max"].as<int>());
	strlcpy(weatherData.forecast_2_icon, docForecast["list"][1]["weather"][0]["icon"] | "50n", sizeof(weatherData.forecast_2_icon));

	weatherData.forecast_3_min = round(docForecast["list"][2]["temp"]["min"].as<int>());
	weatherData.forecast_3_max = round(docForecast["list"][2]["temp"]["max"].as<int>());
	strlcpy(weatherData.forecast_3_icon, docForecast["list"][2]["weather"][0]["icon"] | "50n", sizeof(weatherData.forecast_3_icon));

	weatherData.forecast_4_min = round(docForecast["list"][3]["temp"]["min"].as<int>());
	weatherData.forecast_4_max = round(docForecast["list"][3]["temp"]["max"].as<int>());
	strlcpy(weatherData.forecast_4_icon, docForecast["list"][3]["weather"][0]["icon"] | "50n", sizeof(weatherData.forecast_4_icon));

	return true;
}