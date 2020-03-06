/**
  ******************************************************************************
  * @file    bsp_motor_control.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   ������ƽӿ�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 

#include ".\motor_control\bsp_motor_control.h"
#include "./usart/bsp_debug_usart.h"
#include "./pid/bsp_pid.h"
#include "./tim/bsp_basic_tim.h"
#include "./Encoder/bsp_encoder.h"
#include <math.h>
#include <stdlib.h>

static motor_dir_t direction  = MOTOR_FWD;     // ��¼����
static uint16_t    dutyfactor = 0;             // ��¼ռ�ձ�
static uint8_t    is_motor_en = 0;             // ���ʹ��

/**
  * @brief  ���õ���ٶ�
  * @param  v: �ٶȣ�ռ�ձȣ�
  * @retval ��
  */
void set_motor_speed(uint16_t v)
{
  v = (v > PWM_PERIOD_COUNT) ? PWM_PERIOD_COUNT : v;     // ���޴���
  
  dutyfactor = v;
  
  if (direction == MOTOR_FWD)
  {
    SET_FWD_COMPAER(dutyfactor);     // �����ٶ�
  }
  else
  {
    SET_REV_COMPAER(dutyfactor);     // �����ٶ�
  }
}

/**
  * @brief  ���õ������
  * @param  ��
  * @retval ��
  */
void set_motor_direction(motor_dir_t dir)
{
  direction = dir;
  
//  SET_FWD_COMPAER(0);     // �����ٶ�Ϊ 0
//  SET_REV_COMPAER(0);     // �����ٶ�Ϊ 0

//  HAL_Delay(200);         // ��ʱһ��
  
  if (direction == MOTOR_FWD)
  {
    SET_FWD_COMPAER(dutyfactor);     // �����ٶ�
    SET_REV_COMPAER(0);              // �����ٶ�
  }
  else
  {
    SET_FWD_COMPAER(0);              // �����ٶ�
    SET_REV_COMPAER(dutyfactor);     // �����ٶ�
  }
  
}

/**
  * @brief  ʹ�ܵ��
  * @param  ��
  * @retval ��
  */
void set_motor_enable(void)
{
  is_motor_en = 1;
  MOTOR_FWD_ENABLE();
  MOTOR_REV_ENABLE();
}

/**
  * @brief  ���õ��
  * @param  ��
  * @retval ��
  */
void set_motor_disable(void)
{
  is_motor_en = 0;
  MOTOR_FWD_DISABLE();
  MOTOR_REV_DISABLE();
}

/**
  * @brief  ��ӡ��������
  * @param  ��
  * @retval ��
  */
void show_help(void)
{
    printf("����������������������������Ұ��ֱ�����ٵ��������ʾ���򡪡�������������������������\n\r");
    printf("��������(�Իس�����)��\n\r");
    printf("< ? >       -�����˵�\n\r");
    printf("v[data]     -���õ�����ٶȣ���Χ��0��%d��\n\r", PWM_PERIOD_COUNT);
    printf("d[data]     -���õ���ķ���%d:����ת��%d:����ת\n\r", MOTOR_FWD, MOTOR_REV);
}

/**
  * @brief  �����ڽ��յ�������
  * @param  ��
  * @retval ��
  */
void deal_serial_data(void)
{
    static char showflag =1;
    int dec_temp=0;
    int speed_temp=0;
    
    //���յ���ȷ��ָ���Ϊ1
    char okCmd = 0;
  
    if (showflag)
    {
      show_help();
      showflag = !showflag;
    }

    //����Ƿ���յ�ָ��
    if(receive_cmd == 1)
    {
      if(UART_RxBuffer[0] == 'v' || UART_RxBuffer[0] == 'V')
      {
        //�����ٶ�
        if(UART_RxBuffer[1] == ' ')
        {
          speed_temp = atoi((char const *)UART_RxBuffer+2);
          if(speed_temp>=0 && speed_temp <= PWM_PERIOD_COUNT)
          {
            set_motor_speed(speed_temp);
//            set_pid_actual(speed_temp);    // ���õ����Ŀ���ٶ�
            printf("\n\r�ٶ�: %d\n\r", speed_temp);
            okCmd = 1;
          }
        }
      }
      else if(UART_RxBuffer[0] == 'd')
      {
        //���÷���
        if(UART_RxBuffer[1] == ' ')
        {
          dec_temp = atoi((char const *)UART_RxBuffer+2);

          if(dec_temp>=0)
          {
            set_motor_direction((motor_dir_t)dec_temp);
            printf("\n\r����:%s\n\r", dec_temp ? "����ת" : "����ת");
            okCmd = 1;
          }
        }
      }
      else if(UART_RxBuffer[0] == '?')
      {
        //��ӡ��������
        show_help();
        okCmd = 1;
      }
      //���ָ���������ӡ��������
      if(okCmd != 1)
      {
        printf("\n\r ������������������...\n\r");
        show_help();
      }

      //��մ��ڽ��ջ�������
      receive_cmd = 0;
      uart_FlushRxBuffer();
    }
}

/**
  * @brief  ���λ��ʽ PID ����ʵ��(��ʱ����)
  * @param  ��
  * @retval ��
  */
void motor_pid_control(void)
{
  if (is_motor_en == 1)     // �����ʹ��״̬�²Ž��п��ƴ���
  {
    float cont_val = 0;                       // ��ǰ����ֵ
    static __IO int32_t Capture_Count = 0;    // ��ǰʱ���ܼ���ֵ
    static __IO int32_t Last_Count = 0;       // ��һʱ���ܼ���ֵ
    int32_t actual_speed = 0;                   // ʵ�ʲ���ٶ�
    
    /* ��ǰʱ���ܼ���ֵ = ������ֵ + ����������� * ENCODER_TIM_PERIOD  */
    Capture_Count =__HAL_TIM_GET_COUNTER(&TIM_EncoderHandle) + (Encoder_Overflow_Count * ENCODER_TIM_PERIOD);
    
    /* ת��ת�� = ��λʱ���ڵļ���ֵ / �������ֱܷ��� * ʱ��ϵ��  */
    actual_speed = ((float)(Capture_Count - Last_Count) / ENCODER_TOTAL_RESOLUTION / REDUCTION_RATIO) * 20 * 60;
    
    /* ��¼��ǰ�ܼ���ֵ������һʱ�̼���ʹ�� */
    Last_Count = Capture_Count;
    
    cont_val = PID_realize(actual_speed);    // ���� PID ����
    
    if (cont_val > 0)    // �жϵ������
    {
      set_motor_direction(MOTOR_FWD);
    }
    else
    {
      cont_val = -cont_val;
      set_motor_direction(MOTOR_REV);
    }
    
    cont_val = (cont_val > PWM_PERIOD_COUNT) ? PWM_PERIOD_COUNT : cont_val;    // �ٶ����޴���
    set_motor_speed(cont_val);                                                 // ���� PWM ռ�ձ�
    
  #if PID_ASSISTANT_EN
    set_computer_value(SET_FACT_CMD, CURVES_CH1, actual_speed);                // ��ͨ�� 1 ����ʵ��ֵ
  #else
    printf("ʵ��ֵ��%d. Ŀ��ֵ��%.0f\n", actual_speed, get_pid_actual());      // ��ӡʵ��ֵ��Ŀ��ֵ
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
//  if(htim==(&TIM_TimeBaseStructure))
//  {
//    motor_pid_control();
//  }
//}

/*********************************************END OF FILE**********************/

