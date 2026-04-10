/*
 * led.h - LED driver for ATmega32
 *
 * LEDs on PORTB:
 *   PB0 = LED0 (button-controlled)
 *   PB1 = LED1 (button-controlled)
 *   PB2 = LED2 (button-controlled)
 *   PB3 = BLINK_LED (timer-controlled)
 */
#ifndef LED_H
#define LED_H

#include <avr/io.h>
#include <stdint.h>

#define LED_DDR     DDRB
#define LED_PORT    PORTB
#define LED0        PB0
#define LED1        PB1
#define LED2        PB2
#define BLINK_LED   PB3

void    LED_Init(void);
void    LED_On(uint8_t led);
void    LED_Off(uint8_t led);
void    LED_Toggle(uint8_t led);
void    LED_Set(uint8_t led, uint8_t state);

#endif /* LED_H */
