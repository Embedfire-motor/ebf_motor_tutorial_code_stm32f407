#ifndef __BSP_ADC_H
#define	__BSP_ADC_H

#include "stm32f4xx.h"

// ADC GPIO �궨��
#define CURR_ADC_GPIO_PORT              GPIOB
#define CURR_ADC_GPIO_PIN               GPIO_PIN_1
#define CURR_ADC_GPIO_CLK_ENABLE()      __GPIOB_CLK_ENABLE()
    
// ADC ��ź궨��
#define CURR_ADC                        ADC1
#define CURR_ADC_CLK_ENABLE()           __ADC1_CLK_ENABLE()
#define CURR_ADC_CHANNEL                ADC_CHANNEL_9

// ADC DR�Ĵ����궨�壬ADCת���������ֵ����������
#define CURR_ADC_DR_ADDR                ((uint32_t)ADC1+0x4c)

// ADC DMA ͨ���궨�壬��������ʹ��DMA����
#define CURR_ADC_DMA_CLK_ENABLE()       __DMA2_CLK_ENABLE()
#define CURR_ADC_DMA_CHANNEL            DMA_CHANNEL_0
#define CURR_ADC_DMA_STREAM             DMA2_Stream0

#define ADC_DMA_IRQ                DMA2_Stream0_IRQn
#define ADC_DMA_IRQ_Handler        DMA2_Stream0_IRQHandler

#define ADC_NUM_MAX           1024    // ADC ת��������������ֵ

#define VREF                  3.259f     // �ο���ѹ����������3.3����ͨ��ʵ�ʲ�����3.259

#define GET_ADC_VDC_VAL(val)     ((float)val/(float)4096*VREF)          // �õ���ѹֵ
#define GET_ADC_CURR_VAL(val)    ((float)val/10.0/0.02*1000.0)          // �õ�����ֵ����ѹ�Ŵ�10����0.02�ǲ������裬��λmA��

//#define GET_ADC_CURR_VAL(val)    ((val)/10.02/0.02*1000.0)
  
extern DMA_HandleTypeDef DMA_Init_Handle;

void CURR_ADC_Init(void);
int32_t get_curr_val(void);

#endif /* __BSP_ADC_H */



