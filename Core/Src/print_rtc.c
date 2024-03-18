
/**
  ******************************************************************************
  * @file           : print_rtc.c
  * @brief          : Source file.
  *                   This file contains the print statements for current
  *                   date and time from the DS1307 RTC module.
  ******************************************************************************
*/

#include "main.h"

void print_time(RTC_DS1307_Time_t *currentTime)
{
	printf("Current time: %d:", currentTime->hrs);
	printf("%d:", currentTime->min);
	printf("%d\n", currentTime->sec);
}

void print_cal(RTC_DS1307_Cal_t *currentCal)
{
	printf("Current date: %d/", currentCal->date);
	printf("%d/", currentCal->month);
	printf("%d\n", currentCal->year);
}

char* get_day(RTC_DS1307_CalDay_e day)
{
	char* days[7] = {"Sun", "Mon", "Tue", "Wed", "Thur", "Fri", "Sat"};

	return days[day - 1];
}
