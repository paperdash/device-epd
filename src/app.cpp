#include "app.h"
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "settings.h"
#include "device.h"

AsyncWebServer server(80);

void setupSettingsGet();
void setupSettingsPost();
void setupWifiScan();
void setupWifiConnect();
void setupCurrentImage();

void setupApp()
{
	Serial.println("setup configure");

	if (!SPIFFS.begin())
	{
		Serial.println("An Error has occurred while mounting SPIFFS");
		return;
	}

	// @see https://github.com/me-no-dev/ESPAsyncWebServer
	// @see https://arduinojson.org/v6/assistant/

	// serve static files
	server
		.serveStatic("/", SPIFFS, "/dist/")
		.setDefaultFile("index.html")
		//.setCacheControl("max-age=600")
		;

	setupSettingsGet();
	setupSettingsPost();
	setupWifiScan();
	setupWifiConnect();
	setupCurrentImage();

	// TODO response
	server.on("/stats", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncResponseStream *response = request->beginResponseStream("application/json");
		DynamicJsonDocument doc(1024);	// TODO

		doc["wifi"]["ssid"] = WiFi.SSID();
		doc["wifi"]["connected"] = WiFi.isConnected();
		doc["wifi"]["ip"] = WiFi.localIP().toString();
		doc["wifi"]["mac"] = WiFi.macAddress();
		doc["wifi"]["channel"] = WiFi.channel();
		doc["wifi"]["dns"] = WiFi.dnsIP().toString();
		doc["wifi"]["gateway"] = WiFi.gatewayIP().toString();

		doc["device"]["heap"] = ESP.getFreeHeap();
		doc["device"]["bootCycle"] = deviceGetBootCount();
		doc["device"]["screen"]["width"] = 640;
		doc["device"]["height"]["screen"] = 384;

		JsonArray capability = doc.createNestedArray("capability");
		capability.add("png");
		capability.add("wbmp");

		doc["cloud"]["sleep"] = deviceGetSleepInterval();

		serializeJson(doc, *response);
		request->send(response);
	});

	// CORS
	//DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
	server.begin();

	Serial.println("setup configure - done");
}

void setupSettingsGet()
{
	server.on("/api/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncResponseStream *response = request->beginResponseStream("application/json");
		DynamicJsonDocument root(1024);

		root["device_mode"] = NVS.getString("device_mode");
		root["device_rotation"] = 0;
		root["cloud_server"] = NVS.getString("cloud_server");
		root["cloud_uuid"] = NVS.getString("cloud_uuid");
		root["cloud_user"] = NVS.getString("cloud_user");

		serializeJson(root, *response);
		request->send(response);
	});
}

void setupSettingsPost()
{
	server.on("/api/settings", HTTP_PUT, [](AsyncWebServerRequest *request) { /* nothing and dont remove it */ }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
		DynamicJsonDocument doc(1024);

		DeserializationError error = deserializeJson(doc, data);
		if (error) {
			Serial.print(F("deserializeJson() failed with code "));
			Serial.println(error.c_str());

			request->send(404, "text/plain", "");
		}
		else
		{
			if (doc.containsKey("device_mode")) {
				NVS.setString("device_mode", doc["device_mode"]);
			}
			if (doc.containsKey("cloud_server")) {
				NVS.setString("cloud_server", doc["cloud_server"]);
				//Serial.println(doc["cloud_server"].as<char*>());
			}
			if (doc.containsKey("cloud_uuid")) {
				NVS.setString("cloud_uuid", doc["cloud_uuid"]);
			}
			if (doc.containsKey("cloud_user")) {
				NVS.setString("cloud_user", doc["cloud_user"]);
			}

			request->send(200, "application/ld+json; charset=utf-8", "{}");
		} });
}

/**
 * @todo
 */
void setupCurrentImage()
{
	server.on("/current-image", HTTP_GET, [](AsyncWebServerRequest *request) {
		Serial.println("/current-image");
		//request->send(SPIFFS, "/currentImage.bin", "image/x-bmp");
		request->send(SPIFFS, "/blackPNG.png");
	});

	/*
	server.on("/current-image2", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/currentImage.bin", "image/x-bmp"); // image/x-bmp | image/vnd.wap.wbmp
		//response->addHeader("Content-Encoding", "gzip");
		//response->addHeader("Content-Disposition", "inline; filename=\"image.wbmp\"");

		request->send(response);
	});
*/
}

/**
 * @todo
 */
void setupWifiScan()
{
	//First request will return 0 results unless you start scan from somewhere else (loop/setup)
	//Do not request more often than 3-5 seconds
	server.on("/api/wifi/scan", HTTP_GET, [](AsyncWebServerRequest *request) {
		String json = "[";
		int n = WiFi.scanComplete();
		if (n == -2)
		{
			WiFi.scanNetworks(true);
		}
		else if (n)
		{
			for (int i = 0; i < n; ++i)
			{
				if (i)
				{
					json += ",";
				}

				json += "{";
				json += "\"rssi\":" + String(WiFi.RSSI(i));
				json += ",\"ssid\":\"" + WiFi.SSID(i) + "\"";
				json += ",\"bssid\":\"" + WiFi.BSSIDstr(i) + "\"";
				json += ",\"channel\":" + String(WiFi.channel(i));
				json += ",\"secure\":" + String(WiFi.encryptionType(i));
				json += "}";
			}

			WiFi.scanDelete();
			if (WiFi.scanComplete() == -2)
			{
				WiFi.scanNetworks(true);
			}
		}

		json += "]";
		request->send(200, "application/json", json);
		json = String();
	});
}

/**
 * @todo
 */
void setupWifiConnect()
{
	server.on("/api/wifi/connect", HTTP_POST, [](AsyncWebServerRequest *request) { /* nothing and dont remove it */ }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
		DynamicJsonDocument doc(1024);

		DeserializationError error = deserializeJson(doc, data);
		if (error) {
			Serial.print(F("deserializeJson() failed with code "));
			Serial.println(error.c_str());

			request->send(404, "text/plain", "");
		}
		else
		{
			if (doc.containsKey("ssid")) {
				NVS.setString("wifi_ssid", doc["ssid"]);
				Serial.println(doc["ssid"].as<char*>());
			}
			if (doc.containsKey("password")) {
				NVS.setString("wifi_password", doc["password"]);
				Serial.println(doc["password"].as<char*>());
			}

			request->send(200, "application/ld+json; charset=utf-8", "{}");

			ESP.restart();
		} });
}