#include "wlan.h"
#include "settings.h"


void initClientMode(const char *ssid, const char *password);
void initAPMode();


void setupWlan()
{
	Serial.println("setup Wlan");
	WiFi.setHostname("paperdash-display");

	// load wifi settings
    String ssid = NVS.getString("wlan_ssid");
    String password = NVS.getString("wlan_password");


	if (true)
	{
		// client mode
		initClientMode(ssid.c_str(), password.c_str());
	}
	else
	{
		// ap mode
		initAPMode();
	}
}


void initClientMode(const char *ssid, const char *password)
{
	long startMills = millis();

	WiFi.mode(WIFI_STA);

	Serial.print("Connecting to ");
    Serial.println(ssid);

	WiFi.begin(ssid, password);
	Serial.println(millis() - startMills);


	while (WiFi.waitForConnectResult() != WL_CONNECTED)
	{
		// TODO count failed connecting
		// on x failed, auto start AP mode
		Serial.println("Connection Failed! Rebooting...");
		delay(100);
		ESP.restart();
	}

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

	Serial.print("connected in: ");
	Serial.println(millis() - startMills);

}


void initAPMode()
{

}


bool wlan_isAPMode()
{
	return false;
}


void disableWlan()
{
	Serial.println("disable Wlan");
	//esp_wifi_stop();
}