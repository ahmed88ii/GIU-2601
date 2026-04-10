/*
 * adc.c - ADC driver for ATmega32
 */
#include "adc.h"

void ADC_Init(void) {
    /* REFS0 = AVcc reference; right-justified result */
    ADMUX  = (1 << REFS0);
    /* ADEN = enable; prescaler 64 -> 8MHz/64 = 125kHz (in 50-200kHz range) */
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
}

uint16_t ADC_Read(uint8_t channel) {
    ADMUX  = (ADMUX & 0xE0) | (channel & 0x07);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

int16_t ADC_ReadTemperature(void) {
    uint16_t raw = ADC_Read(0); /* LM35 on PA0 */
    /* Temp(C x10) = raw * 5000mV / 1024 / 10mV = raw * 500 / 1024 */
    return (int16_t)((uint32_t)raw * 500UL / 1024UL);
}
