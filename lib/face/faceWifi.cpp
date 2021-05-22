#include "faceWifi.h"
#include "settings.h"
#include "display.h"
#include "faceWifi/imageSTA.h"

void setupFaceWifi()
{
	// NOP
}

void loopFaceWifi()
{
	// NOP
}

void showFaceWifi()
{
	// init
	GFXcanvas1 *canvas = displayGetCanvas();

	// render
	canvas->drawBitmap(0, 0, imageSTA, 640, 384, 0x0000, 0xFFFF);

	// update screen
	displayFlush();
}