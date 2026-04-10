/*
 * lcd.h - HD44780 LCD driver for ATmega32 (4-bit mode)
 *
 * LCD wired to PORTC:
 *   PC0 = RS   PC1 = EN   RW -> GND (write-only)
 *   PC4 = D4   PC5 = D5   PC6 = D6   PC7 = D7
 */
#ifndef LCD_H
#define LCD_H

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define LCD_DIR     DDRC
#define LCD_PORT    PORTC
#define LCD_RS      PC0
#define LCD_EN      PC1
#define LCD_D4      PC4
#define LCD_D5      PC5
#define LCD_D6      PC6
#define LCD_D7      PC7

void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_WriteChar(char c);
void LCD_WriteString(const char *str);
void LCD_WriteInt(int16_t val);

#endif /* LCD_H */
