#include <SPIFFS.h>
#include "imageService.h"
#include "settings.h"
#include "download.h"
#include "image.h"

void imageServiceRenderFile(const char imageFile[], int x, int y, int w, int h, bool dithering)
{
	File file = SPIFFS.open(imageFile, "r");
	if (!file)
	{
		Serial.println(" file not found");
	}

	ImageNew(x, y, w, h, dithering);

	// TODO check why a small buffer is not working correct
	uint8_t buff[1280] = {0};
	while (int c = file.read(buff, sizeof(buff)))
	{
		ImageWriteBuffer(buff, c);
	}
	file.close();

	ImageFlushBuffer();
}

bool imageServiceUpdateFile(const char format[], const char target[])
{
	String url = NVS.getString("playlist.images");

	if (!url.isEmpty())
	{
		url += format;
		return downloadFile(url, target);
	}

	return false;
}