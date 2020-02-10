#include <Arduino.h>
#include <pgmspace.h>

//#include "uuid.h"
//#include "wlan.h"
//#include "ota.h"
#include "display.h"
#include <HTTPClient.h>

// SMART SIGN CONFIG ========
#define config_PullServer "http://smart-sign-server/satellite/get-data" // pull server address
RTC_DATA_ATTR long config_DeepSleepInterval = 300;						// 5 min pull intervall
String config_UUID = "";
// SMART SIGN CONFIG ========

// runtime data
const char *setting_HeaderKeys[] = {
	"DeepSleepInterval" // update deep sleep interval
	,
	"DisplayImage" // images for display...
	,
	"UpdateFirmware" // execute firmware update
};
long pullCount = 0;

#include <GxEPD2_BW.h>
#define FRAME_BUFFERBUFFE_SIZE GxEPD2_750::WIDTH *GxEPD2_750::HEIGHT / 8
PROGMEM unsigned char displayImageBuffer[FRAME_BUFFERBUFFE_SIZE];

HTTPClient http;

void setupCloud()
{
	http.useHTTP10(true); // http1.1 chunked übertragung funktioniert irgendwie nicht
	http.setTimeout(7000);
	http.collectHeaders(setting_HeaderKeys, sizeof(setting_HeaderKeys) / sizeof(char *));
}


void loopCloud()
{

}


/**
 * 1. neue config daten über den http header laden
 * 2. neues bild vom server laden und anzeigen sofern vorhanden
 * @return bool true on new data to display
 */
bool pullData()
{

	String pullUrl = String(config_PullServer) + "/" + config_UUID; // + "?deep-sleep=" + String(config_DeepSleepInterval) + "&wakeup=" + getWakeupReason();

	Serial.println(pullUrl);
	http.begin(pullUrl);
	int httpCode = http.GET();
	if (httpCode != HTTP_CODE_OK && httpCode != HTTP_CODE_NOT_MODIFIED)
	{
		Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode != HTTP_CODE_OK && httpCode).c_str());
	}
	else
	{
		// update poll interval
		String DeepSleepInterval = http.header("DeepSleepInterval");
		if (DeepSleepInterval.toInt() == 0)
		{
			// TODO disable deep sleep
			config_DeepSleepInterval = DeepSleepInterval.toInt();
		}
		else if (DeepSleepInterval.toInt() > 5 && DeepSleepInterval.toInt() != config_DeepSleepInterval)
		{
			// TODO
			Serial.println("###### config update");
			Serial.println("   set deep sleep interval from: " + String(config_DeepSleepInterval) + " to " + DeepSleepInterval);
			Serial.println("###### config update");

			config_DeepSleepInterval = DeepSleepInterval.toInt();
			//setupDeepSleep();
		}


		// update to new firmware
		String UpdateFirmware = http.header("UpdateFirmware");
		if (UpdateFirmware.length() > 0)
		{
			Serial.println("TODO update firmware...");
		}


		if (httpCode == HTTP_CODE_OK)
		{
			// TODO update image

			// get lenght of document (is -1 when Server sends no Content-Length header)
			int len = http.getSize();

			// create buffer for read
			uint8_t buff[128] = {0};

			// get tcp stream
			WiFiClient *stream = http.getStreamPtr();

			// reset image buffer
			memset(displayImageBuffer, 0, sizeof(displayImageBuffer));
			int imageBufferOffset = 0;

			// read all data from server
			while (http.connected() && (len > 0 || len == -1))
			{

				// get available data size
				size_t size = stream->available();

				if (size)
				{
					// read up to 128 byte
					int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

					for (int i = 0; i < c; i++)
					{
						// write to display buffer
						displayImageBuffer[imageBufferOffset] = buff[i];
						imageBufferOffset++;

						// update display if image buffer is full
						if (imageBufferOffset == FRAME_BUFFERBUFFE_SIZE)
						{
							// TODO update display?
							Serial.printf("TODO update display");

							// update...
							updateDisplay_Neu(displayImageBuffer);

							// reset image buffer
							memset(displayImageBuffer, 0, sizeof(displayImageBuffer));
							imageBufferOffset = 0;

							break;
						}
					}

					if (len > 0)
					{
						len -= c;
					}
				}

				delay(1);
			}

			return true;
		}
	}

	return false;
}