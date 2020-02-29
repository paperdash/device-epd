#include <Arduino.h>
#include <pgmspace.h>
#include <HTTPClient.h>
#include <SPIFFS.h>

#include "settings.h"
#include "device.h"
#include "display.h"

#include "imageWBMP.h"
#include "imagePNG.h"

unsigned long requestInterval = 1000; // 1 sec
unsigned long previousTime = 0;

// runtime data
const char *setting_HeaderKeys[] = {
	// update deep sleep interval
	"X-DeepSleepInterval",
	// execute firmware update url
	"X-UpdateFirmware"};

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

	if (isCloudSetupComplete())
	{
		//config_Url = NVS.getString("cloud_server"); // + "/" + NVS.getString("cloud_uuid");
		//config_Url = "http://paperdash.sonic.da-tom.com/api/device/22805938-2280-8022-3822-385980225980/image.png";
		Serial.println("  cloud setup complete");
	}

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
	return NVS.getString("cloud.url") != ""; // && NVS.getString("cloud_uuid") != "";
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
	String config_Url = NVS.getString("cloud.url");

	Serial.println(config_Url);
	http.begin(config_Url);
	int httpCode = http.GET();
	if (httpCode != HTTP_CODE_OK && httpCode != HTTP_CODE_NOT_MODIFIED)
	{
		Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode != HTTP_CODE_OK && httpCode).c_str());
	}
	else
	{
		// update poll interval
		String DeepSleepInterval = http.header("X-DeepSleepInterval");
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

			// track duration
			long startMills = millis();

			// get lenght of document (is -1 when Server sends no Content-Length header)
			int len = http.getSize();

			// create buffer for read
			uint8_t buff[128] = {0};

			// get tcp stream
			WiFiClient *stream = http.getStreamPtr();

			// reset image buffer
			size_t imageFormat = 0;

			// persist image to display
			File file = SPIFFS.open("/currentImage.tmp", FILE_WRITE);
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

					// initial detect format
					if (imageFormat == 0)
					{
						if (memcmp(buff, ImageHeaderWBMP, sizeof(ImageHeaderWBMP) - 1) == 0)
						{
							Serial.println(" image format: WBMP");
							imageFormat = 2;

							wbmpOpenFramebuffer();
						}
						else if (memcmp(buff, ImageHeaderPNG, sizeof(ImageHeaderPNG) - 1) == 0)
						{
							Serial.println(" image format: PNG");
							imageFormat = 3;

							pngOpenFramebuffer();
						}
						else
						{
							imageFormat = 1;
							Serial.println(" unkown image format. first header are:");
							Serial.println(buff[0]);
							Serial.println(buff[1]);
							Serial.println(buff[2]);
							Serial.println(buff[3]);
							Serial.println(buff[4]);
							Serial.println(buff[5]);
						}
					}

					// write display frame
					switch (imageFormat)
					{
					// WBMP
					case 2:
						wbmpWriteFramebuffer(0, buff, c);
						break;
					// PNG
					case 3:
						pngWriteFramebuffer(0, buff, c);
						break;
					}

					if (len > 0)
					{
						len -= c;
					}
				}

				delay(1);
			}

			// done
			if (file)
			{
				file.close();
				SPIFFS.remove("/currentImage.bin");
				SPIFFS.rename("/currentImage.tmp", "/currentImage.bin");
			}

			// update display
			switch (imageFormat)
			{
			// WBMP
			case 2:
				wbmpFlushFramebuffer();
				break;
			// PNG
			case 3:
				pngFlushFramebuffer();
				break;
			}

			Serial.print("update completed in: ");
			Serial.println(millis() - startMills);
		}
	}

	// clean up
	http.end();
}