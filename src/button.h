/*
 * button.h - Button driver for ATmega32
 *
 * Buttons on PORTD (active-low, internal pull-ups):
 *   PD2 = BTN0  -> toggles LED0
 *   PD3 = BTN1  -> toggles LED1
 *   PD4 = BTN2  -> toggles LED2
 */
#ifndef BUTTON_H
#define BUTTON_H

#include <avr/io.h>
#include <stdint.h>

#define BTN_DDR     DDRD
#define BTN_PORT    PORTD
#define BTN_PIN     PIND
#define BTN0        PD2
#define BTN1        PD3
#define BTN2        PD4

void    Button_Init(void);
uint8_t Button_IsPressed(uint8_t btn);
/* Debounced edge-detect: returns 1 once per press (waits for release). */
uint8_t Button_WasPressed(uint8_t btn);

#endif /* BUTTON_H */
