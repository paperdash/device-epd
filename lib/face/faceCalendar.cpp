#include <HTTPClient.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "faceCalendar.h"
#include "settings.h"
#include "display.h"
#include "datetime.h"
#include "image.h"
#include "imageService.h"
#include "download.h"
#include "faceWeather.h"
#include "faceWeather/icons.h"

#include <Fonts/FreeMono12pt7b.h>	  // weekday - month year
#include <Fonts/FreeMonoBold9pt7b.h>  //
#include <Fonts/FreeSans24pt7b.h>	  // current day
#include <Fonts/FreeSansBold24pt7b.h> // current day

void display_calender();

const char faceCalendarPicture[] = "/calendarPhoto.jpg";
unsigned long lastCalendarDataUpdate = 0;

void setupFaceCalendar()
{
	lastCalendarDataUpdate = millis();
}

void loopFaceCalendar()
{
	// update every 10 min
	if ((millis() - lastCalendarDataUpdate) >= 600000)
	{
		Serial.println(&now, "update calendar data @ %A, %B %d %Y %H:%M:%S");
		lastCalendarDataUpdate = millis();
		updateCalendarData();
	}
}

void showFaceCalendar()
{
	GFXcanvas1 *canvas = displayGetCanvas();

	canvas->setRotation(0);
	canvas->fillScreen(COLOR_FG);

	imageServiceRenderFile(faceCalendarPicture, 250, 0, 0, 0, true);
	display_calender();

	displayFlush();
}

/**
 * download and update calendar data
 */
bool updateCalendarData()
{
	return imageServiceUpdateFile("390x384.jpg", faceCalendarPicture);
}

void display_calender()
{
	// init
	GFXcanvas1 *canvas = displayGetCanvas();
	int16_t sideWidth = 250;
	int16_t tbx, tby;
	uint16_t tbw, tbh, x;
	char label[64];
	canvas->setTextColor(COLOR_FG);
	canvas->setTextSize(1);
	canvas->setRotation(0);

	// left side
	canvas->fillRect(0, 0, sideWidth, canvas->height(), COLOR_BG);

	// weekday
	strftime(label, 64, "%A", &now);
	canvas->setFont(&FreeMono12pt7b);
	canvas->getTextBounds(label, 0, 0, &tbx, &tby, &tbw, &tbh);
	x = ((sideWidth - tbw) / 2) - tbx;
	canvas->setCursor(x, 30);
	canvas->println(label);

	// today
	canvas->setFont(&FreeSansBold24pt7b);
	canvas->setTextSize(2);
	canvas->getTextBounds("29", 0, 0, &tbx, &tby, &tbw, &tbh);
	x = ((sideWidth - tbw) / 2) - tbx;
	canvas->setCursor(x, 120);
	canvas->println(now.tm_mday);

	// month yearh
	strftime(label, 64, "%B %Y", &now);
	canvas->setTextSize(1);
	canvas->setFont(&FreeMono12pt7b);
	canvas->getTextBounds(label, 0, 0, &tbx, &tby, &tbw, &tbh);
	x = ((sideWidth - tbw) / 2) - tbx;
	canvas->setCursor(x, 150);
	canvas->println(label);

	// weekday headline
	canvas->setFont(&FreeMonoBold9pt7b);
	canvas->setCursor(20, 192);
	canvas->println("Mo Tu We Th Fr Sa Su");

	canvas->setCursor(20, 220);

	// skip week days from previous month
	uint8_t skip = (now.day_offset == 0 ? 7 : now.day_offset);
	for (uint8_t d = 1; d < skip; d++)
	{
		canvas->print("   ");
	}

	for (uint8_t d = 1; d <= now.days_in_month; d++)
	{
		canvas->printf("%2d ", d);

		if ((d + now.day_offset - 1) % 7 == 0)
		{
			// new week
			canvas->println("");
			canvas->setCursor(20, canvas->getCursorY());
		}
	}

	// current weather
	canvas->drawLine(15, 320, sideWidth - 15, 320, COLOR_FG);

	// icon
	const unsigned char *icon = getIconById(weatherData.current_icon, 64);
	if (icon)
	{
		canvas->drawBitmap(72, 325, icon, 64, 64, COLOR_BG, COLOR_FG);
	}

	// temperature
	canvas->setFont(&FreeSans24pt7b);
	canvas->setTextSize(1);
	canvas->setCursor(150, 367);
	canvas->println(weatherData.current_temp);
}