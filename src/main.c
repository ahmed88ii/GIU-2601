/*
 * main.c - ATmega32 Embedded Systems Demo
 *
 * Features:
 *  1. Button-controlled LEDs with software debounce
 *  2. LM35 temperature sensor on ADC0, displayed on 16x2 LCD
 *  3. Timer1 CTC ISR: blinks BLINK_LED every 500 ms,
 *     refreshes LCD temperature every 1000 ms
 *
 * Pin Summary:
 *  LEDs    : PB0 (LED0), PB1 (LED1), PB2 (LED2), PB3 (BLINK_LED)
 *  Buttons : PD2 (BTN0), PD3 (BTN1), PD4 (BTN2) -- active-low, pull-up
 *  LCD     : RS=PC0, EN=PC1, D4=PC4, D5=PC5, D6=PC6, D7=PC7
 *  LM35    : PA0 (ADC0)
 *
 * LCD Layout:
 *  Row 0: "Temp: XX.X*C"
 *  Row 1: "L0:x L1:x L2:x"
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#include "led.h"
#include "button.h"
#include "adc.h"
#include "timer.h"
#include "lcd.h"

static void lcd_show_temperature(int16_t tenths) {
    int16_t deg  = tenths / 10;
    uint8_t frac = (uint8_t)(tenths % 10);
    LCD_SetCursor(0, 6);
    if (deg >= 0 && deg < 10) LCD_WriteChar(' ');
    LCD_WriteInt(deg);
    LCD_WriteChar('.');
    LCD_WriteInt(frac);
    LCD_WriteChar((char)0xDF); /* HD44780 degree symbol */
    LCD_WriteChar('C');
}

static void lcd_show_led_states(uint8_t s0, uint8_t s1, uint8_t s2) {
    LCD_SetCursor(1, 0);
    LCD_WriteString("L0:");
    LCD_WriteChar(s0 ? '1' : '0');
    LCD_WriteString(" L1:");
    LCD_WriteChar(s1 ? '1' : '0');
    LCD_WriteString(" L2:");
    LCD_WriteChar(s2 ? '1' : '0');
    LCD_WriteChar(' ');
}

int main(void) {
    LED_Init();
    Button_Init();
    ADC_Init();
    LCD_Init();
    Timer1_Init();

    /* Splash screen */
    LCD_Clear();
    LCD_SetCursor(0, 0); LCD_WriteString("ATmega32 Project");
    LCD_SetCursor(1, 0); LCD_WriteString(" Initializing...");
    _delay_ms(2000);

    /* Static labels */
    LCD_Clear();
    LCD_SetCursor(0, 0); LCD_WriteString("Temp: ");
    lcd_show_led_states(0, 0, 0);

    Timer1_Start();
    sei();

    uint8_t led0 = 0, led1 = 0, led2 = 0;

    while (1) {
        /* Button-controlled LEDs */
        if (Button_WasPressed(BTN0)) {
            led0 ^= 1; LED_Set(LED0, led0);
            lcd_show_led_states(led0, led1, led2);
        }
        if (Button_WasPressed(BTN1)) {
            led1 ^= 1; LED_Set(LED1, led1);
            lcd_show_led_states(led0, led1, led2);
        }
        if (Button_WasPressed(BTN2)) {
            led2 ^= 1; LED_Set(LED2, led2);
            lcd_show_led_states(led0, led1, led2);
        }

        /* Timer blink every 500 ms */
        if (g_blink_tick) {
            g_blink_tick = 0;
            LED_Toggle(BLINK_LED);
        }

        /* Temperature refresh every 1000 ms */
        if (g_temp_tick) {
            g_temp_tick = 0;
            lcd_show_temperature(ADC_ReadTemperature());
        }
    }

    return 0;
}
