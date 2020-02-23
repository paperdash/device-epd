#ifndef DATETIME_H
#define DATETIME_H

#include <arduino.h>

// TODO
struct time_struct
{
	char wday[4];
	char month[4];
	uint8_t month_num;
	uint8_t mday;
	uint8_t mil_hour;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t day_offset; // 1st day of the month offset, Monday = 0
	int year;
};

extern RTC_DATA_ATTR struct time_struct now;

void setupDateTime();

#endif