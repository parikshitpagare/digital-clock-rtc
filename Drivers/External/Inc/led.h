/**
  ******************************************************************************
  * @file           : led.h
  * @brief          : Header for push_buttons.c file.
  *                   This file contains the common defines of the led.
  ******************************************************************************
*/

#ifndef EXTERNAL_INC_LED_H_
#define EXTERNAL_INC_LED_H_

#include "stm32f4xx_hal.h"

/* Push buttons GPIO definitions */
#define LED_GPIO_PORT				GPIOC
#define LED_PIN						GPIO_PIN_0

/* IO operation functions*/
void led_init(void);

#endif /* EXTERNAL_INC_LED_H_ */
