#include <HTTPClient.h>
#include <SPIFFS.h>
#include <pngle.h>
#include "faceCalendar.h"
#include "display.h"
#include "datetime.h"
#include "tools.h"
#include "image.h"
#include "download.h"

#include <Fonts/FreeMono12pt7b.h>	 // weekday - month year
#include <Fonts/FreeSansBold24pt7b.h> // current day

void downloadRandomePicture();
void showFaceCalendar();
void display_calender();
void display_picture();
void display_time();

const char faceCalendarPicture[] = "/calendarPhoto.png";

void setupFaceCalendar()
{
}

void loopFaceCalendar()
{
	// TODO update picture every x seconds
	showFaceCalendar();
}

void showFaceCalendar()
{
	display.setRotation(0);
	display.setFullWindow();
	display.firstPage();
	display.fillScreen(GxEPD_WHITE);

	display_picture();
	display_calender();

	displayFlush();
}

void downloadRandomePicture()
{
	// https://images.unsplash.com/photo-1580886349729-1bd109928600?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjExMDM0OH0&w=640&h=200&fm=png&fit=crop
	// https://images.unsplash.com/photo-1580886349729-1bd109928600?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjExMDM0OH0&w=390&h=384&fm=png&fit=crop&colorquant=2
	// https://images.unsplash.com/photo-1581307385098-a0338263e357?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjExMDM0OH0&w=640&h=384&fm=png&fit=crop&duotone=000000,FFFFFF
	// final filter: &w=390&h=384&fm=png&fit=crop&duotone=000000,FFFFFF
	Serial.println("TODO download new image");

	// TODO download json file

	String pictureUrl = "https://images.unsplash.com/photo-1582910587039-b4f085805c86?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjExMDM0OH0";
	pictureUrl += "&w=390&h=384";			// size
	pictureUrl += "&fm=png";				// format
	pictureUrl += "&fit=crop";				// crop to needed size
	pictureUrl += "&duotone=000000,FFFFFF"; // grayscale to save bytes

	downloadFile(pictureUrl, faceCalendarPicture); // /face/calendar/bg.png

	Serial.println("TODO download new image ---- done");
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
