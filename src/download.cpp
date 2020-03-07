#include <HTTPClient.h>
#include <SPIFFS.h>
#include "download.h"

HTTPClient http;

bool downloadFile(String url, const char *path)
{
	// @note duration time: 200kb = 35sec write to flash
	Serial.println("Download file: " + url);

	bool hasError = false;
	String tmpFile = path;
	tmpFile += ".tmp";

	http.useHTTP10(true); // http1.1 chunked übertragung funktioniert irgendwie nicht
	http.setTimeout(7000);

	http.begin(url);
	int httpCode = http.GET();
	if (httpCode != HTTP_CODE_OK)
	{
		Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode != HTTP_CODE_OK && httpCode).c_str());
		hasError = true;
	}
	else
	{
		// track duration
		long startMills = millis();

		// get lenght of document (is -1 when Server sends no Content-Length header)
		int len = http.getSize();

		// create buffer for read
		uint8_t buff[1024] = {0};

		// get tcp stream
		WiFiClient *stream = http.getStreamPtr();

		// persist image
		File file = SPIFFS.open(tmpFile, FILE_WRITE);
		if (!file)
		{
			Serial.println("Failed to open file for writing");
			hasError = true;
		}

		// read all data from server
		while (http.connected() && (len > 0 || len == -1))
		{
			// get available data size
			size_t size = stream->available();

			if (size)
			{
				// read up to xxx byte
				int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

				// write to storage
				if (file)
				{
					file.write(buff, c);
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
			SPIFFS.remove(path);
			SPIFFS.rename(tmpFile, path);
		}

		Serial.print("download completed in: ");
		Serial.println(millis() - startMills);
	}

	// clean up
	http.end();

	return !hasError;
}