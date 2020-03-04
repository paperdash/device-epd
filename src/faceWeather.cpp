#include <ArduinoJson.h>
#include <SPIFFS.h>
#include "faceWeather.h"
#include "faceWeatherIcons.h"
#include "display.h"
#include "download.h"

#include <Fonts/FreeSansBold24pt7b.h> // current day
#include <Fonts/FreeSansBold18pt7b.h> // current day

const char faceWeatherCurrent[] = "/weatherCurrent.json";
const char faceWeatherForecast[] = "/weatherForecast.json";
faceWeatherData weatherData;

// TODO use theme color

void render_current();
void render_forecast();
bool downloadWeatherData();
void readWeatherData();

void setupFaceWeather()
{
}

void loopFaceWeather()
{
	display.setRotation(0);
	display.setFullWindow();
	display.firstPage();
	display.fillScreen(GxEPD_BLACK);
	display.setTextColor(GxEPD_WHITE);
	display.setTextSize(1);

	render_current();
	render_forecast();

	displayFlush();
}

void render_current()
{
	// temperature
	display.setFont(&FreeSansBold24pt7b);
	display.setTextSize(2);
	display.setCursor(50, 120);
	display.println("3°");

	// icon
	const uint *icon = getIconById("02n", 192); // 192
	if (icon)
	{
		//display.drawInvertedBitmap(224, 30, myIcon, 192, 192, GxEPD_WHITE);
		display.drawBitmap(224, 30, (uint8_t *)icon + 4, icon[0], icon[1], GxEPD_WHITE);
	}

	// text
	display.setTextSize(1);
	display.setCursor(400, 300);
	//display.println("Ein paar Wolken");

	// high
	display.setTextSize(1);
	display.setCursor(500, 30);
	display.println("5°");

	// low
	display.setCursor(500, 60);
	display.println("-3°");
}

void render_forecast()
{
	const uint *icon;

	// line forecast
	display.drawRect(0, 250, 640, 2, GxEPD_WHITE);

	display.drawLine(160, 250, 160, 384, GxEPD_WHITE);
	display.drawLine(320, 250, 320, 384, GxEPD_WHITE);
	display.drawLine(480, 250, 480, 384, GxEPD_WHITE);
	// 160 per block

	display.setTextSize(1);
	display.setFont(&FreeSansBold18pt7b);

	char label[20];
	int16_t tbx, tby;
	uint16_t tbw, tbh, x;
	uint16_t tempRangeY = 260 + 64 + 40;

	// day +1
	icon = getIconById("03d", 64);
	if (icon)
	{
		display.drawBitmap(0 + 48, 260, (uint8_t *)icon + 4, icon[0], icon[1], GxEPD_WHITE);

		sprintf(label, "%2d ... %2d", -3, 11);
		display.getTextBounds(label, 0, 0, &tbx, &tby, &tbw, &tbh);
		x = ((160 - tbw) / 2) - tbx;
		display.setCursor(x, tempRangeY);
		display.print(label);
	}

	// day +2
	icon = getIconById("09d", 64);
	if (icon)
	{
		display.drawBitmap(160 + 48, 260, (uint8_t *)icon + 4, icon[0], icon[1], GxEPD_WHITE);

		sprintf(label, "%2d ... %2d", 0, 7);
		display.getTextBounds(label, 0, 0, &tbx, &tby, &tbw, &tbh);
		x = ((160 - tbw) / 2) - tbx;
		display.setCursor(160 + x, tempRangeY);
		display.print(label);
	}

	// day +3
	icon = getIconById("13d", 64);
	if (icon)
	{
		display.drawBitmap(320 + 48, 260, (uint8_t *)icon + 4, icon[0], icon[1], GxEPD_WHITE);

		sprintf(label, "%2d ... %2d", 1, 21);
		display.getTextBounds(label, 0, 0, &tbx, &tby, &tbw, &tbh);
		x = ((160 - tbw) / 2) - tbx;
		display.setCursor(320 + x, tempRangeY);
		display.print(label);
	}

	// day +4
	icon = getIconById("13d", 64);
	if (icon)
	{
		display.drawBitmap(480 + 48, 260, (uint8_t *)icon + 4, icon[0], icon[1], GxEPD_WHITE);

		sprintf(label, "%2d ... %2d", 19, 35);
		display.getTextBounds(label, 0, 0, &tbx, &tby, &tbw, &tbh);
		x = ((160 - tbw) / 2) - tbx;
		display.setCursor(480 + x, tempRangeY);
		display.print(label);
	}
}

// @todo load params from settings
// @see https://openweathermap.org/appid#work
// @note no more than one time every 10 minutes for one location
bool downloadWeatherData()
{
	String url;
	// http://api.openweathermap.org/data/2.5/weather?id=2766824&APPID=883b3c87223430d6f3a399645f8ba12b&lang=de&cnt=3&units=metric
	// http://api.openweathermap.org/data/2.5/forecast?id=2766824&APPID=883b3c87223430d6f3a399645f8ba12b&lang=de

	// https://openweathermap.org/current
	url = "http://api.openweathermap.org/data/2.5/weather?";
	url += "APPID=883b3c87223430d6f3a399645f8ba12b"; // api key
	url += "&id=2766824";							 // location
	url += "&lang=de&units=metric";					 // settings
	if (!downloadFile(url, faceWeatherCurrent))
	{
		return false;
	}

	// https://openweathermap.org/forecast5
	url = "http://api.openweathermap.org/data/2.5/forecast?";
	url += "APPID=883b3c87223430d6f3a399645f8ba12b"; // api key
	url += "&id=2766824";							 // location
	url += "&lang=de&cnt=3&units=metric";			 // settings
	if (!downloadFile(url, faceWeatherForecast))
	{
		return false;
	}

	return true;
}

void readWeatherData()
{
	File file;
	DeserializationError error;

	// current weather
	file = SPIFFS.open(faceWeatherCurrent);
	StaticJsonDocument<976> docCurrent; // Use arduinojson.org/v6/assistant to compute the capacity.
	error = deserializeJson(docCurrent, file);
	if (error)
	{
		Serial.println(F("Failed to read file, using default configuration"));
	}

	// TODO Copy values from the JsonDocument to the Config
	weatherData.current_temp = 12;

	/*
	config.port = doc["port"] | 2731;
	strlcpy(config.hostname,				 // <- destination
			doc["hostname"] | "example.com", // <- source
			sizeof(config.hostname));		 // <- destination's capacity
	*/
	file.close();

	// forecast
	file = SPIFFS.open(faceWeatherForecast);
	StaticJsonDocument<2180> docForecast; // Use arduinojson.org/v6/assistant to compute the capacity.
	error = deserializeJson(docForecast, file);
	if (error)
	{
		Serial.println(F("Failed to read file, using default configuration"));
	}

	// TODO get values

	file.close();
}