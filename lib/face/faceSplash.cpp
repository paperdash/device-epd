#include "faceSplash.h"
#include "settings.h"
#include "display.h"
#include "datetime.h"

#include <Fonts/FreeMono24pt7b.h>

void setupFaceSplash()
{
	// NOP
}

void loopFaceSplash()
{
	// NOP
}

void showFaceSplash()
{
	// init
	GFXcanvas1 *canvas = displayGetCanvas();
	const char Hello[] = "Hello paperdash!";

	// setup
	canvas->setFont(&FreeMono24pt7b);
	canvas->setRotation(0);
	canvas->fillScreen(COLOR_BG);
	canvas->setTextColor(COLOR_FG);

	// center bounding box by transposition of origin:
	int16_t tbx, tby;
	uint16_t tbw, tbh;
	canvas->getTextBounds(Hello, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t x = ((canvas->width() - tbw) / 2) - tbx;
	uint16_t y = ((canvas->height() - tbh) / 2) - tby;

	// print text
	canvas->setCursor(x, y);
	canvas->print(Hello);

	// update screen
	displayFlush();
}