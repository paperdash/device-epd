#include "configure.h"
#include "wlan.h"
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"

// https://techtutorialsx.com/2018/09/17/esp32-arduino-web-server-serving-external-css-file/
// https://docs.platformio.org/en/latest/platforms/espressif8266.html#uploading-files-to-file-system-spiffs

AsyncWebServer server(80);

void setupConfigure()
{
	if (wlan_isAPMode())
	{
		if (!SPIFFS.begin()) {
			Serial.println("An Error has occurred while mounting SPIFFS");
			return;
		}

		server.on("/dashboard", HTTP_GET, [](AsyncWebServerRequest *request){
			request->send(SPIFFS, "/dashboard.html", "text/html");
		});

		server.on("/dashboard.css", HTTP_GET, [](AsyncWebServerRequest *request){
			request->send(SPIFFS, "/dashboard.css", "text/css");
		});

		server.begin();
	}
}


void loopConfigure()
{
	if (wlan_isAPMode())
	{
		// ap mode
	}
	else
	{
		// connected to wifi
	}
}