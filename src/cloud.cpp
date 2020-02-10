#include <Arduino.h>
#include <pgmspace.h>
#include <HTTPClient.h>


#include "display.h"
#include "device.h"


// TODO SMART SIGN CONFIG ========
#define config_PullServer "http://paperdash.sonic.da-tom.com/gateway.php/" // pull server address
String config_UUID = "22805938-2280-8022-3822-385980225980";				// TODO
// SMART SIGN CONFIG ========

// runtime data
const char *setting_HeaderKeys[] = {
	// update deep sleep interval
	"DeepSleepInterval",
	// execute firmware update url
	"UpdateFirmware"
};

//#include <GxEPD2_BW.h>
//#define FRAME_BUFFERBUFFE_SIZE GxEPD2_750::WIDTH *GxEPD2_750::HEIGHT / 8
//PROGMEM unsigned char displayImageBuffer[FRAME_BUFFERBUFFE_SIZE];

HTTPClient http;

void pullData();


void setupCloud()
{
	Serial.println("setup cloud");

	http.useHTTP10(true); // http1.1 chunked übertragung funktioniert irgendwie nicht
	http.setTimeout(7000);
	http.collectHeaders(setting_HeaderKeys, sizeof(setting_HeaderKeys) / sizeof(char *));

	Serial.println("setup cloud - done");
}

void loopCloud()
{
    pullData();
}

/**
 * 1. neue config daten über den http header laden
 * 2. neues bild vom server laden und anzeigen sofern vorhanden
 * @return bool true on new data to display
 */
void pullData()
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
		if (false && DeepSleepInterval.toInt() == 0)
		{
			// disable deep sleep
			Serial.println("###### deep sleep disabled");
			deviceSetSleepInterval(0);
		}
		else if (DeepSleepInterval.toInt() > 5 && DeepSleepInterval.toInt() != deviceGetSleepInterval())
		{
			// update config
			Serial.println("###### config update");
			Serial.println("   set deep sleep interval from: " + String(deviceGetSleepInterval()) + " to " + DeepSleepInterval);
			Serial.println("###### config update");

			deviceSetSleepInterval(DeepSleepInterval.toInt());
		}


		// update to new firmware
		String UpdateFirmware = http.header("UpdateFirmware");
		if (UpdateFirmware.length() > 0)
		{
			Serial.println("TODO update firmware...");
		}

		if (httpCode == HTTP_CODE_OK)
		{
			// update image

			// get lenght of document (is -1 when Server sends no Content-Length header)
			int len = http.getSize();

			// create buffer for read
			uint8_t buff[128] = {0};

			// get tcp stream
			WiFiClient *stream = http.getStreamPtr();

			// reset image buffer
			//memset(displayImageBuffer, 0, sizeof(displayImageBuffer));
			int imageBufferOffset = 0;
			displayOpenFramebuffer();

			// read all data from server
			while (http.connected() && (len > 0 || len == -1))
			{
				// get available data size
				size_t size = stream->available();

				if (size)
				{
					// read up to 128 byte
					int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

					displayWriteFramebuffer(imageBufferOffset, buff, c);
					imageBufferOffset += c;
/*
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
					*/

					if (len > 0)
					{
						len -= c;
					}
				}

				delay(1);
			}

			// done
			displayFlushFramebuffer();
		}
	}
}