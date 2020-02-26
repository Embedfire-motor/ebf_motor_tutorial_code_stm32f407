#ifndef __BSP_MOTOR_CONTROL_H
#define	__BSP_MOTOR_CONTROL_H

#include "stm32f4xx.h"
#include "./tim/bsp_general_tim.h"
#include "main.h"

/* ����������ö�� */
typedef enum
{
  MOTOR_FWD = 0,
  MOTOR_REV,
}motor_dir_t;

/* �����ٶȣ�ռ�ձȣ� */
#define SET_FWD_COMPAER(ChannelPulse)     TIM1_SetPWM_pulse(PWM_CHANNEL_1,ChannelPulse)    // ���ñȽϼĴ�����ֵ
#define SET_REV_COMPAER(ChannelPulse)     TIM1_SetPWM_pulse(PWM_CHANNEL_2,ChannelPulse)    // ���ñȽϼĴ�����ֵ

/* ʹ����� */
#define MOTOR_FWD_ENABLE()      HAL_TIM_PWM_Start(&TIM_TimeBaseStructure,PWM_CHANNEL_1);    // ʹ�� PWM ͨ�� 1
#define MOTOR_REV_ENABLE()      HAL_TIM_PWM_Start(&TIM_TimeBaseStructure,PWM_CHANNEL_2);    // ʹ�� PWM ͨ�� 2

/* ������� */
#define MOTOR_FWD_DISABLE()     HAL_TIM_PWM_Stop(&TIM_TimeBaseStructure,PWM_CHANNEL_1);     // ���� PWM ͨ�� 1
#define MOTOR_REV_DISABLE()     HAL_TIM_PWM_Stop(&TIM_TimeBaseStructure,PWM_CHANNEL_2);     // ���� PWM ͨ�� 2

void set_motor_speed(uint16_t v);
void set_motor_direction(motor_dir_t dir);

#endif /* __LED_H */

