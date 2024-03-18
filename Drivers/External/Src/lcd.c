/**
  ******************************************************************************
  * @file           : lcd.c
  * @brief          : lcd driver.
  ******************************************************************************
*/

/*
 *  Private function prototypes
 */

#include "lcd.h"

/*
 *  Private function prototypes
 */

static void lcd_config(void);
static void write_lcd_data_lines(uint8_t data);
static void lcd_enable(void);
static void lcd_init_send_command(void);
//static void delay_microsecond(uint32_t delayValue);

/*
 *  LCD operation functions
 */

void lcd_init(void)
{
	/* Configure the LCD pins */
	lcd_config();

	HAL_Delay(50);

	/* Enable instruction register */
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_RS_PIN, SET_LCD_REG_IR);

	/*
	 *  Initialize LCD to 4 bits
	 */

	write_lcd_data_lines(0x3);	/* Write D4 D5 D6 D7 = 1 1 0 0 */
	HAL_Delay(10);

	write_lcd_data_lines(0x3);	/* Write D4 D5 D6 D7 = 1 1 0 0 */
	HAL_Delay(1);
	//delay_microsecond(150);

	write_lcd_data_lines(0x3);	/* Write D4 D5 D6 D7 = 1 1 0 0 */
	write_lcd_data_lines(0x2);	/* Write D4 D5 D6 D7 = 0 1 0 0 */

	/*
	 *  Send commands to for LCD initialization
	 */

	lcd_init_send_command();
}

void lcd_ctrl_backlight(void)
{
	HAL_GPIO_TogglePin(LCD_GPIO_PORT, LCD_BACKLIGHT_PIN);
}

void lcd_send_command(uint8_t command)
{

	/* Enable instruction register */
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_RS_PIN, SET_LCD_REG_IR);

	/* Write data in 2 cycles */
	write_lcd_data_lines ((command >> 4) & 0x0F);	/* Write higher nibble */
	write_lcd_data_lines (command & 0x0F);			/* Write lower nibble */
}

void lcd_send_data(uint8_t data)
{
	/* Enable data register */
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_RS_PIN, SET_LCD_REG_DR);

	/* Write data in 2 cycles */
	write_lcd_data_lines((data >> 4) & 0x0F);	/* Write higher nibble */
	write_lcd_data_lines(data & 0x0F);			/* Write lower nibble */
}

void lcd_print_string(char *message)
{
	do
	{
		lcd_send_data((uint8_t)*message++);
	}
	while (*message != '\0');
}

/*
 *  LCD command functions
 */

void lcd_display_clear(void)
{
	lcd_send_command(LCD_DISP_CLEAR);
	HAL_Delay(5);
}

void lcd_display_return_home(void)
{
	lcd_send_command(LCD_DISP_RETURN_HOME);
	HAL_Delay(5);
}

void lcd_entry_mode_set(void)
{
	lcd_send_command(LCD_ENTRY_MODE_SET);
	HAL_Delay(1);
}

void lcd_display_on(void)
{
	lcd_send_command(LCD_DISP_ON);
	HAL_Delay(1);
}

void lcd_display_off(void)
{
	lcd_send_command(LCD_DISP_OFF);
	HAL_Delay(1);
}

void lcd_display_on_cursor_on(void)
{
	lcd_send_command(LCD_DISP_ON_CURSOR_ON);
	HAL_Delay(1);
}

void lcd_display_on_cursor_off(void)
{
	lcd_send_command(LCD_DISP_ON_CURSOR_OFF);
	HAL_Delay(1);
}

void lcd_display_on_blink_on(void)
{
	lcd_send_command(LCD_DISP_ON_BLINK_ON);
	HAL_Delay(1);
}

void lcd_display_on_blink_off(void)
{
	lcd_send_command(LCD_DISP_ON_BLINK_OFF);
	HAL_Delay(1);
}

void lcd_function_set(void)
{
	lcd_send_command(LCD_FUNC_SET_4DL_2L_5x8F);
	HAL_Delay(1);
}

/* Set cursor according to row and column */
void lcd_set_cursor(uint8_t row, uint8_t col)
{
  col--;
  switch (row)
  {
    case 1:
      /* Set cursor to 1st row address and add index*/
      lcd_send_command((col |= 0x80));
      break;

    case 2:
      /* Set cursor to 2nd row address and add index*/
       lcd_send_command((col |= 0xC0));
      break;

    default:
      break;
  }
}

/*
 *  Private Functions
 */

static void lcd_config(void)
{
	GPIO_InitTypeDef lcdInit = {0};

	/* Enable the clock for peripherals */
	//__HAL_RCC_GPIOB_CLK_ENABLE();

	/* Reset all pins of LCD */
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_RS_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_EN_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_D4_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_D5_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_D6_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_D7_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_BACKLIGHT_PIN, GPIO_PIN_SET);

	/* Configure pins of LCD */
	lcdInit.Pin 	= LCD_RS_PIN | LCD_EN_PIN | LCD_D4_PIN | LCD_D5_PIN | LCD_D6_PIN | LCD_D7_PIN | LCD_BACKLIGHT_PIN;
	lcdInit.Mode 	= GPIO_MODE_OUTPUT_PP;
	lcdInit.Pull 	= GPIO_NOPULL;
	lcdInit.Speed 	= GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(LCD_GPIO_PORT, &lcdInit);
}

static void lcd_init_send_command(void)
{
	lcd_function_set();
	lcd_display_off();
	lcd_display_clear();
	lcd_entry_mode_set();
	lcd_display_on();
}

static void write_lcd_data_lines(uint8_t data)
{
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_D4_PIN, ((data >> 0) & 0x1));
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_D5_PIN, ((data >> 1) & 0x1));
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_D6_PIN, ((data >> 2) & 0x1));
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_D7_PIN, ((data >> 3) & 0x1));

	lcd_enable();
}

static void lcd_enable(void)
{
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_EN_PIN, GPIO_PIN_SET);

	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_EN_PIN, GPIO_PIN_RESET);
	HAL_Delay(1);
}
