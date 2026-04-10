/*
 * timer.h - Timer1 CTC driver for ATmega32
 *
 * ISR fires every ~500 ms (F_CPU=8MHz, prescaler=1024, OCR1A=3905).
 * Two volatile flags are exported:
 *   g_blink_tick  - set every 500 ms  (blinks BLINK_LED)
 *   g_temp_tick   - set every 1000 ms (refreshes LCD temperature)
 */
#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

extern volatile uint8_t g_blink_tick;
extern volatile uint8_t g_temp_tick;

void Timer1_Init(void);
void Timer1_Start(void);
void Timer1_Stop(void);

#endif /* TIMER_H */
