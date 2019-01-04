#include "handle.h"
#include "math.h"

#define WEIGHT_TO_LENGTH 37122

extern uint16_t adc_0;      //adc0获取值
extern uint16_t adc_1;      //adc1获取值
extern double d_adc;
extern int32_t adc_weight;
extern int32_t adc_weight_erro;

double GET_length_n(void)
{
    double length;
    length = -158.41 * pow(d_adc, 5) + 29.444 * pow(d_adc, 4) - 3.9807 * pow(d_adc, 3) - 7.0017 * pow(d_adc, 2) - 35.524 * pow(d_adc, 1);
    return length;
}

void GET_d_by_adc(void)
{
    d_adc = (adc_0 * 1.0 - adc_1 * 1.0) / (adc_0 * 1.0 + adc_1 * 1.0);
}

double GET_length_exp(void)
{
    double length;
    length = (adc_weight - adc_weight_erro) / 37122.0 - 20;
    return length;
}
