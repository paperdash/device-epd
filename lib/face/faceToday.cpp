#include "faceToday.h"
#include "settings.h"
#include "display.h"
#include "datetime.h"
#include "imageService.h"
#include "faceWeather.h"
#include "faceWeather/icons.h"

#include <Fonts/FreeMono24pt7b.h>
#include <Fonts/FreeMono18pt7b.h>

void addTodayBackgroundPicture();
void addTodayDay();
void addTodayWeather();

const char faceTodayPicture[] = "/faceToday.jpg";
unsigned long faceTodayUpdate = 0;

void setupFaceToday()
{
	// NOP
}

void loopFaceToday()
{
	// update every 10 min
	if ((millis() - faceTodayUpdate) >= 600000)
	{
		Serial.println(&now, "update today data @ %A, %B %d %Y %H:%M:%S");
		invalidFaceTodayCache(true);
	}
}

void showFaceToday()
{
	// init
	GFXcanvas1 *canvas = displayGetCanvas();
	canvas->fillScreen(COLOR_BG);
	canvas->setTextColor(COLOR_FG);
	canvas->setTextSize(1);

	// add widgets
	imageServiceRenderFile(faceTodayPicture, 0, 0, 0, 0, true);
	addTodayDay();
	addTodayWeather();

	// update display
	displayFlush();
}

void invalidFaceTodayCache(bool warmUp)
{
	if (warmUp)
	{
		imageServiceUpdateFile("640x384.jpg", faceTodayPicture);
		faceTodayUpdate = millis();
	}
	else
	{
		faceTodayUpdate = 0;
	}
}

void addTodayDay()
{
	// init
	GFXcanvas1 *canvas = displayGetCanvas();
	canvas->setFont(&FreeMono24pt7b);
	canvas->setTextColor(COLOR_BG);

	// add tile
	int16_t x1, y1;
	uint16_t w, h;
	int16_t time_base_y = 50;
	int16_t time_base_x = 15;
	canvas->getTextBounds("00", time_base_x, time_base_y, &x1, &y1, &w, &h);
	canvas->fillRoundRect(-14, -14, x1 + w + 25 + 15, time_base_y + (h * 2) + 5 + 15, 15, COLOR_BG); // add border
	canvas->fillRoundRect(-15, -15, x1 + w + 25 + 15, time_base_y + (h * 2) + 5 + 15, 15, COLOR_FG);

	// 1. row - day
	canvas->setCursor(time_base_x, time_base_y);
	canvas->print(now.tm_mday);
	canvas->setCursor(canvas->getCursorX() - 10, canvas->getCursorY());
	canvas->print(".");

	// 2. row - month
	char label[20];
	strftime(label, 20, "%b", &now);
	canvas->setCursor(time_base_x, time_base_y + h + 10);
	canvas->setFont(&FreeMono18pt7b);
	canvas->println(label);
}

void addTodayWeather()
{
	// init
	GFXcanvas1 *canvas = displayGetCanvas();
	canvas->setTextColor(COLOR_BG);

	// set start point
	int16_t x = displayGetWidth() - 150;
	int16_t y = displayGetHeight() - 60;

	// add tile
	canvas->fillRoundRect(x - 1, y - 1, 150 + 15, 60 + 15, 15, COLOR_BG); // add border
	canvas->fillRoundRect(x, y, 150 + 15, 60 + 15, 15, COLOR_FG);

	// current weather condition - icon
	const unsigned char *icon = getIconById(weatherData.current_icon, 64);
	if (icon)
	{
		canvas->drawBitmap(x + 15, y, icon, 64, 64, COLOR_FG, COLOR_BG);
	}

	// current weather condition - temperature
	canvas->setFont(&FreeMono24pt7b);
	canvas->setTextSize(1);

	int16_t x1, y1;
	uint16_t w, h;
	int16_t textbox_x = x + 64 + 5;
	int16_t textbox_y = y + 72;
	canvas->getTextBounds("000", textbox_x, textbox_y, &x1, &y1, &w, &h);

	canvas->setCursor(x1, y1);
	canvas->print(weatherData.current_temp);
}
