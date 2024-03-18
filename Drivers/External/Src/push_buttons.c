/**
  ******************************************************************************
  * @file           : push_buttons.c
  * @brief          : Push buttons driver driver.
  ******************************************************************************
*/

#include "push_buttons.h"

static void push_button_config(void);

void push_button_init(void)
{
	push_button_config();
}

static void push_button_config(void)
{
	GPIO_InitTypeDef pushBtnInit = {0};

	/* Enable the clock for peripherals */
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/* Configure pins of Push buttons */
	pushBtnInit.Pin 	= BUTTON_CLK_MODE_PIN | BUTTON_LCD_BACKLIGHT_PIN | BUTTON_ALARM_CTRL_PIN | BUTTON_RTC_SET_PIN;
	pushBtnInit.Mode 	= GPIO_MODE_IT_FALLING;
	pushBtnInit.Pull 	= GPIO_PULLUP;
	pushBtnInit.Speed 	= GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(BUTTON_GPIO_PORT, &pushBtnInit);

	HAL_NVIC_SetPriority(EXTI9_5_IRQn,1,0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}
