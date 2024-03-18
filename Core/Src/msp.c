
/**
  ******************************************************************************
  * @file         msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
  ******************************************************************************
 */

#include "main.h"

/*
 *  HAL Initialization
 */

void HAL_MspInit(void)
{
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();

	/* Priority grouping of ARM Cortex Mx processor*/
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
}
