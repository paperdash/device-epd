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

	//NVS.setString("wifi_ssid", "xd-design.info");
	//NVS.setString("wifi_password", "SonicHome");

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


void initClientMode(const char *ssid, const char *password)
{
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
	while (WiFi.waitForConnectResult() != WL_CONNECTED)
	{
		wifiFailedCount++;
		Serial.println("Connection Failed! Rebooting...");
		delay(100);
		ESP.restart();
	}

    Serial.println(" ...connected");
    Serial.print("  IP address: ");
    Serial.println(WiFi.localIP());

	Serial.print("  connected in: ");
	Serial.println(millis() - startMills);
}


void initAPMode()
{
	Serial.println("  init AP (Access Point)");

	WiFi.softAP(deviceName);

	IPAddress IP = WiFi.softAPIP();
  	Serial.print("  AP IP address: ");
  	Serial.println(IP);
}


bool wlan_isConnected()
{
	return WiFi.isConnected();
}