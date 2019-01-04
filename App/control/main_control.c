#include "main_control.h"

uint16_t adc_0 = 0;
uint16_t adc_1 = 0;
uint8_t mode = 0;

extern int32_t pwm_now;        //当前电机pwm值
extern uint8_t start;          //开始信号
extern double d_adc;
extern pid_struct motor_pwm;
extern double length_n;
extern double length_exp;
extern int32_t adc_weight;

void Control_20ms(void)
{
    GET_d_by_adc();
    length_n = GET_length_n();

    switch(mode)
    {
        case 0:
            break;
        case 1:
            pwm_now = PID_Compulate(&motor_pwm, length_exp, length_n);
            break;
        case 2:
            pwm_now = PID_Compulate(&motor_pwm, length_exp, length_n);
            break;
        case 3:
            break;
        default:
            break;
    }

    if(start)
        MOTOR_Set(pwm_now);
    else
        MOTOR_Set(0);

    LED_Change(LED_0);
}

void Control_main(void)
{
    adc_0 = Get_ADC1_Average(3, 3);
    adc_1 = Get_ADC1_Average(4, 3);

    if(mode == 2)
    {
        adc_weight = GET_HX711();
        length_exp =  GET_length_exp();
    }
}
