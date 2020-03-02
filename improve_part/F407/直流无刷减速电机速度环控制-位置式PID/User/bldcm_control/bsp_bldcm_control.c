/**
  ******************************************************************************
  * @file    bsp_bldcm_control.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   ��ˢ������ƽӿ�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 

#include ".\bldcm_control\bsp_bldcm_control.h"
#include "./pid/bsp_pid.h"

/* ˽�б��� */
static bldcm_data_t bldcm_data;

/**
  * @brief  �����ʼ��
  * @param  ��
  * @retval ��
  */
void bldcm_init(void)
{
  PWM_TIMx_Configuration();    // ������ƶ�ʱ�������ų�ʼ��
  hall_tim_config();           // ������������ʼ��
}

/**
  * @brief  ���õ���ٶ�
  * @param  v: �ٶȣ�ռ�ձȣ�
  * @retval ��
  */
void set_bldcm_speed(uint16_t v)
{
  bldcm_data.dutyfactor = v;
  
  set_pwm_pulse(v);     // �����ٶ�
}

/**
  * @brief  ���õ������
  * @param  ��
  * @retval ��
  */
void set_bldcm_direction(motor_dir_t dir)
{
  bldcm_data.direction = dir;
}

/**
  * @brief  ��ȡ�����ǰ����
  * @param  ��
  * @retval ��
  */
motor_dir_t get_bldcm_direction(void)
{
  return bldcm_data.direction;
}

/**
  * @brief  ʹ�ܵ��
  * @param  ��
  * @retval ��
  */
void set_bldcm_enable(void)
{
  hall_enable();
}

/**
  * @brief  ���õ��
  * @param  ��
  * @retval ��
  */
void set_bldcm_disable(void)
{
  /* ���û����������ӿ� */
  hall_disable();
  
  /* ֹͣ PWM ��� */
  stop_pwm_output();
}
void Transmit_FB( __IO int32_t *Feedback);
extern uint32_t hall_timer;        // ��ʱ��
extern uint32_t hall_pulse_num;    // ������
/**
  * @brief  ���λ��ʽ PID ����ʵ��(��ʱ����)
  * @param  ��
  * @retval ��
  */
void bldcm_pid_control(void)
{
  float cont_val = 0;    // ��ǰ����ֵ
  
  int actual = (hall_pulse_num / 24) / ((0.099 / 0xFFFF) * hall_timer)/60;    // �����ת�ĵ�ǰ�ٶ�
  
  hall_pulse_num = 0;
  hall_timer = 0;
   
//  cont_val = PID_realize(actual);
//  
//  set_bldcm_speed(cont_val);
  
  Transmit_FB(&actual);
}

///**
//  * @brief  ��ʱ��ÿ100ms����һ���жϻص�����
//  * @param  htim����ʱ�����
//  * @retval ��
//  */
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//    if(htim==(&TIM_TimeBaseStructure))
//    {
//        bldcm_pid_control();
//    }
//}

/*********************************************END OF FILE**********************/
