#ifndef DATETIME_H
#define DATETIME_H

#include <arduino.h>
#include <time.h>

// @see http://www.cplusplus.com/reference/ctime/tm/
// @see http://www.cplusplus.com/reference/ctime/strftime/
struct datetime_struct : tm
{
	uint8_t month_num;
	uint8_t day_offset; // 1st day of the month offset, Monday = 1, Sunnday = 0
	uint8_t days_in_month;
};

extern RTC_DATA_ATTR struct datetime_struct now;

void setupDateTime();
void loopDateTime();
bool updateDateTime();

#endif