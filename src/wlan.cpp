#include "wlan.h"
#include "settings.h"

const char *deviceName = "paperdash-display";
RTC_DATA_ATTR int wifiFailedCount = 0;

void initClientMode(const char *ssid, const char *password);
void initAPMode();

void setupWlan()
{
	Serial.println("setup Wlan");
	WiFi.setHostname(deviceName);

	// load wifi settings
	String ssid = NVS.getString("wifi_ssid");
	String password = NVS.getString("wifi_password");

	// TODO count failed connecting wifiFailedCount <=3
	if (!ssid.isEmpty() && !password.isEmpty()) // && wifiFailedCount <=3
	{
		// client mode
		initClientMode(ssid.c_str(), password.c_str());
	}
	else
	{
		// ap mode
		initAPMode();
	}

	Serial.println("setup Wlan - done");
}

void ___connectToNetwork()
{
	uint8_t veces = 10;
	WiFi.begin("ssid", "password");
	Serial.println("Establishing connection to WiFi..");
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(1000);
		Serial.println(veces);
		if (!veces--)
		{
			veces = 10;
			WiFi.disconnect();
			Serial.println("Wifi reset...");
			delay(1000);
			WiFi.begin("ssid", "password");
			Serial.println("Establishing connection to WiFi..");
		}
	}
	Serial.println("Connected to network");
}

void initClientMode(const char *ssid, const char *password)
{
	uint8_t veces = 10;
	long startMills = millis();

	Serial.print("  Connecting to: ");
	Serial.print(ssid);

	// print try count
	Serial.print(" (");
	Serial.print(wifiFailedCount);
	Serial.print("x)");

	// connecting
	WiFi.mode(WIFI_STA);


	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.println(veces);
		if (!veces--)
		{
			veces = 10;
			WiFi.disconnect();
			Serial.println("Wifi reset...");
			delay(500);
			WiFi.begin(ssid, password);
			Serial.println("Establishing connection to WiFi..");
		}
	}

	/*
	while (WiFi.waitForConnectResult() != WL_CONNECTED)
	{
		wifiFailedCount++;
		Serial.println("Connection Failed! Rebooting...");
		delay(100);
		ESP.restart();
	}
	*/

	Serial.println(" ...connected");
	Serial.print("  IP address: ");
	Serial.println(WiFi.localIP());

	Serial.print("  connected in: ");
	Serial.println(millis() - startMills);
}

void initAPMode()
{
	Serial.println("  init AP (Access Point)");

	WiFi.softAP("paperdash.io");

	IPAddress IP = WiFi.softAPIP();
	Serial.print("  AP IP address: ");
	Serial.println(IP);
}

bool wlan_isConnected()
{
	return WiFi.isConnected();
}