#include "wlan.h"
#include "settings.h"

const char *deviceName = "paperdash-display";
RTC_DATA_ATTR int wifiFailedCount = 0;

void initClientMode(const char *ssid, const char *password);
void initAPMode();

void setupWlan()
{
	Serial.println("setup Wlan");

	// load wifi settings
	String ssid = NVS.getString("wifi.ssid");
	String password = NVS.getString("wifi.password");

	if (!ssid.isEmpty() && !password.isEmpty() && wifiFailedCount <=3)
	{
		// client mode
		initClientMode(ssid.c_str(), password.c_str());
	}
	else
	{
		// ap mode
		initAPMode();
	}

	WiFi.setHostname(deviceName);

	Serial.println("setup Wlan - done");
}

void initClientMode(const char *ssid, const char *password)
{
	uint8_t tryCount = 5;
	long startMills = millis();

	Serial.print("  Connecting to: ");
	Serial.print(ssid);
	Serial.print(" ");

	// connecting
	WiFi.mode(WIFI_STA);


	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
		if (!tryCount--)
		{
			// TODO is this correct?
			wifiFailedCount++;
			if (wifiFailedCount > 3) {
				Serial.println("  wifi is not reachable...");
				initAPMode();
				return;
			} else {
				tryCount = 5;
				WiFi.disconnect();
				Serial.println("  wifi reset...");
				delay(500);
				WiFi.begin(ssid, password);
			}
		}
	}

	Serial.println(" ...connected");
	Serial.print("  IP address: ");
	Serial.println(WiFi.localIP());

    if (!MDNS.begin(deviceName)) {
        Serial.println("Error setting up MDNS responder!");
    }

	// Add service to MDNS-SD
	MDNS.addService("http", "tcp", 80);
	MDNS.addServiceTxt("http", "tcp", "paperdash", "epd");

	Serial.print("  connected in: ");
	Serial.println(millis() - startMills);
}

void initAPMode()
{
	Serial.println("  init AP (Access Point)");

	//WiFi.disconnect();
	WiFi.softAP("paperdash.io");

	IPAddress IP = WiFi.softAPIP();
	Serial.print("  AP IP address: ");
	Serial.println(IP);
}