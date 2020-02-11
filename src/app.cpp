#include "app.h"
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"
#include "AsyncJson.h"
#include "ArduinoJson.h"

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


	// TODO response
	server.on("/test", HTTP_GET, [] (AsyncWebServerRequest *request) {
		AsyncResponseStream *response = request->beginResponseStream("application/json");

		DynamicJsonDocument root(1024);
		root["heap"] = ESP.getFreeHeap();
		root["ssid"] = WiFi.SSID();

		serializeJson(root, *response);
		request->send(response);
    });


	// TODO request
	AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/rest/endpoint", [](AsyncWebServerRequest *request, JsonVariant &json) {
		JsonObject jsonObj = json.to<JsonObject>();


		// ...
	});
	server.addHandler(handler);

	server.begin();

	Serial.println("setup configure - done");
}