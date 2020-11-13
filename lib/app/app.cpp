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
void setupSettingsGet();
void setupSettingsPost();
void setupWifiScan();
void setupWifiConnect();
void setupCurrentImage();
void setupApiFace();
void setupApiUpdate();
void setupOTA();

//flag to use from web update to update display
bool updateDisplayRequired = false;

//flag to use from web update to reboot the ESP
bool shouldReboot = false;

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
	setupSettingsGet();
	setupSettingsPost();
	setupWifiScan();
	setupWifiConnect();
	setupCurrentImage();
	setupApiFace();
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
	if (shouldReboot)
	{
		Serial.println("Rebooting...");
		delay(100);
		ESP.restart();
	}

	if (updateDisplayRequired)
	{
		Serial.println("loop app update display");

		// stop playlist to show the new image
		PlaylistResetTimer();

		updateDisplayRequired = false;
		displayFlush();
	}
}

void setupSettingsGet()
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
}

void setupSettingsPost()
{
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
 * get current screen
 * @depricated
 */
void setupCurrentImage()
{
	server.on("/current-image2", HTTP_GET, [](AsyncWebServerRequest *request) {
		uint8_t q = 10;
		if (request->hasParam("q"))
		{
			q = request->getParam("q")->value().toInt();
		}

		displayPrintScreenJPG("/tmp2.jpeg", q);
		AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/tmp2.jpeg", "image/jpeg");

		request->send(response);
	});

	server.on("/current-image", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response = request->beginChunkedResponse("image/bmp", [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {
			//Write up to "maxLen" bytes into "buffer" and return the amount written.
			//index equals the amount of bytes that have been already sent
			//You will be asked for more data until 0 is returned
			//Keep in mind that you can not delay or yield waiting for more data!
			return displaySnapshotBMPStream(buffer, maxLen, index);
		});

		response->addHeader("Content-Disposition", "inline; filename=capture.bmp");
		request->send(response);
	});
}

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
 * scan for wifi
 */
void setupWifiScan()
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
}

/**
 * @todo
 */
void setupWifiConnect()
{
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

/*
			if (doc.containsKey("ssid")) {
				NVS.setString("wifi_ssid", doc["ssid"]);
				Serial.println(doc["ssid"].as<char*>());
			}
			if (doc.containsKey("password")) {
				NVS.setString("wifi_password", doc["password"]);
				Serial.println(doc["password"].as<char*>());
			}
			*/

			request->send(200, "application/ld+json; charset=utf-8", "{}");

			ESP.restart();
		} });
}

/**
 * @depricated
 */
static void handle_update_progress_cb(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
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
}

/**
 * @depricated
 */
void setupApiFace()
{
	server.on(
		"/api/face", HTTP_POST, [](AsyncWebServerRequest *request) {
			AsyncResponseStream *response = request->beginResponseStream("application/json");
			DynamicJsonDocument doc(117); // https://arduinojson.org/v6/assistant/

			// todo
			doc["status"] = true;
			doc["image"]["format"] = "xxx";
			doc["image"]["width"] = 0;
			doc["image"]["height"] = 0;

			//doc["jpg"]["comps"] = 0;

			/*
			Serial.println(JpegDec.width);
			Serial.println(JpegDec.height);

			Serial.print(F("Components :"));
			Serial.println(JpegDec.comps);
			Serial.print(F("MCU / row  :"));
			Serial.println(JpegDec.MCUSPerRow);
			Serial.print(F("MCU / col  :"));
			Serial.println(JpegDec.MCUSPerCol);
			Serial.print(F("Scan type  :"));
			Serial.println(JpegDec.scanType);
			Serial.print(F("MCU width  :"));
			Serial.println(JpegDec.MCUWidth);
			Serial.print(F("MCU height :"));
			Serial.println(JpegDec.MCUHeight);
			*/

			serializeJson(doc, *response);
			request->send(response);

			//request->send(200, "application/ld+json; charset=utf-8", "{}");
		},
		handle_update_progress_cb);
}

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

		request->send(200, "application/ld+json; charset=utf-8", "{}");
	});
}

void setupOTA()
{
	// Simple Firmware Update Form
	/*
	server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
		// TODO in die pwa auslagern
		request->send(200, "text/html", "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>");
	});
	*/

	server.on(
		"/update", HTTP_POST, [](AsyncWebServerRequest *request) {
			shouldReboot = !Update.hasError();
			AsyncWebServerResponse *response = request->beginResponse(200, "application/ld+json; charset=utf-8", shouldReboot ? "{\"success\": true}" : "{\"success\": false}");

			response->addHeader("Connection", "close");
			request->send(response); },
		[](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
			if (!index)
			{
				Serial.printf("Update Start: %s\n", filename.c_str());
				// bool canBegin = Update.begin(contentLength, U_FLASH);
				// bool canBegin = Update.begin(contentLength, U_SPIFFS);
				if (!Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000))
				{
					Update.printError(Serial);
				}
			}

			if (!Update.hasError())
			{
				if (Update.write(data, len) != len)
				{
					Update.printError(Serial);
				}
			}

			if (final)
			{
				if (Update.end(true))
				{
					Serial.printf("Update Success: %uB\n", index + len);
				}
				else
				{
					Update.printError(Serial);
				}
			}
		});
}