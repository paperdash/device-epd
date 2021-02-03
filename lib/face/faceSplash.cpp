#include "faceSplash.h"
#include "settings.h"
#include "display.h"
#include <WString.h>

#include <Roboto_Bold_40.h>

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
	String name = NVS.getString("device.name");

	if (name.isEmpty())
	{
		name = "paperdash";
	}

	// create message
	String welcome = "Hello ";
	welcome += name;

	// setup
	canvas->setFont(&Roboto_Bold_40);
	canvas->setRotation(0);
	canvas->fillScreen(COLOR_BG);
	canvas->setTextColor(COLOR_FG);

	// center bounding box by transposition of origin:
	int16_t tbx, tby;
	uint16_t tbw, tbh;
	canvas->getTextBounds(welcome, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t x = ((canvas->width() - tbw) / 2) - tbx;
	uint16_t y = ((canvas->height() - tbh) / 2) - tby;
	int16_t welcomeWidth = tbw;

	// print text
	canvas->setCursor(x, y);
	canvas->print(welcome);

	// add name underline
	canvas->getTextBounds("Hello ", 0, 0, &tbx, &tby, &tbw, &tbh);
	canvas->fillRect(x + tbw + 10, y + 8, welcomeWidth - tbw - 4, 4, COLOR_FG);

	// update screen
	displayFlush();
}