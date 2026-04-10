/*
 * button.c - Button driver for ATmega32
 */
#include "button.h"
#include <util/delay.h>

void Button_Init(void) {
    BTN_DDR  &= ~((1 << BTN0) | (1 << BTN1) | (1 << BTN2));
    BTN_PORT |=  ((1 << BTN0) | (1 << BTN1) | (1 << BTN2)); /* pull-ups */
}

uint8_t Button_IsPressed(uint8_t btn) {
    return !(BTN_PIN & (1 << btn));
}

uint8_t Button_WasPressed(uint8_t btn) {
    if (Button_IsPressed(btn)) {
        _delay_ms(20);                     /* debounce settle   */
        if (Button_IsPressed(btn)) {
            while (Button_IsPressed(btn)); /* wait for release  */
            _delay_ms(20);                 /* debounce release  */
            return 1;
        }
    }
    return 0;
}
