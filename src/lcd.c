/*
 * lcd.c - HD44780 LCD driver for ATmega32 (4-bit mode)
 */
#include "lcd.h"

static void pulse_en(void) {
    LCD_PORT |=  (1 << LCD_EN);
    _delay_us(1);
    LCD_PORT &= ~(1 << LCD_EN);
    _delay_us(50);
}

static void send_nibble(uint8_t n) {
    LCD_PORT &= ~((1<<LCD_D4)|(1<<LCD_D5)|(1<<LCD_D6)|(1<<LCD_D7));
    if (n & 0x01) LCD_PORT |= (1 << LCD_D4);
    if (n & 0x02) LCD_PORT |= (1 << LCD_D5);
    if (n & 0x04) LCD_PORT |= (1 << LCD_D6);
    if (n & 0x08) LCD_PORT |= (1 << LCD_D7);
    pulse_en();
}

static void send_byte(uint8_t b, uint8_t is_data) {
    if (is_data) LCD_PORT |=  (1 << LCD_RS);
    else         LCD_PORT &= ~(1 << LCD_RS);
    send_nibble(b >> 4);
    send_nibble(b & 0x0F);
    _delay_us(50);
}

void LCD_Init(void) {
    LCD_DIR |= (1<<LCD_RS)|(1<<LCD_EN)|(1<<LCD_D4)|(1<<LCD_D5)|(1<<LCD_D6)|(1<<LCD_D7);
    LCD_PORT &= ~((1<<LCD_RS)|(1<<LCD_EN)|(1<<LCD_D4)|(1<<LCD_D5)|(1<<LCD_D6)|(1<<LCD_D7));
    _delay_ms(50);
    /* HD44780 4-bit init sequence */
    send_nibble(0x03); _delay_ms(5);
    send_nibble(0x03); _delay_us(150);
    send_nibble(0x03); _delay_us(150);
    send_nibble(0x02); _delay_us(150);
    send_byte(0x28, 0); /* 4-bit, 2 lines, 5x8 font */
    send_byte(0x0C, 0); /* display on, cursor off    */
    send_byte(0x06, 0); /* entry: increment, no shift */
    send_byte(0x01, 0); /* clear                      */
    _delay_ms(2);
}

void LCD_Clear(void) {
    send_byte(0x01, 0);
    _delay_ms(2);
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t addr = (row == 0) ? 0x00 : 0x40;
    send_byte(0x80 | (addr + col), 0);
}

void LCD_WriteChar(char c) {
    send_byte((uint8_t)c, 1);
}

void LCD_WriteString(const char *str) {
    while (*str) LCD_WriteChar(*str++);
}

void LCD_WriteInt(int16_t val) {
    char buf[7];
    int8_t i = 0;
    if (val < 0) { LCD_WriteChar('-'); val = -val; }
    if (val == 0) { LCD_WriteChar('0'); return; }
    while (val > 0) { buf[i++] = '0' + (int8_t)(val % 10); val /= 10; }
    while (i > 0) LCD_WriteChar(buf[--i]);
}
