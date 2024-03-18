
/**
  ******************************************************************************
  * @file           : timer.h
  * @brief          : Header timer.c file.
  *                   This file contains the common defines of the timer
  *                   peripheral.
  ******************************************************************************
*/

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "main.h"

#define TIM2_PRESACALER		367
#define TIM2_PERIOD			65216

void tim2_init(void);

#endif /* INC_TIMER_H_ */
