#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "faceWeather.h"
#include "faceWeatherIcons.h"
#include "display.h"

#include <Fonts/FreeSansBold24pt7b.h> // current day

// TODO use theme color

void display_current();
void display_forecast();

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
	const uint *icon = getIconById("02n", 192);	// 192
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
		display.drawBitmap(0 +57, 260, (uint8_t *)icon + 4, icon[0], icon[1], GxEPD_WHITE);
	}


	// day +2
	icon = getIconById("09d", 96);
	if (icon)
	{
		display.drawBitmap(210 +57, 260, (uint8_t *)icon + 4, icon[0], icon[1], GxEPD_WHITE);
	}


	// day +3
	icon = getIconById("13d", 96);
	if (icon)
	{
		display.drawBitmap(410 +57, 260, (uint8_t *)icon + 4, icon[0], icon[1], GxEPD_WHITE);
	}

}


/*
bool loadWeatherData(const char *type)
{
	HTTPClient http;
	String url = "http://api.openweathermap.org/data/2.5/forecast?"; // weather | forecast
	url += "id=2766824";
	url += "&lang=de";
	url += "&cnt=3";
	url += "&units=metric";
	url += "&APPID=883b3c87223430d6f3a399645f8ba12b";
	// "/data/2.5/" + RequestType + "?q=" + City + "," + Country + "&APPID=" + apikey + "&mode=json&units=" + units + "&lang=" + Language;

	//http.begin(client, server, 80, uri);

	http.begin(url);
	int httpCode = http.GET();
	if (httpCode != HTTP_CODE_OK)
	{
		Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode != HTTP_CODE_OK && httpCode).c_str());
	}
	else
	{
		// TODO read...

		DynamicJsonDocument doc(1024 * 35);
		DeserializationError error = deserializeJson(doc, http.getStream());
		if (error)
		{
			Serial.print(F("deserializeJson() failed with code "));
			Serial.println(error.c_str());
			return false;
		}

		JsonObject root = doc.as<JsonObject>();
	}

	http.end();
}

*/