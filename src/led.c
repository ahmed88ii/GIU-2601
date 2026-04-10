/*
 * led.c - LED driver for ATmega32
 */
#include "led.h"

void LED_Init(void) {
    LED_DDR  |=  (1 << LED0) | (1 << LED1) | (1 << LED2) | (1 << BLINK_LED);
    LED_PORT &= ~((1 << LED0) | (1 << LED1) | (1 << LED2) | (1 << BLINK_LED));
}

void LED_On(uint8_t led)     { LED_PORT |=  (1 << led); }
void LED_Off(uint8_t led)    { LED_PORT &= ~(1 << led); }
void LED_Toggle(uint8_t led) { LED_PORT ^=  (1 << led); }

void LED_Set(uint8_t led, uint8_t state) {
    if (state) LED_On(led); else LED_Off(led);
}
