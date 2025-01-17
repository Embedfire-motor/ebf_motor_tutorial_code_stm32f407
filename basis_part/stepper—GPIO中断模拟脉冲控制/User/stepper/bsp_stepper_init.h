#ifndef __BSP_STEP_MOTOR_INIT_H
#define __BSP_STEP_MOTOR_INIT_H

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#define MOTOR_PUL_TIM                   TIM2
#define MOTOR_PUL_CLK_ENABLE()          __TIM2_CLK_ENABLE()

#define MOTOR_PUL_IRQn                  TIM2_IRQn
#define MOTOR_PUL_IRQHandler            TIM2_IRQHandler

//引脚定义
/*******************************************************/
//Motor 方向 
#define MOTOR_DIR_PIN                   GPIO_PIN_1   
#define MOTOR_DIR_GPIO_PORT             GPIOE                    
#define MOTOR_DIR_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOE_CLK_ENABLE()

//Motor 使能 
#define MOTOR_EN_PIN                    GPIO_PIN_0
#define MOTOR_EN_GPIO_PORT              GPIOE                       
#define MOTOR_EN_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOE_CLK_ENABLE()

//Motor 脉冲
#define MOTOR_PUL_PIN                   GPIO_PIN_5            
#define MOTOR_PUL_GPIO_PORT             GPIOI
#define MOTOR_PUL_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOI_CLK_ENABLE()  

/************************************************************/
#define HIGH GPIO_PIN_SET       //高电平
#define LOW  GPIO_PIN_RESET     //低电平

#define ON  LOW                 //开
#define OFF HIGH                //关

#define CW  HIGH                //顺时针
#define CCW LOW                 //逆时针

//控制使能引脚
/* 带参宏，可以像内联函数一样使用 */
#define MOTOR_EN(x)         HAL_GPIO_WritePin(MOTOR_EN_GPIO_PORT,MOTOR_EN_PIN,x)
#define MOTOR_PUL(x)        HAL_GPIO_WritePin(MOTOR_PUL_GPIO_PORT,MOTOR_PUL_PIN,x)
#define MOTOR_DIR(x)        HAL_GPIO_WritePin(MOTOR_DIR_GPIO_PORT,MOTOR_DIR_PIN,x)
                          
#define MOTOR_PUL_T()       HAL_GPIO_TogglePin(MOTOR_PUL_GPIO_PORT,MOTOR_PUL_PIN);
  

extern TIM_HandleTypeDef TIM_TimeBaseStructure;
void TIMx_Configuration(void);
extern void stepper_Init(void);
extern void stepper_turn(int tim,float angle,float subdivide,uint8_t dir);
#endif /* __STEP_MOTOR_INIT_H */
