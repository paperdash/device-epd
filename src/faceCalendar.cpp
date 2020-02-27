#include "faceCalendar.h"
#include "display.h"
#include "datetime.h"
#include "SPIFFS.h"
#include "pngle.h"

#include <Fonts/FreeMono12pt7b.h>	 // weekday - month year
#include <Fonts/FreeSansBold24pt7b.h> // current day

void display_calender();
void display_picture();
void display_time();
void on_draw2(pngle_t *pngle, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t rgba[4]);

using std::uint8_t;
static constexpr int MAX_WIDTH = 640 - 250;
static constexpr int MAX_HEIGHT = 384;
static int16_t curRowDelta[MAX_WIDTH + 1];
static int16_t nextRowDelta[MAX_WIDTH + 1];

void setupFaceCalendar()
{
	Serial.println("setupFaceWeather");

	setupDateTime();

	display.setRotation(0);
	display.setFullWindow();
	display.firstPage();
	display.fillScreen(GxEPD_WHITE);

	// draw...
	//display_time();
	display_picture();
	display_calender();

	Serial.println("displayFlush");
	display.nextPage();

	// https://raw.githubusercontent.com/rgujju/paperdink/master/Images/full.jpg
	// https://github.com/rgujju/paperdink/blob/master/Software/paperd.ink/GUI.cpp
	// https://images.unsplash.com/photo-1580886349729-1bd109928600?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjExMDM0OH0&w=640&h=200&fm=png&fit=crop
	// https://images.unsplash.com/photo-1580886349729-1bd109928600?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjExMDM0OH0&w=390&h=384&fm=png&fit=crop&colorquant=2
	// final filter: &w=390&h=384&fm=png&fit=crop&duotone=000000,FFFFFF
	// display_weather

}

void loopFaceCalendar()
{
}

void display_calender()
{
	// init
	int16_t sideWidth = 250;
	int16_t tbx, tby;
	uint16_t tbw, tbh, x;
	char label[64];
	display.setTextColor(GxEPD_WHITE);
	display.setTextSize(1);
	display.setRotation(0);

	// left side
	display.fillRect(0, 0, sideWidth, display.height(), GxEPD_BLACK);

	// weekday
	strftime(label, 64, "%A", &now);
	display.setFont(&FreeMono12pt7b);
	display.getTextBounds(label, 0, 0, &tbx, &tby, &tbw, &tbh);
	x = ((sideWidth - tbw) / 2) - tbx;
	display.setCursor(x, 30);
	display.println(label);

	// today
	display.setFont(&FreeSansBold24pt7b);
	display.setTextSize(2);
	display.getTextBounds("29", 0, 0, &tbx, &tby, &tbw, &tbh);
	x = ((sideWidth - tbw) / 2) - tbx;
	display.setCursor(x, 120);
	display.println(now.tm_mday);

	// month yearh
	strftime(label, 64, "%B %Y", &now);
	display.setTextSize(1);
	display.setFont(&FreeMono12pt7b);
	display.getTextBounds(label, 0, 0, &tbx, &tby, &tbw, &tbh);
	x = ((sideWidth - tbw) / 2) - tbx;
	display.setCursor(x, 150);
	display.println(label);

	// weekday headline
	display.setFont(&FreeMonoBold9pt7b);
	display.setCursor(20, 192);
	display.println("Mo Tu We Th Fr Sa Su");

	display.setCursor(20, 220);

	// skip week days from previous month
	for (uint8_t d = 1; d < now.day_offset; d++)
	{
		display.print("   ");
	}

	for (uint8_t d = 1; d <= now.days_in_month; d++)
	{
		display.printf("%2d ", d);

		if ((d + now.day_offset - 1) % 7 == 0)
		{
			// new week
			display.println("");
			display.setCursor(20, display.getCursorY());
		}
	}

	// TODO current weather
	display.drawLine(15, 320, sideWidth - 15, 320, GxEPD_WHITE);
}

void display_picture()
{
	SPIFFS.begin();
	displayOpen();

	pngle_t *pngle = pngle_new();
	pngle_set_draw_callback(pngle, on_draw2);

	//File file = SPIFFS.open("/blackPNG.png", "r");
	File file = SPIFFS.open("/calendarPhoto.png", "r");
	if (!file)
	{
		Serial.println(" file not found");
	}

	// TODO check why a small buffer is not working correct
	char buff[1280] = {0};
	while (int c = file.readBytes(buff, sizeof(buff)))
	{
		pngle_feed(pngle, buff, c);
	}
	file.close();

	Serial.print("  width: ");
	Serial.print(pngle_get_width(pngle));
	Serial.print("  height: ");
	Serial.println(pngle_get_height(pngle));

	Serial.println("  read png done");
	pngle_destroy(pngle);
}

/**
 * Floyd-Steinberg-Algorithmus
 */
void on_draw2(pngle_t *pngle, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t rgba[4])
{
	uint8_t r = rgba[0]; // 0 - 255
	uint8_t g = rgba[1]; // 0 - 255
	uint8_t b = rgba[2]; // 0 - 255
	//uint8_t a = rgba[3]; // 0: fully transparent, 255: fully opaque

	int16_t gray = round(r * 0.3 + g * 0.59 + b * 0.11);
	int16_t blackOrWhite;

	// Add errors to color if there are
	gray += curRowDelta[x];

	if (gray <= 127)
	{
		blackOrWhite = 0;
	}
	else
	{
		blackOrWhite = 255;
	}

	int16_t oldPixel = gray;
	int16_t newPixel = blackOrWhite;

	int err = oldPixel - newPixel;

	if (x > 0)
		nextRowDelta[x - 1] += err * 3 / 16;
	nextRowDelta[x] += err * 5 / 16;
	nextRowDelta[x + 1] += err * 1 / 16;
	curRowDelta[x + 1] += err * 7 / 16;

	if (x == 0 && y > 0)
	{
		// new line
		memcpy(curRowDelta, nextRowDelta, sizeof(curRowDelta));
		memset(nextRowDelta, 0, sizeof(nextRowDelta));
	}

	displayWritePixel(x + 250, y, blackOrWhite);
}

void display_time()
{
	/*
	int16_t x1, y1;
	uint16_t w, h;

	//display time
	display.setFont(&FreeMonoBold9pt7b); // LARGE_FONT
	display.setTextSize(1);
	display.setTextColor(GxEPD_BLACK);
	int16_t time_base_y = 60;
	int16_t time_base_x = 25;
	display.getTextBounds("03", time_base_x, time_base_y, &x1, &y1, &w, &h); // 03 is arbitrary text to get the height and width
	display.fillRect(time_base_x - 10, time_base_y - h - 10, w + 15, time_base_y + h + 10, GxEPD_WHITE);

	display.setCursor(time_base_x, time_base_y);
	if (now.hour < 10)
	{
		display.print("0");
		display.print(now.hour);
	}
	else
	{
		display.println(now.hour);
	}

	display.setCursor(time_base_x, time_base_y + h + 10);
	if (now.min < 10)
	{
		display.print("0");
		display.print(now.min);
	}
	else
	{
		display.println(now.min);
	}
	*/
}
