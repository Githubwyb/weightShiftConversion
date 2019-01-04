#include "touch.h" 
#include "lcd.h"
#include "delay.h"
#include "math.h"

_m_tp_dev tp_dev =
{
    TP_Init,
    TP_Scan,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

//Ĭ��Ϊtouchtype=0������.
uint8_t CMD_RDX = 0XD0;
uint8_t CMD_RDY = 0X90;

//SPIд����
//������ICд��1byte����    
//num:Ҫд�������
void TP_Write_Byte(uint8_t num)
{
    uint8_t count = 0;
    for (count = 0; count<8; count++)
    {
        if (num & 0x80)TDIN_1;
        else TDIN_0;
        num <<= 1;
        TCLK_0;
        delay_us(1);
        TCLK_1;		//��������Ч	        
    }
}
//SPI������ 
//�Ӵ�����IC��ȡadcֵ
//CMD:ָ��
//����ֵ:����������	   
uint16_t TP_Read_AD(uint8_t CMD)
{
    uint8_t count = 0;
    uint16_t Num = 0;
    TCLK_0;		//������ʱ�� 	 
    TDIN_0; 	//����������
    TCS_0; 		//ѡ�д�����IC
    TP_Write_Byte(CMD);//����������
    delay_us(6);//ADS7846��ת��ʱ���Ϊ6us
    TCLK_0;
    delay_us(1);
    TCLK_1;		//��1��ʱ�ӣ����BUSY
    delay_us(1);
    TCLK_0;
    for (count = 0; count<16; count++)//����16λ����,ֻ�и�12λ��Ч 
    {
        Num <<= 1;
        TCLK_0;	//�½�����Ч  	    	   
        delay_us(1);
        TCLK_1;
        if (DOUT)Num++;
    }
    Num >>= 4;   	//ֻ�и�12λ��Ч.
    TCS_1;		//�ͷ�Ƭѡ	 
    return(Num);
}
//��ȡһ������ֵ(x����y)
//������ȡREAD_TIMES������,����Щ������������,
//Ȼ��ȥ����ͺ����LOST_VAL����,ȡƽ��ֵ 
//xy:ָ�CMD_RDX/CMD_RDY��
//����ֵ:����������
#define READ_TIMES 5 	//��ȡ����
#define LOST_VAL 1	  	//����ֵ
uint16_t TP_Read_XOY(uint8_t xy)
{
    uint16_t i, j;
    uint16_t buf[READ_TIMES];
    uint16_t sum = 0;
    uint16_t temp;
    for (i = 0; i<READ_TIMES; i++)buf[i] = TP_Read_AD(xy);
    for (i = 0; i<READ_TIMES - 1; i++)//����
    {
        for (j = i + 1; j<READ_TIMES; j++)
        {
            if (buf[i]>buf[j])//��������
            {
                temp = buf[i];
                buf[i] = buf[j];
                buf[j] = temp;
            }
        }
    }
    sum = 0;
    for (i = LOST_VAL; i<READ_TIMES - LOST_VAL; i++)sum += buf[i];
    temp = sum / (READ_TIMES - 2 * LOST_VAL);
    return temp;
}
//��ȡx,y����
//��Сֵ��������100.
//x,y:��ȡ��������ֵ
//����ֵ:0,ʧ��;1,�ɹ���
uint8_t TP_Read_XY(uint16_t *x, uint16_t *y)
{
    uint16_t xtemp, ytemp;
    xtemp = TP_Read_XOY(CMD_RDX);
    ytemp = TP_Read_XOY(CMD_RDY);
    //if(xtemp<100||ytemp<100)return 0;//����ʧ��
    *x = xtemp;
    *y = ytemp;
    return 1;//�����ɹ�
}

uint8_t GET_TP_XY(uint16_t* location)
{
    uint16_t x, y;
    if(TP_Scan(0))
    {
        TP_Read_XY2(&x, &y);
        location[0] = fabs(246.402455 - 0.062155 * x);
        location[1] = fabs(0.090914 * y - 17.564962);
        return 1;
    }
    else
        return 0;
}

//����2�ζ�ȡ������IC,�������ε�ƫ��ܳ���
//ERR_RANGE,��������,����Ϊ������ȷ,�����������.	   
//�ú����ܴ�����׼ȷ��
//x,y:��ȡ��������ֵ
//����ֵ:0,ʧ��;1,�ɹ���
#define ERR_RANGE 50 //��Χ 
uint8_t TP_Read_XY2(uint16_t *x, uint16_t *y)
{
    uint16_t x1, y1;
    uint16_t x2, y2;
    uint8_t flag;
    flag = TP_Read_XY(&x1, &y1);
    if (flag == 0)return(0);
    flag = TP_Read_XY(&x2, &y2);
    if (flag == 0)return(0);
    if (((x2 <= x1&&x1<x2 + ERR_RANGE) || (x1 <= x2&&x2<x1 + ERR_RANGE))//ǰ�����β�����+-50��
        && ((y2 <= y1&&y1<y2 + ERR_RANGE) || (y1 <= y2&&y2<y1 + ERR_RANGE)))
    {
        *x = (x1 + x2) / 2;
        *y = (y1 + y2) / 2;
        return 1;
    }
    else return 0;
}

//////////////////////////////////////////////////////////////////////////////////		  
//��������ɨ��
//tp:0,��Ļ����;1,��������(У׼�����ⳡ����)
//����ֵ:��ǰ����״̬.
//0,�����޴���;1,�����д���
uint8_t TP_Scan(uint8_t tp)
{
    if (PEN == 0)//�а�������
    {
        if (tp)TP_Read_XY2(&tp_dev.x[0], &tp_dev.y[0]);//��ȡ��������
        else if (TP_Read_XY2(&tp_dev.x[0], &tp_dev.y[0]))//��ȡ��Ļ����
        {
            tp_dev.x[0] = tp_dev.xfac*tp_dev.x[0] + tp_dev.xoff;//�����ת��Ϊ��Ļ����
            tp_dev.y[0] = tp_dev.yfac*tp_dev.y[0] + tp_dev.yoff;
        }
        if ((tp_dev.sta&TP_PRES_DOWN) == 0)//֮ǰû�б�����
        {
            tp_dev.sta = TP_PRES_DOWN | TP_CATH_PRES;//��������  
            tp_dev.x[4] = tp_dev.x[0];//��¼��һ�ΰ���ʱ������
            tp_dev.y[4] = tp_dev.y[0];
        }
    }
    else
    {
        if (tp_dev.sta&TP_PRES_DOWN)//֮ǰ�Ǳ����µ�
        {
            tp_dev.sta &= ~(1 << 7);//��ǰ����ɿ�	
        }
        else//֮ǰ��û�б�����
        {
            tp_dev.x[4] = 0;
            tp_dev.y[4] = 0;
            tp_dev.x[0] = 0xffff;
            tp_dev.y[0] = 0xffff;
        }
    }
    return tp_dev.sta&TP_PRES_DOWN;//���ص�ǰ�Ĵ���״̬
}

//��������ʼ��  		    
//����ֵ:0,û�н���У׼
//       1,���й�У׼
uint8_t TP_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOB,C,Fʱ��

                                                                                //GPIOB1,2��ʼ������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;//PB14 ����Ϊ��������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//PC5 ����Ϊ��������
    GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;//PB12/PB13/PB15����Ϊ�������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���ģʽ
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��	

    TP_Read_XY(&tp_dev.x[0], &tp_dev.y[0]);//��һ�ζ�ȡ��ʼ��	 
    return 1;
}
