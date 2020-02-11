#include "app.h"
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"

// https://techtutorialsx.com/2018/09/17/esp32-arduino-web-server-serving-external-css-file/
// https://docs.platformio.org/en/latest/platforms/espressif8266.html#uploading-files-to-file-system-spiffs

AsyncWebServer server(80);


void setupApp()
{
	Serial.println("setup configure");

	if (!SPIFFS.begin()) {
		Serial.println("An Error has occurred while mounting SPIFFS");
		return;
	}


	// @see https://github.com/me-no-dev/ESPAsyncWebServer

	// serve static files
	server
		.serveStatic("/", SPIFFS, "/dist/")
		.setDefaultFile("index.html")
		.setCacheControl("max-age=600")
	;

	server.begin();

	Serial.println("setup configure - done");
}