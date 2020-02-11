#include "configure.h"
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"

// https://techtutorialsx.com/2018/09/17/esp32-arduino-web-server-serving-external-css-file/
// https://docs.platformio.org/en/latest/platforms/espressif8266.html#uploading-files-to-file-system-spiffs

AsyncWebServer server(80);


void setupConfigure()
{
	Serial.println("setup configure");

	if (!SPIFFS.begin()) {
		Serial.println("An Error has occurred while mounting SPIFFS");
		return;
	}

	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
		Serial.println("/");
		request->send(SPIFFS, "/index.html", "text/html");
	});

	server.on("/app.css", HTTP_GET, [](AsyncWebServerRequest *request) {
		Serial.println("/app.css");
		request->send(SPIFFS, "/app.css", "text/css");
	});

	server.begin();

	Serial.println("setup configure - done");
}


void loopConfigure()
{

}