#include <Arduino.h>
#include <WiFi.h>
#include "wlan.h"

const char* ssid = "";
const char* password = "";


void setupWlan()
{

  Serial.println("setup Wlan");
  long startMills = millis();

  WiFi.mode(WIFI_STA);
  WiFi.setHostname("smart-sign");
  WiFi.begin(ssid, password);
  Serial.println(millis() - startMills);

  //WiFi.config(IPAddress(192, 168, 178, 62), IPAddress(192, 168, 178, 1), IPAddress(192, 168, 178, 1), IPAddress(255, 255, 255, 0));
  Serial.println(millis() - startMills);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(100);
    ESP.restart();
  }
  Serial.println(millis() - startMills);

}

void disableWlan()
{
  Serial.println("disable Wlan");
  //esp_wifi_stop();
}