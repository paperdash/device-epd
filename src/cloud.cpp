#include <Arduino.h>
#include <pgmspace.h>
#include <HTTPClient.h>
#include <SPIFFS.h>

#include "settings.h"
#include "display.h"
#include "device.h"

// TODO SMART SIGN CONFIG ========
//#define config_PullServer "http://paperdash.sonic.da-tom.com/gateway.php/" // pull server address
String config_UUID = "22805938-2280-8022-3822-385980225980"; // TODO

//#define config_PullServer = NVS.getString("cloud_server");
//String config_UUID = NVS.getString("cloud_server");
// SMART SIGN CONFIG ========

unsigned long requestInterval = 1000; // 1 sec
unsigned long previousTime = 0;

// runtime data
const char *setting_HeaderKeys[] = {
	// update deep sleep interval
	"DeepSleepInterval",
	// execute firmware update url
	"UpdateFirmware"};

HTTPClient http;

void requestCloud();
bool isCloudSetupComplete();
void updateInterval(unsigned long interval);

void setupCloud()
{
	Serial.println("setup cloud");

	updateInterval(10);

	SPIFFS.begin();

	http.useHTTP10(true); // http1.1 chunked übertragung funktioniert irgendwie nicht
	http.setTimeout(7000);
	http.collectHeaders(setting_HeaderKeys, sizeof(setting_HeaderKeys) / sizeof(char *));

	Serial.println("setup cloud - done");
}

void loopCloud()
{
	if (isCloudSetupComplete())
	{
		if (NVS.getString("device_mode") == "passive")
		{
			Serial.println("requestCloud()");

			Serial.println("passive, after this, we go to deep sleep and startover here again");
		}
		else
		{
			unsigned long currentTime = millis();

			if (currentTime - previousTime >= requestInterval)
			{
				/* Update the timing for the next time around */
				previousTime = currentTime;

				Serial.println("requestCloud()");
				requestCloud();
			}
		}

	}
}

bool isCloudSetupComplete()
{
	return NVS.getString("cloud_server") != "" && config_UUID != "";
}

/**
 * interval in seconds
 */
void updateInterval(unsigned long interval)
{
	//Serial.print("updateInterval: ");
	//Serial.println(interval);

	// update config
	Serial.println("###### config update");
	Serial.println("   set deep sleep interval from: " + String(deviceGetSleepInterval()) + " to " + interval);
	Serial.println("###### config update");


	// active wait state
	requestInterval = interval * 1000;

	// passive deep sleep state
	deviceSetSleepInterval(interval);
}

/**
 * 1. neue config daten über den http header laden
 * 2. neues bild vom server laden und anzeigen sofern vorhanden
 * @return bool true on new data to display
 */
void requestCloud()
{

	//String pullUrl = String(config_PullServer) + "/" + config_UUID; // + "?deep-sleep=" + String(config_DeepSleepInterval) + "&wakeup=" + getWakeupReason();
	String pullUrl = NVS.getString("cloud_server") + "/" + config_UUID; // + "?deep-sleep=" + String(config_DeepSleepInterval) + "&wakeup=" + getWakeupReason();

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
			//Serial.println("###### deep sleep disabled");
			//deviceSetSleepInterval(0);
		}
		else if (DeepSleepInterval.toInt() > 5 && DeepSleepInterval.toInt() != deviceGetSleepInterval())
		{
			updateInterval(DeepSleepInterval.toInt());
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

			// persist image to display
			File file = SPIFFS.open("/currentImage.bin", FILE_WRITE);
			if (!file)
			{
				Serial.println("Failed to open file for writing");
			}

			// read all data from server
			while (http.connected() && (len > 0 || len == -1))
			{
				// get available data size
				size_t size = stream->available();

				if (size)
				{
					// read up to 128 byte
					int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

					// write to storage
					if (file)
					{
						file.write(buff, c);
					}

					// write display frame
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

			if (file)
			{
				file.close();
			}

			// done
			displayFlushFramebuffer();
		}
	}
}