#ifndef __TOUCH_H__
#define __TOUCH_H__
#include <stm32f4xx.h>

#define TP_PRES_DOWN 0x80  //����������	  
#define TP_CATH_PRES 0x40  //�а��������� 

//������������
typedef struct
{
    uint8_t(*init)(void);			//��ʼ��������������
    uint8_t(*scan)(uint8_t);				//ɨ�败����.0,��Ļɨ��;1,��������;	  
    uint16_t x[5]; 		//��ǰ����
    uint16_t y[5];		//�����������5������,����������x[0],y[0]����:�˴�ɨ��ʱ,����������,��
                        //x[4],y[4]�洢��һ�ΰ���ʱ������. 
    uint8_t  sta;					//�ʵ�״̬ 
                                    //b7:����1/�ɿ�0; 
                                    //b6:0,û�а�������;1,�а�������. 
                                    //b5:����
                                    //b4~b0:���ݴ��������µĵ���(0,��ʾδ����,1��ʾ����)
                                    /////////////////////������У׼����(����������ҪУ׼)//////////////////////								
    float xfac;
    float yfac;
    short xoff;
    short yoff;
    //�����Ĳ���,��������������������ȫ�ߵ�ʱ��Ҫ�õ�.
    //b0:0,����(�ʺ�����ΪX����,����ΪY�����TP)
    //   1,����(�ʺ�����ΪY����,����ΪX�����TP) 
    //b1~6:����.
    //b7:0,������
    //   1,������ 
    uint8_t touchtype;
}_m_tp_dev;

extern _m_tp_dev tp_dev;	 	//������������touch.c���涨��

                                //������оƬ��������	   
#define PEN  		GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)  		//T_PEN
#define DOUT 		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)   	//T_MISO
#define TDIN_0 		GPIO_ResetBits(GPIOB, GPIO_Pin_15)  	//T_MOSI
#define TDIN_1 		GPIO_SetBits(GPIOB, GPIO_Pin_15)  		//T_MOSI
#define TCLK_0 		GPIO_ResetBits(GPIOB, GPIO_Pin_13)  	//T_SCK
#define TCLK_1 		GPIO_SetBits(GPIOB, GPIO_Pin_13)  		//T_SCK
#define TCS_0  		GPIO_ResetBits(GPIOB, GPIO_Pin_12); 	//T_CS
#define TCS_1  		GPIO_SetBits(GPIOB, GPIO_Pin_12); 		//T_CS

                                //����������
void TP_Write_Byte(uint8_t num);						//�����оƬд��һ������
uint16_t TP_Read_AD(uint8_t CMD);							//��ȡADת��ֵ
uint16_t TP_Read_XOY(uint8_t xy);							//���˲��������ȡ(X/Y)
uint8_t TP_Read_XY(uint16_t *x, uint16_t *y);					//˫�����ȡ(X+Y)
uint8_t TP_Read_XY2(uint16_t *x, uint16_t *y);					//����ǿ�˲���˫���������ȡ                                                                                                                                          //������/������ ���ú���
uint8_t TP_Scan(uint8_t tp);								//ɨ��
uint8_t TP_Init(void);								//��ʼ��
uint8_t GET_TP_XY(uint16_t* location);



#endif
