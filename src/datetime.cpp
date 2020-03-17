#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "datetime.h"

RTC_DATA_ATTR struct datetime_struct now;

// TODO offset
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

unsigned long lastUpdate = 0;

int getNumberOfDays(int month, int year)
{
	// leap year condition, if month is 2
	if (month == 2)
	{
		if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0))
			return 29;
		else
			return 28;
	}
	// months which has 31 days
	else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
		return 31;
	else
		return 30;
}

bool updateDateTime()
{
	//configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
	if (!getLocalTime(&now))
	{
		Serial.println("Failed to obtain time");
		return false;
	}

	//
	now.days_in_month = getNumberOfDays(now.tm_mon + 1, now.tm_year);
	now.month_num = now.tm_mon + 1;

	// gives offset of first day of the month with respect to Monday
	// https://www.tondering.dk/claus/cal/chrweek.php#calcdow
	uint8_t a = (14 - now.month_num) / 12;
	uint16_t y = (now.tm_year + 1900) - a;
	uint16_t m = now.month_num + (12 * a) - 2;
	now.day_offset = (((1 + y + (y / 4) - (y / 100) + (y / 400) + ((31 * m) / 12)) % 7) + 7) % 7;

	return true;
}

void setupDateTime()
{
	configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

	if (!updateDateTime())
	{
		// re-try
		updateDateTime();
	}

	Serial.println(&now, "%A, %B %d %Y %H:%M:%S");
	lastUpdate = millis();
}

void loopDateTime()
{
	if ((millis() - lastUpdate) >= 1000)
	{
		lastUpdate = millis();
		updateDateTime();
	}
}