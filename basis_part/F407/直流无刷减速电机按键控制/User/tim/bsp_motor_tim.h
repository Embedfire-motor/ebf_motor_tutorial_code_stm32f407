#ifndef __ADVANCE_TIM_H
#define	__ADVANCE_TIM_H

#include "stm32f4xx.h"

/* ������ƶ�ʱ�� */
#define ADVANCE_TIM           				      TIM8
#define ADVANCE_TIM_CLK_ENABLE()  			    __TIM8_CLK_ENABLE()

/* �ۼ� TIM_Period�������һ�����»����ж�		
	����ʱ����0������4999����Ϊ5000�Σ�Ϊһ����ʱ���� */
#define PWM_PERIOD_COUNT     (5000-1)

/* �߼����ƶ�ʱ��ʱ��ԴTIMxCLK = HCLK=168MHz 
	 �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(PWM_PRESCALER_COUNT+1)/PWM_PERIOD_COUNT = 20KHz*/
#define PWM_PRESCALER_COUNT     (168-1)

/* TIM8ͨ��1������� */
#define ADVANCE_OCPWM1_PIN           		    GPIO_PIN_5
#define ADVANCE_OCPWM1_GPIO_PORT     		    GPIOI
#define ADVANCE_OCPWM1_GPIO_CLK_ENABLE() 	  __GPIOI_CLK_ENABLE()
#define ADVANCE_OCPWM1_AF					          GPIO_AF3_TIM8

/* TIM8ͨ��1����������� */
#define ADVANCE_OCNPWM1_PIN            		  GPIO_PIN_13
#define ADVANCE_OCNPWM1_GPIO_PORT      		  GPIOH
#define ADVANCE_OCNPWM1_GPIO_CLK_ENABLE()	  __GPIOH_CLK_ENABLE()
#define ADVANCE_OCNPWM1_AF					        GPIO_AF3_TIM8

/* TIM8ͨ��2������� */
#define ADVANCE_OCPWM2_PIN           		    GPIO_PIN_6
#define ADVANCE_OCPWM2_GPIO_PORT     		    GPIOI
#define ADVANCE_OCPWM2_GPIO_CLK_ENABLE() 	  __GPIOI_CLK_ENABLE()
#define ADVANCE_OCPWM2_AF					          GPIO_AF3_TIM8

/* TIM8ͨ��2����������� */
#define ADVANCE_OCNPWM2_PIN            		  GPIO_PIN_14
#define ADVANCE_OCNPWM2_GPIO_PORT      		  GPIOH
#define ADVANCE_OCNPWM2_GPIO_CLK_ENABLE()	  __GPIOH_CLK_ENABLE()
#define ADVANCE_OCNPWM2_AF					        GPIO_AF3_TIM8

/* TIM8ͨ��3������� */
#define ADVANCE_OCPWM3_PIN           		    GPIO_PIN_7
#define ADVANCE_OCPWM3_GPIO_PORT     		    GPIOI
#define ADVANCE_OCPWM3_GPIO_CLK_ENABLE() 	  __GPIOI_CLK_ENABLE()
#define ADVANCE_OCPWM3_AF					          GPIO_AF3_TIM8

/* TIM8ͨ��3����������� */
#define ADVANCE_OCNPWM3_PIN            		  GPIO_PIN_15
#define ADVANCE_OCNPWM3_GPIO_PORT      		  GPIOH
#define ADVANCE_OCNPWM3_GPIO_CLK_ENABLE()	  __GPIOH_CLK_ENABLE()
#define ADVANCE_OCNPWM3_AF					          GPIO_AF3_TIM8

/* TIM8��·�������� */
//#define ADVANCE_BKIN_PIN              	   	GPIO_PIN_6              
//#define ADVANCE_BKIN_GPIO_PORT        		  GPIOA                      
//#define ADVANCE_BKIN_GPIO_CLK_ENABLE()  	  __GPIOA_CLK_ENABLE()
//#define ADVANCE_BKIN_AF						          GPIO_AF3_TIM8

/* ������������ʱ�� */
#define HALL_TIM           				      TIM5
#define HALL_TIM_CLK_ENABLE()  			    __TIM5_CLK_ENABLE()

/* �ۼ� TIM_Period�������һ�����»����ж�		
	����ʱ����0������4999����Ϊ5000�Σ�Ϊһ����ʱ���� */
#define HALL_PERIOD_COUNT     (0xFFFF)

/* �߼����ƶ�ʱ��ʱ��ԴTIMxCLK = HCLK / 2 = 84MHz
	 �趨��ʱ��Ƶ��Ϊ = TIMxCLK / (PWM_PRESCALER_COUNT + 1) / PWM_PERIOD_COUNT = 7.6Hz
   ���� T = 131ms */
#define HALL_PRESCALER_COUNT     (168-1)

/* TIM5 ͨ�� 1 ���� */
#define HALL_INPUT1_PIN           		    GPIO_PIN_10
#define HALL_INPUT1_GPIO_PORT     		    GPIOH
#define HALL_INPUT1_GPIO_CLK_ENABLE() 	  __GPIOH_CLK_ENABLE()
#define HALL_INPUT1_AF					          GPIO_AF2_TIM5

/* TIM5 ͨ�� 2 ���� */
#define HALL_INPUT2PIN           		      GPIO_PIN_11
#define HALL_INPUT2GPIO_PORT     		      GPIOH
#define HALL_INPUT2GPIO_CLK_ENABLE() 	    __GPIOH_CLK_ENABLE()
#define HALL_INPUT2AF					            GPIO_AF2_TIM5

/* TIM5 ͨ�� 3 ���� */
#define HALL_INPUT3_PIN           		    GPIO_PIN_12
#define HALL_INPUT3_GPIO_PORT     		    GPIOH
#define HALL_INPUT3_GPIO_CLK_ENABLE() 	  __GPIOH_CLK_ENABLE()
#define HALL_INPUT3_AF					          GPIO_AF2_TIM5

#define HALL_TIM_IRQn                    TIM4_IRQn
#define HALL_TIM_IRQHandler              TIM4_IRQHandler

extern TIM_HandleTypeDef TIM_TimeBaseStructure;

void TIMx_Configuration(void);

#endif /* __ADVANCE_TIM_H */

