
/**
  ******************************************************************************
  * @file           : timer.c
  * @brief          : timer.
  ******************************************************************************
*/

#include "timer.h"

static void tim2_config(void);

TIM_HandleTypeDef hTimRtcUpdate;

void tim2_init(void)
{
	tim2_config();
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htimer)
{
	/* Enable the clock for TIM2 peripheral */
	__HAL_RCC_TIM2_CLK_ENABLE();

	HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

static void tim2_config(void)
{
	hTimRtcUpdate.Instance = TIM2;
	hTimRtcUpdate.Init.Prescaler = TIM2_PRESACALER;
	hTimRtcUpdate.Init.Period = TIM2_PERIOD;

	HAL_TIM_Base_Init(&hTimRtcUpdate);
}
