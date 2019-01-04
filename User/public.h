#ifndef _PUBLIC_H_
#define _PUBLIC_H_
#include <stm32f4xx.h>

//driver
#include "delay.h"
#include "usart.h"
#include "tim.h"

//device
#include "square.h"
#include "led.h"
#include "lcd.h"
#include "touch.h"
#include "hx711.h"
#include "motor.h"
#include "adc.h"

//slave_computer
#include "slave_computer.h"

//control
#include "main_control.h"

//algorithm
#include "handle.h"
#include "pid.h"

void STM32_Init(void);

#endif // _PUBLIC_H
