#include "faceCalendar.h"
#include "display.h"
#include "datetime.h"

void display_calender();
void display_lines();
void display_time();

void setupFaceCalendar()
{
	Serial.println("setupFaceWeather");

	setupDateTime();


	display.setRotation(0);
	display.setFullWindow();
	display.firstPage();
	display.fillScreen(GxEPD_WHITE);

	// draw...
	display_lines();
	display_calender();
	display_time();

	Serial.println("displayFlush");
	display.nextPage();


	// https://raw.githubusercontent.com/rgujju/paperdink/master/Images/full.jpg
	// https://github.com/rgujju/paperdink/blob/master/Software/paperd.ink/GUI.cpp
	// display_weather
}

void loopFaceCalendar()
{
}

void display_lines()
{
	// black area bottom
	display.fillRect(0, (display.height() / 3) * 2, display.width(), (display.height() / 3), GxEPD_BLACK);

	// vertical lines
	display.fillRect((display.width() / 4), 0, 5, (display.height() / 3) * 2, GxEPD_BLACK);
	display.fillRect((display.width() / 4), (display.height() / 3) * 2, 5, (display.height() / 3), GxEPD_WHITE);
}

/**
 * @todo make it better :)
 */
void display_calender()
{
	int16_t x1, y1;
	uint16_t w, h;
	display.setFont(&FreeMonoBold9pt7b);
	display.setTextSize(1);
	display.setTextColor(GxEPD_BLACK);
	int16_t calender_base_y = 40;
	int16_t calender_base_x = 120;
	display.setCursor(calender_base_x, calender_base_y);
	display.println("Mon   Tue   Wed   Thu   Fri   Sat   Sun");
	display.getTextBounds("Mon   Tue   Wed   Thu   Fri   Sat   Sun", calender_base_x, calender_base_y, &x1, &y1, &w, &h);
	uint8_t num_offset, print_valid = 0;
	uint8_t day = 1;
	for (uint8_t j = 0; j <= 5; j++)
	{
		for (uint8_t i = 1; i <= 7 && day <= 31; i++)
		{
			// you can hack around with this value to align your text properly based on what font, font size etc you are using
			num_offset = 21; // 21 is what works for me for the first 2 columns
			if (i >= 3 && i <= 7)
			{
				num_offset = 17; // then i need to reduce to 17
			}
			if (j == 0 && i == now.day_offset)
			{
				// start from the offset in the month, ie which day does 1st of the month lie on
				print_valid = 1;
			}
			if (print_valid)
			{
				display.setCursor(calender_base_x + (i * (w / 7)) - num_offset, calender_base_y + ((j + 1) * h) + ((j + 1) * 7));
				if (day == now.mday)
				{
					char str[3];
					sprintf(str, "%d", day);
					int16_t x2, y2;
					uint16_t w2, h2;
					display.getTextBounds(str, calender_base_x + (i * (w / 7)) - num_offset, calender_base_y + ((j + 1) * h) + ((j + 1) * 7), &x2, &y2, &w2, &h2);
					display.fillRect(x2 - 4, y2 - 4, w2 + 8, h2 + 8, GxEPD_BLACK);
					display.setTextColor(GxEPD_WHITE);
				}
				else
				{
					display.setTextColor(GxEPD_BLACK);
				}
				// once the offset is reached, start incrementing
				display.println(day);
				day += 1;
			}
		}
	}

	// display day
	display.setTextColor(GxEPD_WHITE);
	display.setFont(&FreeMonoBold9pt7b); // LARGE_FONT
	display.setTextSize(1);
	display.setCursor(33, 250);
	display.println(now.mday);

	// display month
	display.setTextColor(GxEPD_WHITE);
	display.setFont(&FreeMonoBold9pt7b); // MED_FONT
	display.setTextSize(2);
	display.setCursor(30, 290);
	display.println(now.month);
}

void display_time()
{
	int16_t x1, y1;
	uint16_t w, h;

	//display time
	display.setFont(&FreeMonoBold9pt7b);	// LARGE_FONT
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
}
