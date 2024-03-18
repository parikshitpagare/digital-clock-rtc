
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
*/

#ifndef __MAIN_H
#define __MAIN_H

#include <stdio.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "rtc_ds1307.h"
#include "lcd.h"
#include "push_buttons.h"
#include "led.h"
#include "buzzer.h"
#include "timer.h"

/*
 *  Enums for LCD cursor rows and columns
 */

typedef enum
{
	LCD_ROW_1 = 1,
	LCD_ROW_2
} lcd_cursor_row_e;

typedef enum
{
	LCD_COL_1 = 1,
	LCD_COL_2,
	LCD_COL_3,
	LCD_COL_4,
	LCD_COL_5,
	LCD_COL_6,
	LCD_COL_7,
	LCD_COL_8,
	LCD_COL_9,
	LCD_COL_10,
	LCD_COL_11,
	LCD_COL_12,
	LCD_COL_13,
	LCD_COL_14,
	LCD_COL_15,
	LCD_COL_16
} lcd_cursor_col_e;

/*
 *  Macros for RTC
 */

#define DS1307_RTC_CLK_MODE		currentTime->clkMode
#define DS1307_RTC_HRS 			currentTime->hrs
#define DS1307_RTC_MIN 			currentTime->min
#define DS1307_RTC_SEC 			currentTime->sec

#define DS1307_RTC_DAY			currentCal->day
#define DS1307_RTC_DATE			currentCal->date
#define DS1307_RTC_MONTH		currentCal->month

#define	SET_CLK_MODE			setClk->clkMode
#define	SET_CLK_HRS				setClk->hrs
#define	SET_CLK_MIN				setClk->min

#define	SET_DAY					setCal->day
#define	SET_DATE				setCal->date
#define	SET_MONTH				setCal->month

#define	SET_ALARM_MODE			setAlarm->clkMode
#define	SET_ALARM_HRS			setAlarm->hrs
#define	SET_ALARM_MIN			setAlarm->min

/*
 *  Structs for setting clock, calendar and alarm
 */

typedef struct
{
	uint8_t clkMode;
	uint8_t hrs;
	uint8_t min;
} Set_Clk_t;

typedef struct
{
	uint8_t day;
	uint8_t date;
	uint8_t month;
} Set_Cal_t;

typedef struct
{
	uint8_t clkMode;
	uint8_t hrs;
	uint8_t min;
} Set_Alarm_t;


/*
 *  Time and Date print functions for testing
 */

void print_time(RTC_DS1307_Time_t *currentTime);
void print_cal(RTC_DS1307_Cal_t *currentCal);

/*
 *  Time and Date display functions on LCD
 */

void lcd_display_clk(RTC_DS1307_Time_t *currentTime);
void lcd_display_cal(RTC_DS1307_Cal_t *currentCal);

/*
 *  Button configuration functions
 */

void rtc_set_btn_config(Set_Clk_t *setClk, Set_Cal_t *setCal);
void clk_mode_btn_config(RTC_DS1307_Time_t *currentTime, RTC_DS1307_Cal_t *currentCal, Set_Clk_t *setClk, Set_Cal_t *setCal, Set_Alarm_t *setAlarm);
void alarm_ctrl_btn_config(RTC_DS1307_Time_t *currentTime, RTC_DS1307_Cal_t *currentCal, Set_Clk_t *setClk, Set_Cal_t *setCal, Set_Alarm_t *setAlarm);
void lcd_backlight_btn_config(RTC_DS1307_Time_t *currentTime, RTC_DS1307_Cal_t *currentCal, Set_Clk_t *setClk, Set_Cal_t *setCal, Set_Alarm_t *setAlarm);

/*
 *  Alarm related functions
 */

void alarm_ring(void);
void alarm_check(Set_Alarm_t *setAlarm, RTC_DS1307_Time_t *currentTime);

/*
 *  Error handling functions
 */

void rtc_ds1307_error_handler(void);

#endif /* __MAIN_H */
