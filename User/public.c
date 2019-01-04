#include <public.h>

void STM32_Init(void)
{
    USART1_Init(115200, 1);
    SQUARE_Init();
    MOTOR_Init(1, 4000);
    LCD_Init();
    TP_Init();
    HX711_Init();
    ADC1_Init();
    PID_param_Init();
    TIM_20ms_Init();
}
