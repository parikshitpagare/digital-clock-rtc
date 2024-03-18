
/**
  ******************************************************************************
  * @file           : buzzer.c
  * @brief          : Buzzer driver.
  ******************************************************************************
*/

#include "buzzer.h"

static void buzzer_config(void);

void buzzer_init(void)
{
	buzzer_config();
	HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_PIN, GPIO_PIN_RESET);
}

static void buzzer_config(void)
{
	GPIO_InitTypeDef buzzerInit = {0};

	/* Configure pins of Push buttons */
	buzzerInit.Pin 		= BUZZER_PIN;
	buzzerInit.Mode 	= GPIO_MODE_OUTPUT_PP;
	buzzerInit.Pull 	= GPIO_NOPULL;
	buzzerInit.Speed 	= GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(BUZZER_GPIO_PORT, &buzzerInit);
}
