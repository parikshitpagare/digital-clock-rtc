
/**
  ******************************************************************************
  * @file           : rtc_ds1307.h
  * @brief          : Header for rtc_ds1307.c file.
  *                   This file contains the common defines of the DS1307 RTC
  *                   module.
  ******************************************************************************
*/

#ifndef EXTERNAL_RTC_DS1307_H_
#define EXTERNAL_RTC_DS1307_H_

#include "stm32f4xx_hal.h"
/*
 *  RTC DS1307 Enum definitions
 */

typedef enum
{
	CLK_MODE_AM = 0, CLK_MODE_PM, CLK_MODE_24
}RTC_DS1307_ClkMode_e;

typedef enum
{
	SUN = 1, MON, TUE, WED, THU, FRI, SAT
}RTC_DS1307_CalDay_e;

typedef enum
{
	JAN = 1, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC
}RTC_DS1307_CalMonth_e;

/*
 *  RTC DS1307 I2C Address
 */

#define RTC_DS1307_ADDR_I2C				(0x68 << 1)

/*
 *  RTC DS1307 Register addresses
 */

#define RTC_DS1307_ADDR_SEC				((uint8_t)0x00)
#define RTC_DS1307_ADDR_MIN				((uint8_t)0x01)
#define RTC_DS1307_ADDR_HRS				((uint8_t)0x02)
#define RTC_DS1307_ADDR_DAY				((uint8_t)0x03)
#define RTC_DS1307_ADDR_DATE			((uint8_t)0x04)
#define RTC_DS1307_ADDR_MONTH			((uint8_t)0x05)
#define RTC_DS1307_ADDR_YEAR			((uint8_t)0x06)
#define RTC_DS1307_ADDR_CONTROL			((uint8_t)0x07)

/*
 * 	RTC DS1307 Parameters
 */

#define RTC_DS1307_I2C_MEM_SIZE_8BIT 	1
#define RTC_DS1307_CLK_HALT_BIT 		7

/* RTC DS1307 GPIO Definitions */
#define RTC_DS1307_GPIO_PORT			GPIOB
#define RTC_DS1307_I2C_SCL_PIN			GPIO_PIN_6
#define RTC_DS1307_I2C_SDA_PIN 			GPIO_PIN_7

/* RTC DS1307 I2C Definitions */
#define RTC_DS1307_I2C 					I2C1
#define RTC_DS1307_I2C_SPEED			100000

/*
 *  RTC DS1307 Structure definitions
 */

typedef struct
{
	RTC_DS1307_ClkMode_e 	clkMode;
	uint8_t 				sec;
	uint8_t 				min;
	uint8_t 				hrs;
}RTC_DS1307_Time_t;

typedef struct
{
	uint8_t 				date;
	RTC_DS1307_CalDay_e 	day;
	RTC_DS1307_CalMonth_e 	month;
	uint8_t 				year;
}RTC_DS1307_Cal_t;

/*
 *  IO operation functions
 */

uint8_t rtc_ds1307_init(void);
void rtc_ds1307_set_time(RTC_DS1307_Time_t*);
void rtc_ds1307_get_time(RTC_DS1307_Time_t*);
void rtc_ds1307_set_cal(RTC_DS1307_Cal_t*);
void rtc_ds1307_get_cal(RTC_DS1307_Cal_t*);

#endif /* EXTERNAL_RTC_DS1307_H_ */
