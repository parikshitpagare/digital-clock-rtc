
/**
  ******************************************************************************
  * @file    it.h
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
*/

#ifndef __IT_H
#define __IT_H

void SysTick_Handler(void);
void EXTI9_5_IRQHandler(void);
void TIM2_IRQHandler(void);

#endif /* __IT_H */
