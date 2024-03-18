
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
*/

#include <it.h>
#include "main.h"

extern TIM_HandleTypeDef hTimRtcUpdate;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void EXTI9_5_IRQHandler(void)
{
	/* Delay for button de-bouncing */
	HAL_Delay(150);

	HAL_GPIO_EXTI_IRQHandler(BUTTON_RTC_SET_PIN);
	HAL_GPIO_EXTI_IRQHandler(BUTTON_CLK_MODE_PIN);
	HAL_GPIO_EXTI_IRQHandler(BUTTON_ALARM_CTRL_PIN);
	HAL_GPIO_EXTI_IRQHandler(BUTTON_LCD_BACKLIGHT_PIN);
}

void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&hTimRtcUpdate);
}
