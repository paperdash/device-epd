#include <HTTPClient.h>
#include <SPIFFS.h>
#include "download.h"
#include "device.h"

bool downloadFile(String url, const char *path, const char* CAcert)
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

	HTTPClient http;
	http.useHTTP10(true); // http1.1 chunked is not working correctly
	http.setTimeout(7000);
	http.setUserAgent("paperdash-epd");

	// use the last 8 bytes of the unique serial id
	http.addHeader("X-PaperDash-Id", DeviceId);

	if (CAcert) {
		http.begin(url, CAcert);
	} else {
		http.begin(url);
	}

	int httpCode = http.GET();
	if (httpCode != HTTP_CODE_OK)
	{
		Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
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


bool downloadFile(String url, const char *path)
{
	return downloadFile(url, path, NULL);
}