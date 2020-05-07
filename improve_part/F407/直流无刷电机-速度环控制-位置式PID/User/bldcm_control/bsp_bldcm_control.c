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
  bldcm_data.is_enable = 1;
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
  
  bldcm_data.is_enable = 0;
}

/**
  * @brief  ���λ��ʽ PID ����ʵ��(��ʱ����)
  * @param  ��
  * @retval ��
  */
extern uint8_t dir;
void bldcm_pid_control(void)
{
  if (bldcm_data.is_enable)
  {
    float cont_val = 0;    // ��ǰ����ֵ
    
    int32_t actual = get_motor_speed();   // �����ת�ĵ�ǰ�ٶ�

    cont_val = PID_realize(actual);
//    printf("���Ʊ���ǰ��%0.2f��", cont_val);
    if (cont_val < 0)
    {
      cont_val = -cont_val;
    }
//    else
//    {
//      set_bldcm_direction(MOTOR_FWD);
//    }
    
    cont_val = cont_val > PWM_PERIOD_COUNT ? PWM_PERIOD_COUNT : cont_val;
    set_bldcm_speed(cont_val);
    
    float get_motor_dir(void);
    if (get_motor_dir() != 0)
    {
      actual = -actual;
    }
    
  #ifdef PID_ASSISTANT_EN
    set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual, 1);     // ��ͨ�� 1 ����ʵ��ֵ
  #else
//    printf("ʵ��ֵ��%d. Ŀ��ֵ��%d\n", actual, get_pid_target());
    printf("���Ʊ�����%0.2f��ʵ��ֵ ��%d, Ŀ��ֵ��%.0f, dir = %d\n", cont_val, actual, get_pid_target(), dir);
  #endif
  }
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
