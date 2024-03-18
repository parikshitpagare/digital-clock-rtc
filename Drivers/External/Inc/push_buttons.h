/**
  ******************************************************************************
  * @file           : push_buttons.h
  * @brief          : Header for push_buttons.c file.
  *                   This file contains the common defines of the push buttons.
  ******************************************************************************
*/

#ifndef EXTERNAL_INC_PUSH_BUTTONS_H_
#define EXTERNAL_INC_PUSH_BUTTONS_H_

#include "stm32f4xx_hal.h"
#include <stdio.h>

/* Push buttons GPIO definitions */
#define BUTTON_GPIO_PORT				GPIOC
#define BUTTON_RTC_SET_PIN				GPIO_PIN_5
#define BUTTON_CLK_MODE_PIN				GPIO_PIN_6
#define BUTTON_ALARM_CTRL_PIN			GPIO_PIN_8
#define BUTTON_LCD_BACKLIGHT_PIN		GPIO_PIN_9

/* IO operation functions*/
void push_button_init(void);

#endif /* EXTERNAL_INC_PUSH_BUTTONS_H_ */
