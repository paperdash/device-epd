#include "device.h"
#include "settings.h"

#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
RTC_DATA_ATTR unsigned int bootCount = 0;
RTC_DATA_ATTR long config_DeepSleepInterval = 10; // sec

unsigned long bootTime = 0;
char DeviceId[21 + 1];

// private methods
void sleepDevice();
bool isBootTimeOver();

void setupDevice()
{
	// increment boot number and print it every reboot
	bootCount++;
	bootTime = millis();

	// create anonymous device id
	sprintf(DeviceId, "%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X", UniqueID8[0], UniqueID8[1], UniqueID8[2], UniqueID8[3], UniqueID8[4], UniqueID8[5], UniqueID8[6], UniqueID8[7]);
}

void loopDevice()
{
	// NOP
}

void sleepDevice()
{
	Serial.println("Going to sleep now");
	Serial.flush();

	esp_sleep_enable_timer_wakeup(config_DeepSleepInterval * uS_TO_S_FACTOR);
	esp_deep_sleep_start();
}

void deviceSetSleepInterval(long interval)
{
	config_DeepSleepInterval = interval;
}

long deviceGetSleepInterval()
{
	return config_DeepSleepInterval;
}

unsigned int deviceGetBootCount()
{
	return bootCount;
}

bool deviceIsConfigured()
{
	return !(NVS.getString("wifi.ssid").isEmpty() || NVS.getString("device.name").isEmpty() || NVS.getString("device.theme").isEmpty());
}

bool isBootTimeOver()
{
	return millis() - bootTime >= 60000;
}

String getWakeupReason()
{
	esp_sleep_wakeup_cause_t wakeup_reason;
	wakeup_reason = esp_sleep_get_wakeup_cause();

	//return String(wakeup_reason);

	switch (wakeup_reason)
	{
	case 1:
		return String("Wakeup caused by external signal using RTC_IO");
	case 2:
		return String("Wakeup caused by external signal using RTC_CNTL");
	case 3:
		return String("Wakeup caused by timer");
	case 4:
		return String("Wakeup caused by touchpad");
	case 5:
		return String("Wakeup caused by ULP program");
	default:
		return String("Wakeup was not caused by deep sleep: " + String(wakeup_reason));
	}

	return String("unkown");
}