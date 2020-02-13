#include "app.h"
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "settings.h"

// https://techtutorialsx.com/2018/09/17/esp32-arduino-web-server-serving-external-css-file/
// https://docs.platformio.org/en/latest/platforms/espressif8266.html#uploading-files-to-file-system-spiffs

AsyncWebServer server(80);

void setupSettingsGet();
void setupSettingsPost();
void setupWifiScan();
void setupWifiConnect();

void setupApp()
{
	Serial.println("setup configure");

	if (!SPIFFS.begin()) {
		Serial.println("An Error has occurred while mounting SPIFFS");
		return;
	}


	// @see https://github.com/me-no-dev/ESPAsyncWebServer
	// @see https://arduinojson.org/v6/assistant/

	// serve static files
	server
		.serveStatic("/", SPIFFS, "/dist/")
		.setDefaultFile("index.html")
		.setCacheControl("max-age=600")
	;

	setupSettingsGet();
	setupSettingsPost();
	setupWifiScan();
	setupWifiConnect();


	// TODO response
	server.on("/test", HTTP_GET, [] (AsyncWebServerRequest *request) {
		AsyncResponseStream *response = request->beginResponseStream("application/json");

		DynamicJsonDocument root(1024);
		root["heap"] = ESP.getFreeHeap();
		root["ssid"] = WiFi.SSID();

		serializeJson(root, *response);
		request->send(response);
    });


	// CORS
	//DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
	server.begin();

	Serial.println("setup configure - done");
}


void setupSettingsGet()
{
	server.on("/api/settings", HTTP_GET, [] (AsyncWebServerRequest *request) {
		AsyncResponseStream *response = request->beginResponseStream("application/json");

		DynamicJsonDocument root(1024);
		//root["heap"] = ESP.getFreeHeap();
		// NVS.getString("wifi_ssid");
		root["wifi_ssid"] = NVS.getString("wifi_ssid"); // WiFi.SSID();
		root["device_mode"] = "active";
		root["device_rotation"] = 0;
		root["cloud_refresh"] = 97; // aktueller sleep timer

		serializeJson(root, *response);
		request->send(response);
    });

}


void setupSettingsPost()
{
	AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/api/settings", [](AsyncWebServerRequest *request, JsonVariant &json) {
		JsonObject jsonObj = json.to<JsonObject>();

		Serial.println("/api/settings");

		NVS.setString("wifi_ssid", jsonObj["wifi_ssid"], true);


/*
		char hostname[64];

		strlcpy(hostname,                  // <- destination
          jsonObj["wifi_ssid"] | "example.com",  // <- source
          sizeof(hostname));         // <- destination's capacity
*/
		Serial.println(NVS.getString("wifi_ssid"));


		// ...
	});
	server.addHandler(handler);

}


void setupCurrentImage()
{
	server.on("/current-image", HTTP_GET, [] (AsyncWebServerRequest *request) {


		AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/currentImage.bin", "image/vnd.wap.wbmp");
		//response->addHeader("Content-Encoding", "gzip");


		request->send(response);
    });

}


void setupWifiScan()
{
	//First request will return 0 results unless you start scan from somewhere else (loop/setup)
	//Do not request more often than 3-5 seconds
	server.on("/api/wifi/scan", HTTP_GET, [](AsyncWebServerRequest *request){
		String json = "[";
		int n = WiFi.scanComplete();
		if(n == -2){
			WiFi.scanNetworks(true);
		} else if(n){
			for (int i = 0; i < n; ++i){
			if(i) json += ",";
			json += "{";
			json += "\"rssi\":"+String(WiFi.RSSI(i));
			json += ",\"ssid\":\""+WiFi.SSID(i)+"\"";
			json += ",\"bssid\":\""+WiFi.BSSIDstr(i)+"\"";
			json += ",\"channel\":"+String(WiFi.channel(i));
			json += ",\"secure\":"+String(WiFi.encryptionType(i));
			json += "}";
			}
			WiFi.scanDelete();
			if(WiFi.scanComplete() == -2){
				WiFi.scanNetworks(true);
			}
		}
		json += "]";
		request->send(200, "application/json", json);
		json = String();
	});
}


void setupWifiConnect()
{
	AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/api/wifi/connect", [](AsyncWebServerRequest *request, JsonVariant &json) {
		//JsonObject jsonObj = json.to<JsonObject>();
		// TODO save settings


		AsyncResponseStream *response = request->beginResponseStream("application/json");

		DynamicJsonDocument root(1024);
		root["status"] = true;
		root["message"] = "";

		serializeJson(root, *response);
		request->send(response);

		// ist ein restart wichtig?
		//ESP.restart();

	});
	server.addHandler(handler);
}