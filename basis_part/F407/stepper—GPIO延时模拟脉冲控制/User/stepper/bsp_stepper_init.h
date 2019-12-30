#ifndef __BSP_STEP_MOTOR_INIT_H
#define	__BSP_STEP_MOTOR_INIT_H

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

//���Ŷ���
/*******************************************************/
//Motor ���� 
#define MOTOR_DIR_PIN                  	GPIO_PIN_13   
#define MOTOR_DIR_GPIO_PORT            	GPIOB                     
#define MOTOR_DIR_GPIO_CLK_ENABLE()   	__HAL_RCC_GPIOB_CLK_ENABLE()

////Motor ����

#define MOTOR_PUL_PIN                  	GPIO_PIN_15            
#define MOTOR_PUL_GPIO_PORT            	GPIOA
#define MOTOR_PUL_GPIO_CLK_ENABLE()   	__HAL_RCC_GPIOA_CLK_ENABLE()

//Motor ʹ�� 
#define MOTOR_EN_PIN                  	GPIO_PIN_6
#define MOTOR_EN_GPIO_PORT            	GPIOA                       
#define MOTOR_EN_GPIO_CLK_ENABLE()    	__HAL_RCC_GPIOA_CLK_ENABLE()
	
/************************************************************/
#define HIGH 1	//�ߵ�ƽ
#define LOW 0		//�͵�ƽ

#define ON 1	//��
#define OFF 0		//��

#define CLOCKWISE 			1//˳ʱ��
#define ANTI_CLOCKWISE	0//��ʱ��


//����ʹ������
/* ���κ꣬��������������һ��ʹ�� */
#define MOTOR_EN(x)					HAL_GPIO_WritePin(MOTOR_EN_GPIO_PORT,MOTOR_EN_PIN,x)
#define MOTOR_PLU(x)				HAL_GPIO_WritePin(MOTOR_PUL_GPIO_PORT,MOTOR_PUL_PIN,x)
#define MOTOR_DIR(x)				HAL_GPIO_WritePin(MOTOR_DIR_GPIO_PORT,MOTOR_DIR_PIN,x)

extern void stepper_Init(void);
extern void stepper_turn(int tim,float angle,float subdivide,uint8_t dir);
#endif /* __STEP_MOTOR_INIT_H */
