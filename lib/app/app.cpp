#include <SPIFFS.h>
#include <Update.h>
#include "app.h"
#include "ESPAsyncWebServer.h"
#include <ESPmDNS.h>
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

void setupApiDevice();
void setupApiSettings();
void setupApiWifi();
void setupApiUpdate();
void setupOTA();

//flag to use from web update to update display
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

	setupApiDevice();
	setupApiSettings();
	setupApiWifi();
	setupApiUpdate();
	setupOTA();

	server.onNotFound([](AsyncWebServerRequest *request) {
		request->send(404);
	});

	// TODO response
	server.on("/stats", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncResponseStream *response = request->beginResponseStream("application/json");
		DynamicJsonDocument doc(668); // https://arduinojson.org/v6/assistant/

		doc["wifi"]["mac"] = WiFi.macAddress();
		doc["wifi"]["ssid"] = WiFi.SSID();
		doc["wifi"]["connected"] = WiFi.isConnected();
		doc["wifi"]["rssi"] = WiFi.RSSI();
		doc["wifi"]["ip"] = WiFi.localIP().toString();
		doc["wifi"]["channel"] = WiFi.channel();
		doc["wifi"]["dns"] = WiFi.dnsIP().toString();
		doc["wifi"]["gateway"] = WiFi.gatewayIP().toString();

		doc["device"]["id"] = DeviceId;
		doc["device"]["time"] = time(NULL);
		doc["device"]["hostname"] = WiFi.getHostname();
		doc["device"]["bootCycle"] = deviceGetBootCount();
		doc["device"]["screen"]["width"] = 640;
		doc["device"]["screen"]["height"] = 384;

		doc["device"]["fs"]["total"] = SPIFFS.totalBytes();
		doc["device"]["fs"]["used"] = SPIFFS.usedBytes();
		doc["device"]["fs"]["free"] = SPIFFS.totalBytes() - SPIFFS.usedBytes();

		doc["device"]["heap"]["total"] = ESP.getHeapSize();
		doc["device"]["heap"]["free"] = ESP.getFreeHeap();
		doc["device"]["psram"]["total"] = ESP.getPsramSize();
		doc["device"]["psram"]["free"] = ESP.getFreePsram();

		doc["playlist"]["current"] = PlaylistGetCurrentFace();
		doc["playlist"]["remaining"] = (PlaylistGetRemainingTimeMs() / 1000) + 3; // + face rendering time 3s

		doc["firmware"]["created"] = FW_CREATED;
		doc["firmware"]["rev"] = FW_GIT_REV;

		JsonArray capability = doc.createNestedArray("capability");
		capability.add("jpg");
		//capability.add("wbmp");

		serializeJson(doc, *response);
		request->send(response);
	});

	// CORS
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
	//DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "content-type"));

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
		displayFlush();
	}
}

/**
 * api settings endpoint
 */
void setupApiSettings()
{
	server.on("/api/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncResponseStream *response = request->beginResponseStream("application/json");
		DynamicJsonDocument root(1024);

		root["system"]["country"] = NVS.getString("system.country");
		root["system"]["language"] = NVS.getString("system.language");
		root["system"]["timezone"] = NVS.getString("system.timezone");
		root["system"]["utc"] = NVS.getInt("system.utc");
		root["system"]["dst"] = NVS.getInt("system.dst");
		root["system"]["wifi"] = NVS.getString("wifi.ssid");

		// gmtOffset_sec
		// daylightOffset_sec

		//root["device"]["angle"] = NVS.getInt("device.angle");
		root["device"]["theme"] = NVS.getString("device.theme");
		root["device"]["name"] = NVS.getString("device.name");

		root["playlist"]["timer"] = NVS.getInt("playlist.timer");

		root["weather"]["api"] = NVS.getString("weather.api");
		root["weather"]["location"] = NVS.getInt("weather.loc");
		root["weather"]["lang"] = NVS.getString("weather.lang");
		root["weather"]["unit"] = NVS.getString("weather.unit");

		//root["cloud"]["mode"] = NVS.getString("cloud.mode");
		//root["cloud"]["url"] = NVS.getString("cloud.url");
		//root["cloud"]["token"] = NVS.getString("cloud.token");

		serializeJson(root, *response);
		request->send(response);
	});

	server.on(
		"/api/settings", HTTP_PUT, [](AsyncWebServerRequest *request) { /* nothing and dont remove it */ }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
		DynamicJsonDocument doc(2048);

		DeserializationError error = deserializeJson(doc, data);
		if (error) {
			Serial.print(F("deserializeJson() failed with code "));
			Serial.println(error.c_str());

			request->send(404, "text/plain", "");
		}
		else
		{
			JsonVariant system = doc["system"];
			if (!system.isNull()) {
				NVS.setString("system.country", system["country"]);
				NVS.setString("system.language", system["language"]);
				NVS.setString("system.timezone", system["timezone"]);
				NVS.setInt("system.utc", system["utc"].as<unsigned int>());
				NVS.setInt("system.dst", system["dst"].as<unsigned int>());
			}

			JsonVariant device = doc["device"];
			if (!device.isNull()) {
				NVS.setInt("device.angle", device["angle"].as<unsigned int>());
				NVS.setString("device.theme", device["theme"]);
				NVS.setString("device.name", device["name"]);
			}

			JsonVariant playlist = doc["playlist"];
			if (!playlist.isNull()) {
				NVS.setInt("playlist.timer", playlist["timer"].as<unsigned int>());
			}

			JsonVariant weather = doc["weather"];
			if (!doc["weather"].isNull()) {
				NVS.setString("weather.api", weather["api"]);
				NVS.setInt("weather.loc", weather["location"].as<unsigned int>());
				NVS.setString("weather.lang", weather["lang"]);
				NVS.setString("weather.unit", weather["unit"]);
			}
/*
			NVS.setString("cloud.mode", doc["cloud"]["mode"]);
			NVS.setString("cloud.url", doc["cloud"]["url"]);
			NVS.setString("cloud.token", doc["cloud"]["token"]);
			*/

			NVS.commit();

			request->send(200, "application/ld+json; charset=utf-8", "{}");
		} });
}

/**
 * api device endpoint
 */
void setupApiDevice()
{
	server.on("/api/device/restart", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(200, "application/json", "{}");

		ESP.restart();
	});

	server.on("/api/device/screen", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response = request->beginChunkedResponse("image/bmp", [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {
			return displaySnapshotBMPStream(buffer, maxLen, index);
		});

		response->addHeader("Content-Disposition", "inline; filename=capture.bmp");
		request->send(response);
	});

	server.on(
		"/api/device/screen", HTTP_POST, [](AsyncWebServerRequest *request) {
			AsyncResponseStream *response = request->beginResponseStream("application/json");
			DynamicJsonDocument doc(117); // https://arduinojson.org/v6/assistant/

			// todo
			doc["status"] = true;
			doc["image"]["format"] = "xxx";
			doc["image"]["width"] = 0;
			doc["image"]["height"] = 0;

			serializeJson(doc, *response);
			request->send(response); },
		[](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
			if (!index)
			{
				Serial.printf("UploadStart: %s\n", filename.c_str());
				bool dither = strcmp(filename.c_str(), "dithering") == 0;

				ImageNew(0, 0, 0, 0, dither);
				PlaylistResetTimer();
			}

			ImageWriteBuffer(data, len);

			if (final)
			{
				Serial.printf("UploadEnd: %s, %u B\n", filename.c_str(), index + len);
				ImageFlushBuffer();

				updateDisplayRequired = true;
			}
		});

	server.on("/api/device/scan", HTTP_GET, [](AsyncWebServerRequest *request) {
		String json = "[";
		Serial.printf("Browsing for service _%s._%s.local. ... ", "http", "tcp");
		int n = MDNS.queryService("http", "tcp");
		if (n == 0)
		{
			Serial.println("no services found");
		}
		else
		{
			Serial.print(n);
			Serial.println(" service(s) found");

			size_t cnt = 0;
			for (size_t i = 0; i < n; ++i)
			{
				// checking for epd
				if (MDNS.hasTxt(i, "paperdash"))
				{
					if (cnt)
					{
						json += ",";
					}
					cnt++;

					json += "{";
					json += "\"host\":\"" + MDNS.hostname(i) + "\"";
					json += ",\"ip\":\"" + MDNS.IP(i).toString() + "\"";
					json += ",\"port\":" + String(MDNS.port(i));
					json += ",\"type\":\"" + String(MDNS.txt(i, 0)) + "\"";
					json += "}";
				}
			}
		}
		Serial.println();

		json += "]";
		request->send(200, "application/json", json);
		json = String();
	});
}

/**
 * api wifi endpoint
 */
void setupApiWifi()
{
	server.on("/api/wifi/scan", HTTP_GET, [](AsyncWebServerRequest *request) {
		String json = "[";

		int n = WiFi.scanNetworks();
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

		json += "]";
		request->send(200, "application/json", json);
		json = String();
	});

	server.on(
		"/api/wifi/connect", HTTP_POST, [](AsyncWebServerRequest *request) { /* nothing and dont remove it */ }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
		DynamicJsonDocument doc(1024);

		DeserializationError error = deserializeJson(doc, data);
		if (error) {
			Serial.print(F("deserializeJson() failed with code "));
			Serial.println(error.c_str());

			request->send(404, "application/ld+json; charset=utf-8", "{}");
		}
		else
		{
			JsonVariant ssid = doc["ssid"];
			if (!ssid.isNull()) {
				NVS.setString("wifi.ssid", ssid);
				Serial.println(ssid.as<char*>());
			}

			JsonVariant password = doc["password"];
			if (!password.isNull()) {
				NVS.setString("wifi.password", password);
				Serial.println(password.as<char*>());
			}

			request->send(200, "application/json; charset=utf-8", "{}");

			ESP.restart();
		} });
}

/**
 * api data endpoint
 * @todo
 */
void setupApiUpdate()
{
	server.on("/api/update", HTTP_GET, [](AsyncWebServerRequest *request) {
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

		if (request->getParam("url") && request->hasParam("file"))
		{
			Serial.println("file...");

			downloadFile(request->getParam("url")->value().c_str(), request->getParam("file")->value().c_str());
		}

		request->send(200, "application/ld; charset=utf-8", "{}");
	});
}

/**
 * api data endpoint
 * @todo
 */
void setupOTA()
{
	// Simple Firmware Update Form
	server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(200, "text/html", "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>");
	});

	server.on(
		"/update", HTTP_POST, [](AsyncWebServerRequest *request) { },
		[](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
			if (!index)
			{
				Serial.printf("Update Start: %s\n", filename.c_str());
				uint8_t flashType = 255;

				if (filename.equals("firmware.bin"))
				{
					Serial.println("firmware update");
					flashType = U_FLASH;
				}
				else if (filename.equals("spiffs.bin"))
				{
					Serial.println("spiffs update");
					flashType = U_SPIFFS;

					SPIFFS.end();
				}
				else
				{
					Serial.println("unkown, reject");
				}

				if (flashType != 255)
				{
					Serial.println("update start...");
					if (!Update.begin(UPDATE_SIZE_UNKNOWN, flashType))
					{
						Update.printError(Serial);
					}
				}
				else
				{
					Serial.println("unkown update type, reject");
				}
			}

			if (Update.isRunning())
			{
				if (!Update.hasError())
				{
					if (Update.write(data, len) != len)
					{
						Update.printError(Serial);
					}
				}

				if (final)
				{
					AsyncWebServerResponse *response = request->beginResponse(200, "application/json; charset=utf-8", !Update.hasError() ? "{\"success\": true}" : "{\"success\": false}");
					response->addHeader("Refresh", "20");
					response->addHeader("Location", "/");
					request->send(response);

					if (!Update.end(true))
					{
						Update.printError(Serial);
					}
					else
					{
						Serial.println("Update complete");
						Serial.flush();
						ESP.restart();
					}
				}
			}
		});
}