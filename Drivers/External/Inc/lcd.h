
/**
  ******************************************************************************
  * @file           : lcd.h
  * @brief          : Header for lcd.c file.
  *                   This file contains the common defines of the lcd display
  *                   module.
  ******************************************************************************
*/

#ifndef EXTERNAL_INC_LCD_H_
#define EXTERNAL_INC_LCD_H_

#include "stm32f4xx_hal.h"

/*
 *  LCD Enum definitions
 */

typedef enum
{
	SET_LCD_WRITE = 0,
	SET_LCD_READ
}LCD_ReadWrite_e;

typedef enum
{
	SET_LCD_REG_IR = 0,
	SET_LCD_REG_DR
}LCD_RegSelect_e;

/*
 * 	LCD Parameters
 */

/* LCD commands definitions */
#define LCD_DISP_CLEAR 				0x01
#define LCD_DISP_RETURN_HOME		0x02
#define LCD_ENTRY_MODE_SET 			0x06
#define LCD_DISP_ON					0x0C
#define LCD_DISP_OFF				0x08
#define LCD_DISP_ON_CURSOR_ON		0x0E
#define LCD_DISP_ON_CURSOR_OFF		LCD_DISP_ON
#define LCD_DISP_ON_BLINK_ON		0x0D
#define LCD_DISP_ON_BLINK_OFF		LCD_DISP_ON
#define LCD_FUNC_SET_4DL_2L_5x8F	0x28

/* LCD GPIO definitions */
#define LCD_GPIO_PORT				GPIOB
#define LCD_RS_PIN					GPIO_PIN_3
#define LCD_EN_PIN					GPIO_PIN_4
#define LCD_D4_PIN					GPIO_PIN_13
#define LCD_D5_PIN					GPIO_PIN_14
#define LCD_D6_PIN					GPIO_PIN_15
#define LCD_D7_PIN					GPIO_PIN_1
#define LCD_BACKLIGHT_PIN			GPIO_PIN_9

/*
 *  IO operation functions
 */

void lcd_init(void);
void lcd_send_command(uint8_t command);
void lcd_send_data(uint8_t data);
void lcd_print_string(char *message);
void lcd_ctrl_backlight(void);

/*
 *  LCD command functions
 */

void lcd_display_clear(void);
void lcd_display_return_home(void);
void lcd_entry_mode_set(void);
void lcd_display_on(void);
void lcd_display_off(void);
void lcd_display_on_cursor_on(void);
void lcd_display_on_cursor_off(void);
void lcd_display_on_blink_on(void);
void lcd_display_on_blink_off(void);
void lcd_function_set(void);
void lcd_set_cursor(uint8_t row, uint8_t col);

#endif /* EXTERNAL_INC_LCD_H_ */
