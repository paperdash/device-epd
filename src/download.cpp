#include <HTTPClient.h>
#include <SPIFFS.h>
#include "download.h"
#include "device.h"

HTTPClient http;

bool downloadFile(String url, const char *path)
{
	// @note duration time: 200kb = 35sec write to flash
	Serial.println("Download file: " + url);

	// tmp file
	bool hasError = false;
	String tmpFile = path;
	tmpFile += ".tmp";

	// track duration
	long startMills = millis();

	File file = SPIFFS.open(tmpFile, FILE_WRITE);
	if (!file)
	{
		Serial.println("Failed to open file for writing");
		return false;
	}

	http.useHTTP10(true); // http1.1 chunked is not working correctly
	http.setTimeout(7000);
	http.setUserAgent("paperdash esp");

	// use the last 8 bytes of the unique serial id
	http.addHeader("X-PaperDash-Id", DeviceId);

	http.begin(url);
	int httpCode = http.GET();
	if (httpCode != HTTP_CODE_OK)
	{
		Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode != HTTP_CODE_OK && httpCode).c_str());
		hasError = true;
	}
	else
	{
		http.writeToStream(&file);

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