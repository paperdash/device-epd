#include <ArduinoJson.h>
#include <SPIFFS.h>
#include "faceWeather.h"
#include "faceWeatherIcons.h"
#include "display.h"
#include "download.h"

#include <Fonts/FreeSansBold24pt7b.h> // current day

const char faceWeatherCurrent[] = "/weatherCurrent.json";
const char faceWeatherForecast[] = "/weatherForecast.json";
faceWeatherData weatherData;

// TODO use theme color

void display_current();
void display_forecast();

void setupFaceWeather()
{
}

void updateData()
{
	String url;
	// http://api.openweathermap.org/data/2.5/weather?id=2766824&APPID=883b3c87223430d6f3a399645f8ba12b&lang=de&cnt=3&units=metric
	// http://api.openweathermap.org/data/2.5/forecast?id=2766824&APPID=883b3c87223430d6f3a399645f8ba12b&lang=de

	// https://openweathermap.org/current
	url = "http://api.openweathermap.org/data/2.5/weather?";
	url += "APPID=883b3c87223430d6f3a399645f8ba12b"; // api key
	url += "&id=2766824";							 // location
	url += "&lang=de&units=metric";					 // settings
	downloadFile(url, faceWeatherCurrent);

	// https://openweathermap.org/forecast5
	url = "http://api.openweathermap.org/data/2.5/forecast?";
	url += "APPID=883b3c87223430d6f3a399645f8ba12b"; // api key
	url += "&id=2766824";							 // location
	url += "&lang=de&cnt=3&units=metric";			 // settings
	downloadFile(url, faceWeatherForecast);
}

void loopFaceWeather()
{
	display.setRotation(0);
	display.setFullWindow();
	display.firstPage();
	display.fillScreen(GxEPD_BLACK);
	display.setTextColor(GxEPD_WHITE);
	display.setTextSize(1);

	display_current();
	display_forecast();

	display.nextPage();
}

void display_current()
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
		display.drawBitmap(272, 30, (uint8_t *)icon + 4, icon[0], icon[1], GxEPD_WHITE);
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

void display_forecast()
{
	const uint *icon;

	// line forecast
	//display.drawLine(0, 250, 640, 250, GxEPD_WHITE);
	display.drawRect(0, 250, 640, 2, GxEPD_WHITE);

	display.drawLine(210, 250, 210, 384, GxEPD_WHITE);
	display.drawLine(420, 250, 420, 384, GxEPD_WHITE);
	// 210 per block

	// day +1
	icon = getIconById("03d", 96);
	if (icon)
	{
		display.drawBitmap(0 + 57, 260, (uint8_t *)icon + 4, icon[0], icon[1], GxEPD_WHITE);
	}

	// day +2
	icon = getIconById("09d", 96);
	if (icon)
	{
		display.drawBitmap(210 + 57, 260, (uint8_t *)icon + 4, icon[0], icon[1], GxEPD_WHITE);
	}

	// day +3
	icon = getIconById("13d", 96);
	if (icon)
	{
		display.drawBitmap(410 + 57, 260, (uint8_t *)icon + 4, icon[0], icon[1], GxEPD_WHITE);
	}
}

void loadConfiguration()
{
	SPIFFS.begin();
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

	SPIFFS.end();
}