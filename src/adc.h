/*
 * adc.h - ADC driver for ATmega32
 *
 * Temperature sensor: LM35 on PA0 (ADC channel 0).
 *   LM35 output: 10 mV / C
 *   AVcc reference (5 V), 10-bit ADC
 *   Temp (C x10) = ADC * 500 / 1024
 */
#ifndef ADC_H
#define ADC_H

#include <avr/io.h>
#include <stdint.h>

void     ADC_Init(void);
uint16_t ADC_Read(uint8_t channel);
/* Returns temperature in tenths of C (e.g. 235 = 23.5 C) */
int16_t  ADC_ReadTemperature(void);

#endif /* ADC_H */
