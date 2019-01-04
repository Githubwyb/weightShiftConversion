#ifndef _ADC_H_
#define _ADC_H_
#include <stm32f4xx.h>

void ADC1_Init(void);
uint16_t Get_ADC1(uint8_t ch);
uint16_t Get_ADC1_Average(uint8_t ch, uint8_t times);

#endif
