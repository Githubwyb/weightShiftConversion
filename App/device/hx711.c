#include "hx711.h"
#include "delay.h"

void HX711_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOD, GPIO_Pin_2);
}

int32_t GET_HX711(void)
{
    uint32_t val = 0;
    int32_t data = 0;
    uint8_t i = 0;

    GPIO_SetBits(GPIOD, GPIO_Pin_6);       //DOUT=1 
    GPIO_ResetBits(GPIOD, GPIO_Pin_2);     //SCK=0 
    while (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6))   //等待DOUT=0

    delay_us(1);
    for (i = 0; i < 24; i++)
    {
        GPIO_SetBits(GPIOD, GPIO_Pin_2);       //SCK=1 
        val = val << 1;
        delay_us(1);
        GPIO_ResetBits(GPIOD, GPIO_Pin_2);     //SCK=0 
        if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6))   //DOUT=1 
            val++;
        delay_us(1);
    }
    GPIO_SetBits(GPIOD, GPIO_Pin_2);
    if(val & 0x800000)
    {
        val--;
        val ^= 0xffffff;
        data = -val;
    }
    else
    {
        data = val;
    }
//    val = val ^ 0x800000;
    delay_us(1);
    GPIO_ResetBits(GPIOD, GPIO_Pin_2);
    delay_us(1);
    return data;
}
