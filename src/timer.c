/*
 * timer.c - Timer1 CTC interrupt driver for ATmega32
 *
 * f_interrupt = 8000000 / (1024 * (3905+1)) = ~2 Hz -> 500 ms period
 * g_blink_tick: every 500 ms
 * g_temp_tick:  every 1000 ms (every 2nd tick)
 */
#include "timer.h"

volatile uint8_t g_blink_tick = 0;
volatile uint8_t g_temp_tick  = 0;

#define TIMER1_OCR  3905U

void Timer1_Init(void) {
    TCCR1A = 0;
    TCCR1B = (1 << WGM12);      /* CTC mode, top = OCR1A          */
    OCR1A  = TIMER1_OCR;
    TIMSK |= (1 << OCIE1A);     /* enable compare-match A interrupt */
}

void Timer1_Start(void) {
    TCCR1B |= (1 << CS12) | (1 << CS10); /* prescaler 1024 */
}

void Timer1_Stop(void) {
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}

ISR(TIMER1_COMPA_vect) {
    static uint8_t half = 0;
    g_blink_tick = 1;
    half ^= 1;
    if (half == 0)
        g_temp_tick = 1;
}
