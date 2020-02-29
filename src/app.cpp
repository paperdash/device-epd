#include "app.h"
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"
//#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "settings.h"
#include "device.h"
#include "playlist.h"
#include "image.h"

AsyncWebServer server(80);

void setupSettingsGet();
void setupSettingsPost();
void setupWifiScan();
void setupWifiConnect();
void setupCurrentImage();
void setupApiFace();

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
	setupApiFace();

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

void setupSettingsGet()
{
	server.on("/api/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncResponseStream *response = request->beginResponseStream("application/json");
		DynamicJsonDocument root(1024);

		root["device"]["angle"] = NVS.getInt("device.angle");
		root["device"]["theme"] = NVS.getString("device.theme");

		root["playlist"]["timer"] = NVS.getInt("playlist.timer");

		root["api"]["owm"] = NVS.getString("api.owm");

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

			NVS.setString("api.owm", doc["api"]["owm"]);

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

		// stop playlist to show the new image
		PlaylistResetTimer();

		ImageNew(0, 0, 0, 0, true);
	}

	ImageWriteBuffer(data, len);

	if (final)
	{
		Serial.printf("UploadEnd: %s, %u B\n", filename.c_str(), index + len);
		ImageFlushBuffer();
	}
}

void setupApiFace()
{
	server.on("/api/face", HTTP_POST, [](AsyncWebServerRequest *request) {
		request->send(200, "application/ld+json; charset=utf-8", "{}");

	}, handle_update_progress_cb);
}