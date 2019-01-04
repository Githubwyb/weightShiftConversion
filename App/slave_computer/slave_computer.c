#include "slave_computer.h"
#include "lcd.h"
#include "touch.h"
#include "motor.h"
#include "pid.h"
#include "math.h"
#include "string.h"

#define PAGE_MAX 2
#define DATA_X 120
#define NAME_X 20

uint8_t page_now = 1;       //当前页数
uint8_t line_now = 1;       //当前列数
uint8_t data_now = 1;       //当前操作数字位数
uint8_t touch_key = 0;      //当前按下的键值
uint8_t touch_flag = 0;     //触屏检测
uint8_t test_data = 0;      //测试数据
uint8_t clear_flag = 1;     //清楚标志位
uint8_t is_touch = 0;       //触屏是否被按下
int32_t pwm_now = 0;        //当前电机pwm值
uint8_t start = 0;          //开始信号
uint8_t data_line[13] = {5,20,35,50,65,80,95,110,125,140,155,170,185};
uint8_t clear = 0;
uint8_t name[13][255];

double d_adc = 0;
double length_n = 0;
double length_exp = 0;
int32_t adc_weight = 0;
int32_t adc_weight_erro = 0;
extern uint8_t mode;

extern uint16_t adc_0;      //adc0获取值
extern uint16_t adc_1;      //adc1获取值
extern pid_struct motor_pwm;

void Page_1(void)
{
    static uint8_t n = 4;

    strcpy((char*)name[0], "mode:");
    strcpy((char*)name[1], "start:");
    strcpy((char*)name[2], "clear:");
    strcpy((char*)name[3], "length_exp:");
    strcpy((char*)name[4], "pwm_now:");
    strcpy((char*)name[5], "adc_0:");
    strcpy((char*)name[6], "adc_1:");
    strcpy((char*)name[7], "d_adc:");
    strcpy((char*)name[8], "length_n:");
    strcpy((char*)name[9], "adc_weight:");
    strcpy((char*)name[10], "weight(g):");

    int32_t data[12][3];

    data[0][0] = mode;
    data[0][1] = 3;
    data[0][2] = 0;

    data[1][0] = start;
    data[1][1] = 1;
    data[1][2] = 0;

    data[2][0] = clear;
    data[2][1] = 1;
    data[2][2] = 0;

    data[3][0] = length_exp * 1000;
    data[3][1] = 30000;
    data[3][2] = -30000;

    data[4][0] = pwm_now;

    data[5][0] = adc_0;

    data[6][0] = adc_1;

    data[7][0] = d_adc * 1000;

    data[8][0] = length_n * 1000;

    data[9][0] = adc_weight;

    if(mode == 2)
        data[10][0] = (length_exp + 20) * 100;
    else
        data[10][0] = 0;

    Page(name, data, n, 11 - n);

    mode = data[0][0];

    start = data[1][0];

    clear = data[2][0];

    length_exp = data[3][0] / 1000.0;

    if(mode <= 1)
        n = 4;
    else
        n = 3;

    if(clear)
        adc_weight_erro = adc_weight;

    if(!start)
        motor_pwm.integ = 0;
    
    if(line_now != 2)
        start = 0;

    if(line_now != 3)
        clear = 0;
}

void Page_2(void)
{
    strcpy((char*)name[0], "p");
    strcpy((char*)name[1], "i");
    strcpy((char*)name[2], "d");
    strcpy((char*)name[3], "pwm_now");

    int32_t data[4][3];

    data[0][0] = motor_pwm.kp * 1000;
    data[0][1] = 1000000;
    data[0][2] = -1000000;

    data[1][0] = motor_pwm.ki * 1000;
    data[1][1] = 1000000;
    data[1][2] = -1000000;

    data[2][0] = motor_pwm.kd * 1000;
    data[2][1] = 1000000;
    data[2][2] = -1000000;

    data[3][0] = pwm_now;

    Page(name, data, 3, 1);

    motor_pwm.kp = data[0][0] / 1000.0;

    motor_pwm.ki = data[1][0] / 1000.0;

    motor_pwm.kd = data[2][0] / 1000.0;
}

void SHOW_Pages(void)
{
    touch_key = GET_touch_key();

    if(is_touch && !touch_flag)
    {
        clear_flag = 1;
        LCD_Clear(WHITE);
    }
    
    is_touch = touch_flag;

    switch(touch_key)
    {
        case 1:
            if(page_now != 1)
                page_now--;
            line_now = 1;
            data_now = 1;
            start = 0;
            break;
        case 3:
            if(page_now != PAGE_MAX)
                page_now++;
            line_now = 1;
            data_now = 1;
            start = 0;
            break;
        case 5:
            if(start == 0)
                start = 1;
            else
                start = 0;
            break;
        default:
            break;
    }

    switch(page_now)
    {
        case 1:
            Page_1();
            break;
        case 2:
            Page_2();
            break;
        // case 3:
        //     Page_3();
        //     break;
        // case 4:
        //     Page_4();
        //     break;
        // case 5:
        //     Page_5();
        //     break;
        // case 6:
        //     Page_6();
        //     break;
        default:
            break;
    }

    if(clear_flag)
    {
        clear_flag = 0;
        // LCD_Fill(5, 205, 75, 235, YELLOW);
        // LCD_Fill(85, 205, 155, 235, YELLOW);
        // LCD_Fill(165, 205, 235, 235, YELLOW);
        // LCD_Fill(5, 245, 75, 275, YELLOW);
        // LCD_Fill(85, 245, 155, 275, YELLOW);
        // LCD_Fill(165, 245, 235, 275, YELLOW);
        // LCD_Fill(5, 285, 75, 315, YELLOW);
        // LCD_Fill(85, 285, 155, 315, YELLOW);
        // LCD_Fill(165, 285, 235, 315, YELLOW);

        LCD_DrawRectangle(5, 205, 75, 235);
        LCD_DrawRectangle(85, 205, 155, 235);
        LCD_DrawRectangle(165, 205, 235, 235);
        LCD_DrawRectangle(5, 245, 75, 275);
        LCD_DrawRectangle(85, 245, 155, 275);
        LCD_DrawRectangle(165, 245, 235, 275);
        LCD_DrawRectangle(5, 285, 75, 315);
        LCD_DrawRectangle(85, 285, 155, 315);
        LCD_DrawRectangle(165, 285, 235, 315);

        LCD_ShowString(8, 212, 70, 16, 16, "Page_pre");
        LCD_ShowString(88, 212, 70, 16, 16, "line_pre");
        LCD_ShowString(168, 212, 70, 16, 16, "Page_nex");
        LCD_ShowString(8, 252, 70, 16, 16, "data_pre");
        LCD_ShowString(112, 252, 70, 16, 16, "OK");
        LCD_ShowString(168, 252, 70, 16, 16, "data_nex");
        LCD_ShowString(37, 292, 70, 16, 16, "-");
        LCD_ShowString(88, 292, 70, 16, 16, "line_nex");
        LCD_ShowString(197, 292, 70, 16, 16, "+");
        
        LCD_ShowString(NAME_X,data_line[0],70,12,12,"Page:");
        LCD_ShowNum(DATA_X, data_line[0], page_now, 2, 12);
        LCD_ShowString(NAME_X,data_line[1],70,12,12,"data_plus:");
        LCD_ShowNum(DATA_X, data_line[1], data_now, 2, 12);
        LCD_ShowChar(5, data_line[line_now + 1], '>', 12, 0);
    }
}

void Page(uint8_t name[][255], int32_t data[][3], uint8_t n_data, uint8_t n_show)
{
    uint8_t i = 0;

    for(i = 0;i < n_show;i++)
        LCD_ShowInt(DATA_X, data_line[i + 2 + n_data], data[n_data + i][0], 10, 12);

    switch(touch_key)
    {
        case 2:
            if(line_now != 1)
                line_now--;
            break;
        case 8:
            if(line_now != n_data)
                line_now++;
            break;
        default:
            break;
    }

    data_op(data[line_now - 1], data[line_now - 1][1], data[line_now - 1][2]);

    if(clear_flag)
    {
        for(i = 0;i < n_data + n_show;i++)
            LCD_ShowString(NAME_X,data_line[i + 2],70,12,12,name[i]);

        for(i = 0;i < n_data;i++)
            LCD_ShowInt(DATA_X, data_line[i + 2], data[i][0], 10, 12);
    }
}

uint8_t GET_touch_key(void)
{
//    static uint8_t touch_up = 1;
    static uint8_t key;
    uint16_t location[2];
    touch_flag = GET_TP_XY(location);
    if(!is_touch && touch_flag)
    {
        key = 0;
        if(location[0] > 5 && location[0] < 75 && location[1] > 205 && location[1] < 235)
        {
            key = 1;
            LCD_Fill(5, 205, 75, 235, RED);
        }
        if(location[0] > 85 && location[0] < 155 && location[1] > 205 && location[1] < 235)
        {
            key = 2;
            LCD_Fill(85, 205, 155, 235, RED);
        }
        if(location[0] > 165 && location[0] < 235 && location[1] > 205 && location[1] < 235)
        {
            key = 3;
            LCD_Fill(165, 205, 235, 235, RED);
        }
        if(location[0] > 5 && location[0] < 75 && location[1] > 245 && location[1] < 275)
        {
            key = 4;
            LCD_Fill(5, 245, 75, 275, RED);
        }
        if(location[0] > 85 && location[0] < 155 && location[1] > 245 && location[1] < 275)
        {
            key = 5;
            LCD_Fill(85, 245, 155, 275, RED);
        }
        if(location[0] > 165 && location[0] < 235 && location[1] > 245 && location[1] < 275)
        {
            key = 6;
            LCD_Fill(165, 245, 235, 275, RED);
        }
        if(location[0] > 5 && location[0] < 75 && location[1] > 285 && location[1] < 315)
        {
            key = 7;
            LCD_Fill(5, 285, 75, 315, RED);
        }
        if(location[0] > 85 && location[0] < 155 && location[1] > 285 && location[1] < 315)
        {
            key = 8;
            LCD_Fill(85, 285, 155, 315, RED);
        }
        if(location[0] > 165 && location[0] < 235 && location[1] > 285 && location[1] < 315)
        {
            key = 9;
            LCD_Fill(165, 285, 235, 315, RED);
        }
    }

    if(is_touch && !touch_flag)
        return key;

    return 0;
}

void data_op(int32_t* data, int32_t data_max, int32_t data_min)
{
    int32_t plus;
    plus = pow(10, data_now - 1);
    switch(touch_key)
    {
        case 4:
            if(pow(10, data_now) <= data_max)
                data_now++;
            break;
        case 6:
            if(data_now != 1)
                data_now--;
            break;
        case 7:
            *data -= plus;
            break;
        case 9:
            *data += plus;
            break;
        default:
            break;
    }
    if(*data >= data_max)
        *data = data_max;
    else if(*data <= data_min)
        *data = data_min;
}
