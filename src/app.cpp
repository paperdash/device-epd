#include <SPIFFS.h>
#include "app.h"
#include "ESPAsyncWebServer.h"
#include "ArduinoJson.h"
#include "settings.h"
#include "device.h"
#include "playlist.h"
#include "image.h"
#include "display.h"
#include "datetime.h"
#include "faceWeather.h"
#include "faceCalendar.h"
#include "download.h"

AsyncWebServer server(80);

void setupSettingsGet();
void setupSettingsPost();
void setupWifiScan();
void setupWifiConnect();
void setupCurrentImage();
void setupApiFace();
void setupApiUpdate();
bool updateDisplayRequired = false;

void setupApp()
{
	Serial.println("setup configure");

	// @see https://github.com/me-no-dev/ESPAsyncWebServer
	// @see https://arduinojson.org/v6/assistant/

	// serve static files
	server
		.serveStatic("/", SPIFFS, "/dist/")
		.setDefaultFile("index.html")
		//.setCacheControl("max-age=600")
		;
	server.serveStatic("/fs/", SPIFFS, "/");

	setupSettingsGet();
	setupSettingsPost();
	setupWifiScan();
	setupWifiConnect();
	setupCurrentImage();
	setupApiFace();
	setupApiUpdate();

	server.onNotFound([](AsyncWebServerRequest *request) {
		request->send(404);
	});

	// TODO response
	server.on("/stats", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncResponseStream *response = request->beginResponseStream("application/json");
		DynamicJsonDocument doc(1024); // TODO

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
		doc["device"]["screen"]["height"] = 384;

		doc["device"]["fs"]["total"] = SPIFFS.totalBytes();
		doc["device"]["fs"]["used"] = SPIFFS.usedBytes();
		doc["device"]["fs"]["free"] = SPIFFS.totalBytes() - SPIFFS.usedBytes();

		doc["playlist"]["current"] = PlaylistGetCurrentFace();
		doc["playlist"]["remaining"] = PlaylistGetRemainingTimeMs() / 1000;

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

void loopApp()
{
	if (updateDisplayRequired)
	{
		Serial.println("loop app update display");

		// stop playlist to show the new image
		PlaylistResetTimer();

		updateDisplayRequired = false;
		display.nextPage();
	}
}

void setupSettingsGet()
{
	server.on("/api/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncResponseStream *response = request->beginResponseStream("application/json");
		DynamicJsonDocument root(1024);

		root["device"]["angle"] = NVS.getInt("device.angle");
		root["device"]["theme"] = NVS.getString("device.theme");

		root["playlist"]["timer"] = NVS.getInt("playlist.timer");

		root["weather"]["api"] = NVS.getString("weather.api");
		root["weather"]["location"] = NVS.getInt("weather.loc");
		root["weather"]["lang"] = NVS.getString("weather.lang");
		root["weather"]["unit"] = NVS.getString("weather.unit");

		root["cloud"]["mode"] = NVS.getString("cloud.mode");
		root["cloud"]["url"] = NVS.getString("cloud.url");
		root["cloud"]["token"] = NVS.getString("cloud.token");

		serializeJson(root, *response);
		request->send(response);
	});
}

void setupSettingsPost()
{
	server.on("/api/settings", HTTP_PUT, [](AsyncWebServerRequest *request) { /* nothing and dont remove it */ }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
		DynamicJsonDocument doc(2048);

		DeserializationError error = deserializeJson(doc, data);
		if (error) {
			Serial.print(F("deserializeJson() failed with code "));
			Serial.println(error.c_str());

			request->send(404, "text/plain", "");
		}
		else
		{
			NVS.setInt("device.angle", doc["device"]["angle"].as<unsigned int>());
			NVS.setString("device.theme", doc["device"]["theme"]);

			NVS.setInt("playlist.timer", doc["playlist"]["timer"].as<unsigned int>());

			NVS.setString("weather.api", doc["weather"]["api"]);
			NVS.setInt("weather.loc", doc["weather"]["location"].as<unsigned int>());
			NVS.setString("weather.lang", doc["weather"]["lang"]);
			NVS.setString("weather.unit", doc["weather"]["unit"]);

			NVS.setString("cloud.mode", doc["cloud"]["mode"]);
			NVS.setString("cloud.url", doc["cloud"]["url"]);
			NVS.setString("cloud.token", doc["cloud"]["token"]);

			NVS.commit();

			request->send(200, "application/ld+json; charset=utf-8", "{}");
		} });
}

/**
 *
 */
void setupCurrentImage()
{
	server.on("/current-image", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(SPIFFS, "/currentImage.bin");
	});
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

static void handle_update_progress_cb(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
	if (!index)
	{
		Serial.printf("UploadStart: %s\n", filename.c_str());
		ImageNew(0, 0, 0, 0, true);
	}

	ImageWriteBuffer(data, len);

	if (final)
	{
		Serial.printf("UploadEnd: %s, %u B\n", filename.c_str(), index + len);
		ImageFlushBuffer();

		updateDisplayRequired = true;
	}
}

void setupApiFace()
{
	server.on("/api/face", HTTP_POST, [](AsyncWebServerRequest *request) {
		Serial.println("post request");

		/*
		int params = request->params();
		for (int i = 0; i < params; i++)
		{
			AsyncWebParameter *p = request->getParam(i);
			if (p->isFile())
			{
				Serial.printf("<li>FILE[%s]: %s, size: %u</li>", p->name().c_str(), p->value().c_str(), p->size());
			}
			else if (p->isPost())
			{
				Serial.printf("<li>POST[%s]: %s</li>", p->name().c_str(), p->value().c_str());
			}
			else
			{
				Serial.printf("<li>GET[%s]: %s</li>", p->name().c_str(), p->value().c_str());
			}
		}

		if (request->hasParam("dithering", true))
		{
			AsyncWebParameter *p = request->getParam("dithering", true);
			Serial.println(p->value());
			dithering = p->value().toInt() == 1;
		}
		*/

		request->send(200, "application/ld+json; charset=utf-8", "{}");
	},
			  handle_update_progress_cb);
}

void setupApiUpdate()
{
	server.on("/api/update", HTTP_GET, [](AsyncWebServerRequest *request) {
		/*
		int params = request->params();
		for (int i = 0; i < params; i++)
		{
			AsyncWebParameter *p = request->getParam(i);
			if (p->isFile())
			{
				Serial.printf("<li>FILE[%s]: %s, size: %u</li>", p->name().c_str(), p->value().c_str(), p->size());
			}
			else if (p->isPost())
			{
				Serial.printf("<li>POST[%s]: %s</li>", p->name().c_str(), p->value().c_str());
			}
			else
			{
				Serial.printf("<li>GET[%s]: %s</li>", p->name().c_str(), p->value().c_str());
			}
		}
		*/

		if (request->hasParam("datetime"))
		{
			Serial.println("update datetime...");

			updateDateTime();
		}

		if (request->hasParam("weather"))
		{
			Serial.println("update weather data...");

			updateWeatherData();
		}

		if (request->hasParam("calendar"))
		{
			Serial.println("update calendar data...");

			updateCalendarData();
		}

		if (request->hasParam("file"))
		{
			Serial.println("file...");

			downloadFile(request->getParam("url")->value().c_str(), request->getParam("file")->value().c_str());
		}

		request->send(200, "application/ld+json; charset=utf-8", "{}");
	});
}