#include <HTTPClient.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "faceCalendar.h"
#include "display.h"
#include "datetime.h"
#include "image.h"
#include "download.h"
#include "faceWeather.h"
#include "faceWeatherIcons.h"

#include <Fonts/FreeMono12pt7b.h>	 // weekday - month year
#include <Fonts/FreeMonoBold9pt7b.h> //
#include <Fonts/FreeSans24pt7b.h>	 // current day
#include <Fonts/FreeSansBold24pt7b.h> // current day

void showFaceCalendar();
void display_calender();
void display_picture();
void display_time();

const char faceCalendarPicutreJson[] = "/calendarPhoto.json";
const char faceCalendarPicture[] = "/calendarPhoto.jpg";
//const char faceCalendarPictureUrl[] = "http://us-central1-paperdash-io.cloudfunctions.net/apiImageOnRequest/390x384.jpg"; // TODO load from settings
const char faceCalendarPictureUrl[] = "https://paperdash.io/api/image/390x384.jpg"; // TODO load from settings

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

void playlistFaceCalendar()
{
	showFaceCalendar();
}

void showFaceCalendar()
{
	GFXcanvas1 *canvas = displayGetCanvas();

	canvas->setRotation(0);
	canvas->fillScreen(GxEPD_WHITE);

	display_picture();
	display_calender();

	displayFlush();
}

/**
 * download and update calendar data
 */
bool updateCalendarData()
{
	String url = NVS.getString("service.image");

	// TODO
	if (url) {
		url += "390x384.jpg"
	}
	// url += "APPID=" + NVS.getString("service.image");
	// char faceCalendarPictureUrl[] = NVS.getString("service.image");

	return downloadFile(faceCalendarPictureUrl, faceCalendarPicture);
}

void display_calender()
{
	// init
	GFXcanvas1 *canvas = displayGetCanvas();
	int16_t sideWidth = 250;
	int16_t tbx, tby;
	uint16_t tbw, tbh, x;
	char label[64];
	canvas->setTextColor(GxEPD_WHITE);
	canvas->setTextSize(1);
	canvas->setRotation(0);

	// left side
	canvas->fillRect(0, 0, sideWidth, canvas->height(), GxEPD_BLACK);

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
	canvas->drawLine(15, 320, sideWidth - 15, 320, GxEPD_WHITE);

	// icon
	const unsigned char *icon = getIconById(weatherData.current_icon, 64);
	if (icon)
	{
		canvas->drawBitmap(72, 325, icon, 64, 64, GxEPD_BLACK, GxEPD_WHITE);
	}

	// temperature
	canvas->setFont(&FreeSans24pt7b);
	canvas->setTextSize(1);
	canvas->setCursor(150, 367);
	canvas->println(weatherData.current_temp);
}

void display_picture()
{
	File file = SPIFFS.open(faceCalendarPicture, "r");
	if (!file)
	{
		Serial.println(" file not found");
	}

	ImageNew(250, 0, 0, 0, true);

	// TODO check why a small buffer is not working correct
	uint8_t buff[1280] = {0};
	while (int c = file.read(buff, sizeof(buff)))
	{
		ImageWriteBuffer(buff, c);
	}
	file.close();

	ImageFlushBuffer();
}

void display_time()
{
	/*
	int16_t x1, y1;
	uint16_t w, h;

	//display time
	canvas->setFont(&FreeMonoBold9pt7b); // LARGE_FONT
	canvas->setTextSize(1);
	canvas->setTextColor(GxEPD_BLACK);
	int16_t time_base_y = 60;
	int16_t time_base_x = 25;
	canvas->getTextBounds("03", time_base_x, time_base_y, &x1, &y1, &w, &h); // 03 is arbitrary text to get the height and width
	canvas->fillRect(time_base_x - 10, time_base_y - h - 10, w + 15, time_base_y + h + 10, GxEPD_WHITE);

	canvas->setCursor(time_base_x, time_base_y);
	if (now.hour < 10)
	{
		canvas->print("0");
		canvas->print(now.hour);
	}
	else
	{
		canvas->println(now.hour);
	}

	canvas->setCursor(time_base_x, time_base_y + h + 10);
	if (now.min < 10)
	{
		canvas->print("0");
		canvas->print(now.min);
	}
	else
	{
		canvas->println(now.min);
	}
	*/
}
