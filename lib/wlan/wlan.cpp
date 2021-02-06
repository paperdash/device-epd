#include "wlan.h"
#include "settings.h"
#include <DNSServer.h>

const char *deviceName = "paperdash-epd";
RTC_DATA_ATTR int wifiFailedCount = 0;

// DNS server
const byte DNS_PORT = 53;
DNSServer dnsServer;

/* Soft AP network parameters */
IPAddress apIP(192,178,4,1);

#include <DNSServer.h>

bool initClientMode(const char *ssid, const char *password);
void initAPMode();

void setupWlan()
{
	Serial.println("setup Wlan");

	// load wifi settings
	String ssid = NVS.getString("wifi.ssid");
	String password = NVS.getString("wifi.password");

	bool clientMode = false;
	if (!ssid.isEmpty() && !password.isEmpty() && wifiFailedCount <=3)
	{
		// try client mode
		clientMode = initClientMode(ssid.c_str(), password.c_str());
	}

	if (!clientMode)
	{
		// ap mode
		initAPMode();
	}

	WiFi.setHostname(deviceName);

	Serial.println("setup Wlan - done");
}


void loopWlan()
{
	// DNS
	dnsServer.processNextRequest();
}

bool initClientMode(const char *ssid, const char *password)
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
			WiFi.disconnect();
			wifiFailedCount++;
			if (wifiFailedCount > 3) {
				Serial.println("  wifi is not reachable...");
				return false;
			} else {
				tryCount = 5;
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

	return true;
}

void initAPMode()
{
	Serial.println("  init AP (Access Point)");

  	WiFi.mode(WIFI_AP);
	WiFi.softAP("paperdash");

	// prevent esp from crashing
	delay(2000);

	WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

	// redirect all to local ap
  	dnsServer.start(DNS_PORT, "*", apIP);

	IPAddress IP = WiFi.softAPIP();
	Serial.print("  AP IP address: ");
	Serial.println(IP);
}