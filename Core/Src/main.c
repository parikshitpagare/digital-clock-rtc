
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */

#include "main.h"

extern TIM_HandleTypeDef hTimRtcUpdate;

RTC_DS1307_Time_t currentTime;
RTC_DS1307_Cal_t currentCal;

Set_Clk_t setClk;
Set_Cal_t setCal;
Set_Alarm_t setAlarm;

uint8_t rtcSetBtn;

bool alarmStatus;
bool alarmRing;

/*
 * 	Function prototypes
 */

void sys_clk_config(void);

/*
 * 	Callback for ISR
 */

/* ISR callback for timer 2 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	lcd_display_clk(&currentTime);
	lcd_display_cal(&currentCal);

	alarm_ring();
}

/* ISR callback for push buttons */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == BUTTON_RTC_SET_PIN)
		rtc_set_btn_config(&setClk, &setCal);

	if (GPIO_Pin == BUTTON_CLK_MODE_PIN)
		clk_mode_btn_config(&currentTime, &currentCal, &setClk, &setCal, &setAlarm);

	if (GPIO_Pin == BUTTON_ALARM_CTRL_PIN)
		alarm_ctrl_btn_config(&currentTime, &currentCal, &setClk, &setCal, &setAlarm);

	if (GPIO_Pin == BUTTON_LCD_BACKLIGHT_PIN)
		lcd_backlight_btn_config(&currentTime, &currentCal, &setClk, &setCal, &setAlarm);
}

int main(void)
{
	/* Reset of all peripherals, Initializes the Flash interface and the Systick */
	HAL_Init();
	sys_clk_config();

	/* Initialize external components */
	if (rtc_ds1307_init())
	{
		rtc_ds1307_error_handler();
		while (1);
	}

	lcd_init();
	push_button_init();
	led_init();
	buzzer_init();

	/* Initialize timer 2 */
	tim2_init();

	/* Initialize global variables */
	rtcSetBtn = 0;
	alarmStatus = 0;
	alarmRing = 0;

	/* Initialize RTC clock and calendar */
	currentTime.clkMode = CLK_MODE_24;
	currentTime.hrs = 0;
	currentTime.min = 0;
	currentTime.sec = 0;
	currentCal.day = SUN;
	currentCal.date = 1;
	currentCal.month = JAN;
	currentCal.year = 24;

	/* Initialize the values for setting clock */
	setClk.clkMode = CLK_MODE_24;
	setClk.hrs = 0;
	setClk.min = 0;

	/* Initialize the values for setting calendar */
	setCal.day = SUN;
	setCal.date = 1;
	setCal.month = JAN;

	/* Initialize the values for setting alarm */
	setAlarm.clkMode = CLK_MODE_24;
	setAlarm.hrs = 0;
	setAlarm.min = 0;

	rtc_ds1307_set_time(&currentTime);
	rtc_ds1307_set_cal(&currentCal);

	/* Start timer 2 interrupt */
	HAL_TIM_Base_Start_IT(&hTimRtcUpdate);

	while(1) {}

	return 0;
}

void sys_clk_config(void)
{
	  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	  /* Configure the main internal regulator output voltage */
	  __HAL_RCC_PWR_CLK_ENABLE();
	  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

	  /* Initializes the RCC Oscillators according to the specified parameters
	   *   in the RCC_OscInitTypeDef structure.
	   */
	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	  RCC_OscInitStruct.PLL.PLLM = 16;
	  RCC_OscInitStruct.PLL.PLLN = 336;
	  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
	  RCC_OscInitStruct.PLL.PLLQ = 7;

	  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	  {
		  rtc_ds1307_error_handler();
	  }

	  /* Initializes the CPU, AHB and APB buses clocks */
	  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	  {
		  rtc_ds1307_error_handler();
	  }
}

/**
  **************************************************************************
  * @brief : Error handling functions
  **************************************************************************
  */

void rtc_ds1307_error_handler(void)
{
	printf ("RTC init failed\n");
}

/**
  **************************************************************************
  * @brief : Alarm related functions
  **************************************************************************
  */

void alarm_ring(void)
{
	uint8_t static alarmRingCounter = 0;

	alarm_check(&setAlarm, &currentTime);

	/* Reset the LED and Buzzer pins when alarm time is not up */
	if ((alarmRing == 0) || (alarmStatus == 0))
	{
		alarmRingCounter = 0;

		HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_PIN, GPIO_PIN_RESET);
	}

	/* Toggle the LED and Buzzer pins when alarm time is up */
	else if ((alarmStatus == 1) && (alarmRing == 1))
	{
		alarmRingCounter++;

		/* Stop alarm ringing automatically after approximate 30 sec */
		if (alarmRingCounter > 110)
		{
			alarmRing = 0;
			alarmRingCounter = 0;
		}

		HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
		HAL_GPIO_TogglePin(BUZZER_GPIO_PORT, BUZZER_PIN);
	}
}

void alarm_check(Set_Alarm_t *setAlarm, RTC_DS1307_Time_t *currentTime)
{
	rtc_ds1307_get_time(currentTime);

	if ((SET_ALARM_MODE == DS1307_RTC_CLK_MODE) && (SET_ALARM_HRS == DS1307_RTC_HRS) && (SET_ALARM_MIN == DS1307_RTC_MIN) && (DS1307_RTC_SEC == 0))
	{
		alarmRing = 1;
	}
}

/**
  **************************************************************************
  * @brief : LCD related functions
  **************************************************************************
  */

/*
 *  Display and set clock
 */

void lcd_display_clk(RTC_DS1307_Time_t *currentTime)
{
	static char getTime[9];
	char* clkModeStatus[] = { "am","pm", "  "};

	rtc_ds1307_get_time(currentTime);

	/* Display 12h or 24h clock mode */
	if ((DS1307_RTC_CLK_MODE == CLK_MODE_AM) || (DS1307_RTC_CLK_MODE == CLK_MODE_PM))
	{
		lcd_set_cursor(LCD_ROW_1, LCD_COL_1);
		lcd_print_string("    ");
	}
	else if (DS1307_RTC_CLK_MODE == CLK_MODE_24)
	{
		lcd_set_cursor(LCD_ROW_1, LCD_COL_1);
		lcd_print_string("24h>");
	}

	/* Display alarm is enabled or disabled */
	if(alarmStatus)
	{
		lcd_set_cursor(LCD_ROW_2, LCD_COL_1);
		lcd_print_string("AL>");
	}
	else if (!alarmStatus)
	{
		lcd_set_cursor(LCD_ROW_2, LCD_COL_1);
		lcd_print_string("    ");
	}

	/* Display am/pm if 12h is selected else display nothing if 24h is selected*/
	lcd_set_cursor(LCD_ROW_1, LCD_COL_6);
	lcd_print_string(clkModeStatus[DS1307_RTC_CLK_MODE]);

	/* Display the clock */
	getTime[0] = (DS1307_RTC_HRS / 10) + 48;
	getTime[1] = (DS1307_RTC_HRS % 10) + 48;
	getTime[2] = ':';
	getTime[3] = (DS1307_RTC_MIN / 10) + 48;
	getTime[4] = (DS1307_RTC_MIN % 10) + 48;
	getTime[5] = ':';
	getTime[6] = (DS1307_RTC_SEC / 10) + 48;
	getTime[7] = (DS1307_RTC_SEC % 10) + 48;
	getTime[8] = '\0';

	lcd_set_cursor(LCD_ROW_1, LCD_COL_9);
	lcd_print_string(getTime);
}

void lcd_display_set_clk(Set_Clk_t *setClk)
{
	char time[6];
	char* clkModeStatus[] = { "am","pm", ""};

	lcd_set_cursor(LCD_ROW_1, LCD_COL_1);
	lcd_print_string("Set");

	lcd_set_cursor(LCD_ROW_2, LCD_COL_1);
	lcd_print_string("Clk");

	/* Display am/pm if 12h is selected else display nothing if 24h is selected */
	lcd_set_cursor(LCD_ROW_2, LCD_COL_9);
	lcd_print_string(clkModeStatus[SET_CLK_MODE]);

	/* Display '24h' if 24h is selected else display mode 'md' if 12h is selected */
	if (SET_CLK_MODE == CLK_MODE_24)
	{
		lcd_set_cursor(LCD_ROW_1, LCD_COL_9);
		lcd_print_string("  ");

		lcd_set_cursor(LCD_ROW_2, LCD_COL_7);
		lcd_print_string("24h>");
	}
	if ((SET_CLK_MODE == CLK_MODE_AM) || (SET_CLK_MODE == CLK_MODE_PM))
	{
		lcd_set_cursor(LCD_ROW_1, LCD_COL_9);
		lcd_print_string("md");

		lcd_set_cursor(LCD_ROW_2, LCD_COL_7);
		lcd_print_string("  ");
	}

	lcd_set_cursor(LCD_ROW_1, LCD_COL_12);
	lcd_print_string("hh");

	lcd_set_cursor(LCD_ROW_1, LCD_COL_15);
	lcd_print_string("mm");

	/* Display the set clock interface */
	time[0] = (SET_CLK_HRS / 10) + 48;
	time[1] = (SET_CLK_HRS % 10) + 48;
	time[2] = ':';
	time[3] = (SET_CLK_MIN / 10) + 48;
	time[4] = (SET_CLK_MIN % 10) + 48;
	time[5] = '\0';

	lcd_set_cursor(LCD_ROW_2, LCD_COL_12);
	lcd_print_string(time);
}

/*
 *  Display and set calendar
 */

void lcd_display_cal(RTC_DS1307_Cal_t *currentCal)
{
	static char getDate[6];
	char* days[] = { "sun","mon","tue","wed","thu","fri","sat"};

	rtc_ds1307_get_cal(currentCal);

	lcd_set_cursor(LCD_ROW_2, LCD_COL_4);

	/* Display day of the week */
	lcd_set_cursor(LCD_ROW_2, LCD_COL_8);
	lcd_print_string(days[DS1307_RTC_DAY - 1]);

	/* Display date and month */
	getDate[0] = (DS1307_RTC_DATE / 10) + 48;
	getDate[1] = (DS1307_RTC_DATE % 10) + 48;
	getDate[2] = '/';
	getDate[3] = (DS1307_RTC_MONTH / 10) + 48;
	getDate[4] = (DS1307_RTC_MONTH % 10) + 48;
	getDate[5] = '\0';

	lcd_set_cursor(LCD_ROW_2, LCD_COL_12);
	lcd_print_string(getDate);
}

void lcd_display_set_cal(Set_Cal_t *setCal)
{
	char cal[6];
	char* days[] = { "sun","mon","tue","wed","thu","fri","sat"};

	lcd_set_cursor(LCD_ROW_1, LCD_COL_1);
	lcd_print_string("Set");

	lcd_set_cursor(LCD_ROW_2, LCD_COL_1);
	lcd_print_string("Cal");

	lcd_set_cursor(LCD_ROW_1, LCD_COL_8);
	lcd_print_string("day");

	/* Display set day of the week interface */
	lcd_set_cursor(LCD_ROW_2, LCD_COL_8);
	lcd_print_string(days[SET_DAY - 1]);

	lcd_set_cursor(LCD_ROW_1, LCD_COL_12);
	lcd_print_string("dd");

	lcd_set_cursor(LCD_ROW_1, LCD_COL_15);
	lcd_print_string("mm");

	/* Display the set date and month interface */
	cal[0] = (SET_DATE / 10) + 48;
	cal[1] = (SET_DATE % 10) + 48;
	cal[2] = '/';
	cal[3] = (SET_MONTH / 10) + 48;
	cal[4] = (SET_MONTH % 10) + 48;
	cal[5] = '\0';

	lcd_set_cursor(LCD_ROW_2, LCD_COL_12);
	lcd_print_string(cal);
}

/*
 *  Set alarm
 */

void lcd_display_set_alarm(Set_Alarm_t *setAlarm)
{
	char alarm[6];
	char* clkModeStatus[] = { "am","pm", "  "};

	lcd_set_cursor(LCD_ROW_1, LCD_COL_1);
	lcd_print_string("Set");

	lcd_set_cursor(LCD_ROW_2, LCD_COL_1);
	lcd_print_string("Alm");

	/* Display am/pm if 12h is selected else display nothing if 24h is selected */
	lcd_set_cursor(LCD_ROW_2, LCD_COL_9);
	lcd_print_string(clkModeStatus[SET_ALARM_MODE]);

	/* Display '24h' if 24h is selected else display mode 'md' if 12h is selected */
	if (SET_ALARM_MODE == CLK_MODE_24)
	{
		lcd_set_cursor(LCD_ROW_1, LCD_COL_9);
		lcd_print_string("  ");

		lcd_set_cursor(LCD_ROW_2, LCD_COL_7);
		lcd_print_string("24h>");
	}
	if ((SET_ALARM_MODE == CLK_MODE_AM) || (SET_ALARM_MODE == CLK_MODE_PM))
	{
		lcd_set_cursor(LCD_ROW_1, LCD_COL_9);
		lcd_print_string("md");

		lcd_set_cursor(LCD_ROW_2, LCD_COL_7);
		lcd_print_string("  ");

		if (SET_ALARM_HRS == 0)
		{
			SET_ALARM_HRS = 12;
		}
		if ((SET_ALARM_HRS > 12) && (SET_ALARM_HRS <= 23))
		{
			SET_ALARM_HRS = (SET_ALARM_HRS - 12);
		}
	}

	lcd_set_cursor(LCD_ROW_1, LCD_COL_12);
	lcd_print_string("hh");

	lcd_set_cursor(LCD_ROW_1, LCD_COL_15);
	lcd_print_string("mm");

	/* Display the set alarm interface */
	alarm[0] = (SET_ALARM_HRS / 10) + 48;
	alarm[1] = (SET_ALARM_HRS % 10) + 48;
	alarm[2] = ':';
	alarm[3] = (SET_ALARM_MIN / 10) + 48;
	alarm[4] = (SET_ALARM_MIN % 10) + 48;
	alarm[5] = '\0';

	lcd_set_cursor(LCD_ROW_2, LCD_COL_12);
	lcd_print_string(alarm);
}

/**
  **************************************************************************
  * @brief : Push button configuration functions
  **************************************************************************
  */

/* Configure Button 01 */
void rtc_set_btn_config(Set_Clk_t *setClk, Set_Cal_t *setCal)
{
	lcd_display_clear();

	rtcSetBtn++;

	if (rtcSetBtn > 3)
		rtcSetBtn = 0;

	if (rtcSetBtn == 0)
	{
		SET_CLK_HRS = 0;
		SET_CLK_MIN = 0;

		SET_DAY = SUN;
		SET_DATE = 1;
		SET_MONTH = JAN;

		HAL_NVIC_EnableIRQ(TIM2_IRQn);
	}

	else if (rtcSetBtn == 1)
	{
		HAL_NVIC_DisableIRQ(TIM2_IRQn);

		HAL_Delay(200);
		lcd_display_clear();

		lcd_display_set_clk(setClk);
	}

	else if (rtcSetBtn == 2)
		lcd_display_set_cal(setCal);

	else if (rtcSetBtn == 3)
		lcd_display_set_alarm(&setAlarm);
}

/* Configure Button 02 */
void clk_mode_btn_config(RTC_DS1307_Time_t *currentTime, RTC_DS1307_Cal_t *currentCal, Set_Clk_t *setClk, Set_Cal_t *setCal, Set_Alarm_t *setAlarm)
{
	/*
	 *  Button 02 -> Function 01: Change clock mode between 12h and 24h on main display
	 */

	if (rtcSetBtn == 0)
	{
		rtc_ds1307_get_time(currentTime);

		/* Convert to 12hr clock mode */
		if (DS1307_RTC_CLK_MODE == CLK_MODE_24)
		{
			if ((DS1307_RTC_HRS > 1) && (DS1307_RTC_HRS < 12))
				DS1307_RTC_CLK_MODE = CLK_MODE_AM;

			else if (DS1307_RTC_HRS > 12)
			{
				DS1307_RTC_CLK_MODE = CLK_MODE_PM;
				DS1307_RTC_HRS = (DS1307_RTC_HRS - 12);

			}
			else if (DS1307_RTC_HRS == 0)
			{
				DS1307_RTC_CLK_MODE = CLK_MODE_AM;
				DS1307_RTC_HRS = 12;
			}
		}

		/* Convert to 24hr clock mode */
		else if (DS1307_RTC_CLK_MODE == CLK_MODE_AM)
		{
			DS1307_RTC_CLK_MODE = CLK_MODE_24;

			if (DS1307_RTC_HRS == 12)
				DS1307_RTC_HRS = 0;
		}

		/* Convert to 24hr clock mode */
		else if ((DS1307_RTC_CLK_MODE == CLK_MODE_PM))
		{
			DS1307_RTC_CLK_MODE = CLK_MODE_24;

			if (DS1307_RTC_HRS == 12)
				DS1307_RTC_HRS = 12;
			else
				DS1307_RTC_HRS = (DS1307_RTC_HRS + 12);
		}

		rtc_ds1307_set_time(currentTime);

		SET_CLK_MODE = DS1307_RTC_CLK_MODE;
		SET_ALARM_MODE = DS1307_RTC_CLK_MODE;
	}

	/*
	 *  Button 02 -> Function 02: Set the Clock Mode status while setting clock
	 */

	else if (rtcSetBtn == 1)
	{
		rtc_ds1307_get_time(currentTime);

		uint8_t clkMode = DS1307_RTC_CLK_MODE;

		if (clkMode == CLK_MODE_AM)
		{
			SET_CLK_MODE = CLK_MODE_PM;
			DS1307_RTC_CLK_MODE = CLK_MODE_PM;
		}

		else if (clkMode == CLK_MODE_PM)
		{
			SET_CLK_MODE = CLK_MODE_AM;
			DS1307_RTC_CLK_MODE = CLK_MODE_AM;
		}

		else if (DS1307_RTC_CLK_MODE == CLK_MODE_24)
			SET_CLK_MODE = CLK_MODE_24;

		rtc_ds1307_set_time(currentTime);

		lcd_display_set_clk(setClk);
	}

	/*
	 *  Button 02 -> Function 03: Set the Day while setting calendar
	 */

	else if (rtcSetBtn == 2)
	{
		uint8_t btn02Counter = SET_DAY;
		uint8_t const btn01CounterMax = 7;

		btn02Counter++;

		if (btn02Counter > btn01CounterMax)
			btn02Counter = 1;

		DS1307_RTC_DAY = btn02Counter;
		SET_DAY = btn02Counter;

		rtc_ds1307_set_cal(currentCal);

		lcd_display_set_cal(setCal);
	}

	/*
	 *  Button 02 -> Function 04: Set the Alarm Mode status while setting alarm
	 */

	else if (rtcSetBtn == 3)
	{
		rtc_ds1307_get_time(currentTime);

		uint8_t alarmMode = SET_ALARM_MODE;

		if (alarmMode == CLK_MODE_AM)
			SET_ALARM_MODE = CLK_MODE_PM;

		else if (alarmMode == CLK_MODE_PM)
			SET_ALARM_MODE = CLK_MODE_AM;

		lcd_display_set_alarm(setAlarm);
	}
}

/* Configure Button 03 */
void alarm_ctrl_btn_config(RTC_DS1307_Time_t *currentTime, RTC_DS1307_Cal_t *currentCal, Set_Clk_t *setClk, Set_Cal_t *setCal, Set_Alarm_t *setAlarm)
{
	/*
	 *  Button 03 -> Function 01: Turn on/off and stop alarm
	 */

	if (rtcSetBtn == 0)
	{
		if (alarmRing)
		{
			HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_RESET);
			alarmRing = 0;
		}

		else if (!alarmStatus && !alarmRing)
			alarmStatus = 1;

		else if (alarmStatus && !alarmRing)
			alarmStatus = 0;
	}

	/*
	 *  Button 03 -> Function 02: Set the Hrs value while setting clock
	 */

	else if (rtcSetBtn == 1)
	{
		uint8_t btn03Counter = SET_CLK_HRS;
		uint8_t const btn03CounterMax[2] = {23, 12};

		if (SET_CLK_MODE == CLK_MODE_24)
		{
			btn03Counter++;

			if (btn03Counter > btn03CounterMax[0])
				btn03Counter = 0;

			DS1307_RTC_HRS = btn03Counter;
			SET_CLK_HRS = btn03Counter;

			rtc_ds1307_set_time(currentTime);
			lcd_display_set_clk(setClk);
		}

		else if ((SET_CLK_MODE == CLK_MODE_AM) || (SET_CLK_MODE == CLK_MODE_PM))
		{
			btn03Counter++;

			if (btn03Counter > btn03CounterMax[1])
				btn03Counter = 1;

			DS1307_RTC_HRS = btn03Counter;
			SET_CLK_HRS = (btn03Counter);

			rtc_ds1307_set_time(currentTime);
			lcd_display_set_clk(setClk);
		}
	}

	/*
	 *  Button 03 -> Function 03: Set the Date value while setting calendar
	 */

	else if (rtcSetBtn == 2)
	{
		uint8_t btn03Counter = SET_DATE;
		uint8_t const btn02CounterMax = 31;

		rtc_ds1307_get_cal(currentCal);

		btn03Counter++;

		if (btn03Counter > btn02CounterMax)
			btn03Counter = 1;

		DS1307_RTC_DATE = btn03Counter;
		SET_DATE = btn03Counter;

		rtc_ds1307_set_cal(currentCal);
		lcd_display_set_cal(setCal);
	}

	/*
	 *  Button 03 -> Function 04: Set the Hrs value while setting alarm
	 */

	else if (rtcSetBtn == 3)
	{
		uint8_t btn03Counter[2] = {SET_ALARM_HRS, SET_ALARM_HRS};
		uint8_t const btn03CounterMax[2] = {23, 12};

		if (SET_ALARM_MODE == CLK_MODE_24)
		{
			btn03Counter[0]++;

			if (btn03Counter[0] > btn03CounterMax[0])
				btn03Counter[0] = 0;

			SET_ALARM_HRS = btn03Counter[0];

			lcd_display_set_alarm(setAlarm);
		}

		else if ((SET_ALARM_MODE == CLK_MODE_AM) || (SET_ALARM_MODE == CLK_MODE_PM))
		{
			btn03Counter[1]++;

			if (btn03Counter[1] > btn03CounterMax[1])
				btn03Counter[1] = 1;

			SET_ALARM_HRS = (btn03Counter[1]);

			lcd_display_set_alarm(setAlarm);
		}
	}
}

/* Configure Button 04 */
void lcd_backlight_btn_config(RTC_DS1307_Time_t *currentTime, RTC_DS1307_Cal_t *currentCal, Set_Clk_t *setClk, Set_Cal_t *setCal, Set_Alarm_t *setAlarm)
{
	/*
	 *  Button 04 -> Control LCD backlight
	 */

	if (rtcSetBtn == 0)
	{
		lcd_ctrl_backlight();
	}

	/*
	 *  Button 04 -> Function 02: Set Min value while setting clock
	 */

	else if (rtcSetBtn == 1)
	{
		uint8_t btn04Counter = SET_CLK_MIN;
		uint8_t const btn04CounterMax = 59;

		btn04Counter++;

		if (btn04Counter > btn04CounterMax)
			btn04Counter = 0;

		DS1307_RTC_MIN = btn04Counter;
		SET_CLK_MIN = btn04Counter;

		rtc_ds1307_set_time(currentTime);
		lcd_display_set_clk(setClk);
	}

	/*
	 *  Button 04 -> Function 03: Set Month value while setting Calendar
	 */

	else if (rtcSetBtn == 2)
	{
		uint8_t btn04Counter = SET_MONTH;
		uint8_t const btn04CounterMax = 12;

		btn04Counter++;

		if (btn04Counter > btn04CounterMax)
			btn04Counter = 1;

		DS1307_RTC_MONTH = btn04Counter;
		SET_MONTH = btn04Counter;

		rtc_ds1307_set_cal(currentCal);
		lcd_display_set_cal(setCal);
	}

	/*
	 *  Button 04 -> Function 04: Set Min value while setting alarm
	 */

	else if (rtcSetBtn == 3)
	{
		uint8_t btn04Counter = SET_ALARM_MIN;
		uint8_t const btn04CounterMax = 59;

		btn04Counter++;

		if (btn04Counter > btn04CounterMax)
			btn04Counter = 0;

		SET_ALARM_MIN = btn04Counter;

		lcd_display_set_alarm(setAlarm);
	}
}
