#ifndef __ADVANCE_TIM_H
#define	__ADVANCE_TIM_H

#include "stm32f4xx.h"

#define ADVANCE_TIM           		    TIM8
#define ADVANCE_TIM_CLK_ENABLE()  	  __HAL_RCC_TIM8_CLK_ENABLE()

/* TIM8ͨ��1������� */
#define CHANNEL1_OC_PIN           		  GPIO_PIN_5              
#define CHANNEL1_OC_GPIO_PORT     		  GPIOI                     
#define CHANNEL1_OC_GPIO_CLK_ENABLE() 	__HAL_RCC_GPIOI_CLK_ENABLE()
#define CHANNEL1_OC_AF					        GPIO_AF3_TIM8
#define ADVANCE_TIM_CHANNEL_1           TIM_CHANNEL_1

/* TIM8ͨ��2������� */
#define CHANNEL2_OC_PIN           		  GPIO_PIN_6              
#define CHANNEL2_OC_GPIO_PORT     		  GPIOI                     
#define CHANNEL2_OC_GPIO_CLK_ENABLE() 	__HAL_RCC_GPIOI_CLK_ENABLE()
#define CHANNEL2_OC_AF					        GPIO_AF3_TIM8
#define ADVANCE_TIM_CHANNEL_2           TIM_CHANNEL_2

#define ADVANCE_TIM_IRQn		          TIM8_CC_IRQn
#define ADVANCE_TIM_IRQHandler        TIM8_CC_IRQHandler

/*Ƶ����ز���*/
//��ʱ��ʵ��ʱ��Ƶ��Ϊ��168MHz/(TIM_PRESCALER+1)
//���� �߼���ʱ���� Ƶ��Ϊ168MHz,������ʱ��Ϊ84MHz
//168/(5+1)=28Mhz
//������Ҫ��Ƶ�ʿ����Լ�����
#define TIM_PRESCALER                5
// ���嶨ʱ�����ڣ�����Ƚ�ģʽ��������Ϊ0xFFFF
#define TIM_PERIOD                   0xFFFF

extern TIM_HandleTypeDef TIM_AdvanceHandle;

extern __IO uint16_t OC_Pulse_num_Channel1;     //�Ƚ�����ļ���ֵ
extern __IO uint16_t OC_Pulse_num_Channel2;     //�Ƚ�����ļ���ֵ

void TIMx_AdvanceConfig(void);

#endif /* __ADVANCE_TIM_H */

