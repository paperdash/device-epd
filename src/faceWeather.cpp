#include "faceWeather.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>


void setupFaceWeather()
{
	// https://github.com/andrei7c4/weatherdisplay
}


void loopFaceWeather()
{

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