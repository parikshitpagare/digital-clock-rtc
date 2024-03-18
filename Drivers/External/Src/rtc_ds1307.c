
/**
  ******************************************************************************
  * @file           : rtc_ds1307.c
  * @brief          : RTC DS1307 driver.
  ******************************************************************************
*/

#include "rtc_ds1307.h"

I2C_HandleTypeDef hRTC_ds1307;

/*
 *  Private function prototypes
 */

static void rtc_ds1307_i2c_config(void);
static void rtc_ds1307_write(uint8_t value, uint8_t reg_addr);
static uint8_t rtc_ds1307_read(uint8_t reg_addr);
static uint8_t convert_binary_to_bcd(uint8_t binValue);
static uint8_t convert_bcd_to_binary(uint8_t bcdValue);

/*
 *  RTC DS1307 operation functions
 */

/*
 *  Clock Halt Bit = 0 : Success
 *  Clock Halt Bit = 1 : Fail
 */
uint8_t rtc_ds1307_init(void)
{
	uint8_t clkHaltState;

	/* Configure the I2C parameters */
	rtc_ds1307_i2c_config();

	/* Enable the clock halt */
	rtc_ds1307_write(0x00, RTC_DS1307_ADDR_SEC);

	/* Read clock halt bit */
	clkHaltState = rtc_ds1307_read(RTC_DS1307_ADDR_SEC);

	return ((clkHaltState >> RTC_DS1307_CLK_HALT_BIT) & 0x1);
}

void rtc_ds1307_set_time(RTC_DS1307_Time_t *rtc_time)
{
	uint8_t sec;
	uint8_t min;
	uint8_t hrs;

	/*
	 *  Set the value of sec
	 */

	sec = convert_binary_to_bcd(rtc_time->sec);

	/* Keep Clock Halt bit as 0 */
	sec &= ~(1 << RTC_DS1307_CLK_HALT_BIT);

	rtc_ds1307_write(sec, RTC_DS1307_ADDR_SEC);

	/*
	 *   Set the value of min
	 */

	min = convert_binary_to_bcd(rtc_time->min);

	rtc_ds1307_write(min, RTC_DS1307_ADDR_MIN);

	/*
	 *  Set the value of hrs
	 */

	hrs = convert_binary_to_bcd(rtc_time->hrs);

	/* Setting Clock Mode to 12hr or 24hr */
	if (rtc_time->clkMode == CLK_MODE_24)
	{
		hrs &= ~(1 << 6);
	}
	else
	{
		hrs |= (1 << 6);

		if (rtc_time->clkMode == CLK_MODE_PM)
		{
			hrs |= (1 << 5);
		}
		else
		{
			hrs &= ~(1 << 5);
		}
	}

	rtc_ds1307_write(hrs, RTC_DS1307_ADDR_HRS);
}

void rtc_ds1307_get_time(RTC_DS1307_Time_t *rtc_time)
{
	uint8_t sec;
	uint8_t min;
	uint8_t hrs;

	/*
	 *  Get the value of sec
	 */

	sec = rtc_ds1307_read(RTC_DS1307_ADDR_SEC);

	/* Keep Clock Halt bit as 0 */
	sec &= ~(1 << RTC_DS1307_CLK_HALT_BIT);

	rtc_time->sec = convert_bcd_to_binary(sec);

	/*
	 *  Get the value of min
	 */

	min = rtc_ds1307_read(RTC_DS1307_ADDR_MIN);

	rtc_time->min = convert_bcd_to_binary(min);

	/*
	 *  Get the value of hrs
	 */

	hrs = rtc_ds1307_read(RTC_DS1307_ADDR_HRS);

	if (hrs & (1 << 6))
	{
		if (hrs & (1 << 5))
		{
			rtc_time->clkMode = CLK_MODE_PM;
		}
		else
		{
			rtc_time->clkMode = CLK_MODE_AM;
		}

		/* Clear bits 5 and 6 */
		hrs &= ~(0x3 << 5);
	}
	else
	{
		rtc_time->clkMode = CLK_MODE_24;
	}

	rtc_time->hrs = convert_bcd_to_binary(hrs);

}

void rtc_ds1307_set_cal(RTC_DS1307_Cal_t *rtc_cal)
{
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;

	/* Set the value of day*/
	day = convert_binary_to_bcd(rtc_cal->day);
	rtc_ds1307_write(day, RTC_DS1307_ADDR_DAY);

	/* Set the value of date*/
	date = convert_binary_to_bcd(rtc_cal->date);
	rtc_ds1307_write(date, RTC_DS1307_ADDR_DATE);

	/* Set the value of month*/
	month = convert_binary_to_bcd(rtc_cal->month);
	rtc_ds1307_write(month, RTC_DS1307_ADDR_MONTH);

	/* Set the value of year*/
	year = convert_binary_to_bcd(rtc_cal->year);
	rtc_ds1307_write(year, RTC_DS1307_ADDR_YEAR);
}

void rtc_ds1307_get_cal(RTC_DS1307_Cal_t *rtc_cal)
{
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;

	/* Set the value of day*/
	day = rtc_ds1307_read(RTC_DS1307_ADDR_DAY);
	rtc_cal->day = convert_bcd_to_binary(day);

	/* Set the value of day*/
	date = rtc_ds1307_read(RTC_DS1307_ADDR_DATE);
	rtc_cal->date = convert_bcd_to_binary(date);

	/* Set the value of day*/
	month = rtc_ds1307_read(RTC_DS1307_ADDR_MONTH);
	rtc_cal->month = convert_bcd_to_binary(month);

	/* Set the value of day*/
	year = rtc_ds1307_read(RTC_DS1307_ADDR_YEAR);
	rtc_cal->year = convert_bcd_to_binary(year);
}

/*
 *  RTC DS1307 pin configurations
 *  Called by HAL_I2C_Init() function @rtc_ds1307_i2c_config()
 */

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
	GPIO_InitTypeDef ds1307RtcInit = {0};

	/* Enable the clock for peripherals */
	__HAL_RCC_I2C1_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* Configure I2C sda and scl pins */
	ds1307RtcInit.Pin 		 = RTC_DS1307_I2C_SDA_PIN | RTC_DS1307_I2C_SCL_PIN;
	ds1307RtcInit.Mode 		 = GPIO_MODE_AF_OD;
	ds1307RtcInit.Pull 		 = GPIO_PULLUP;
	ds1307RtcInit.Speed 	 = GPIO_SPEED_FREQ_LOW;
	ds1307RtcInit.Alternate  = GPIO_AF4_I2C1;

	HAL_GPIO_Init(RTC_DS1307_GPIO_PORT, &ds1307RtcInit);
}

/*
 *  Private Functions
 */

static void rtc_ds1307_i2c_config(void)
{
	hRTC_ds1307.Instance 				= RTC_DS1307_I2C;
	hRTC_ds1307.Init.ClockSpeed 		= RTC_DS1307_I2C_SPEED;
	hRTC_ds1307.Init.DutyCycle 			= I2C_DUTYCYCLE_2;
	hRTC_ds1307.Init.AddressingMode 	= I2C_ADDRESSINGMODE_7BIT;
	hRTC_ds1307.Init.DualAddressMode 	= I2C_DUALADDRESS_DISABLE;
	hRTC_ds1307.Init.GeneralCallMode 	= I2C_GENERALCALL_DISABLE;
	hRTC_ds1307.Init.NoStretchMode 		= I2C_NOSTRETCH_DISABLE;

	HAL_I2C_Init(&hRTC_ds1307);
}

static void rtc_ds1307_write(uint8_t value, uint8_t reg_addr)
{
	HAL_I2C_Mem_Write(&hRTC_ds1307, RTC_DS1307_ADDR_I2C, reg_addr, RTC_DS1307_I2C_MEM_SIZE_8BIT, &value, 1, HAL_MAX_DELAY);
}

static uint8_t rtc_ds1307_read(uint8_t reg_addr)
{
	uint8_t regData;

	HAL_I2C_Mem_Read(&hRTC_ds1307, RTC_DS1307_ADDR_I2C, reg_addr, RTC_DS1307_I2C_MEM_SIZE_8BIT, &regData, 1, HAL_MAX_DELAY);

	return regData;
}

static uint8_t convert_binary_to_bcd(uint8_t binValue)
{
	uint8_t firstDigit;
	uint8_t lastDigit;
	uint8_t bcdValue;

	bcdValue = binValue;

	if (binValue >= 10)
	{
		firstDigit = binValue / 10;
		lastDigit = binValue % 10;

		bcdValue = ((firstDigit << 4) | lastDigit);
	}

	return bcdValue;
}

static uint8_t convert_bcd_to_binary(uint8_t bcdValue)
{
	uint8_t firstDigit;
	uint8_t lastDigit;
	uint8_t binValue;

	firstDigit = (uint8_t)((bcdValue >> 4) * 10);
	lastDigit = (bcdValue & (uint8_t)0x0F);

	binValue = firstDigit + lastDigit;

	return binValue;
}
