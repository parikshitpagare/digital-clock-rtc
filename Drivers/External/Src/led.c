
/**
  ******************************************************************************
  * @file           : led.c
  * @brief          : Led driver.
  ******************************************************************************
*/

#include "led.h"

static void led_config(void);

void led_init(void)
{
	led_config();
	HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_RESET);
}

static void led_config(void)
{
	GPIO_InitTypeDef ledInit = {0};

	/* Configure pins of Push buttons */
	ledInit.Pin 	= LED_PIN;
	ledInit.Mode 	= GPIO_MODE_OUTPUT_PP;
	ledInit.Pull 	= GPIO_NOPULL;
	ledInit.Speed 	= GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(LED_GPIO_PORT, &ledInit);
}
