/**
  ******************************************************************************
  * @file           : buzzer.h
  * @brief          : Header for buzzer.c file.
  *                   This file contains the common defines of the buzzer.
  ******************************************************************************
*/

#ifndef EXTERNAL_INC_BUZZER_H_
#define EXTERNAL_INC_BUZZER_H_

#include "stm32f4xx_hal.h"

/* Push buttons GPIO definitions */
#define BUZZER_GPIO_PORT				GPIOC
#define BUZZER_PIN						GPIO_PIN_1

/* IO operation functions*/
void buzzer_init(void);

#endif /* EXTERNAL_INC_BUZZER_H_ */
