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
#include <math.h>
#include <stdlib.h>

static motor_dir_t direction  = MOTOR_FWD;     // ��¼����
static uint16_t    dutyfactor = 0;             // ��¼ռ�ձ�

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
  
  SET_FWD_COMPAER(0);     // �����ٶ�Ϊ 0
  SET_REV_COMPAER(0);     // �����ٶ�Ϊ 0

  HAL_Delay(200);         // ��ʱһ��
  
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
#define get_actual()    123    // ��ȡ�������ķ���ֵ
/**
  * @brief  ���λ��ʽ PID ����ʵ��(��ʱ����)
  * @param  ��
  * @retval ��
  */
void motor_pid_control(void)
{
  float cont_val = 0;    // ��ǰ����ֵ
  
  uint32_t actual = get_actual();    // �����ת�ĵ�ǰ�ٶ�
  
  cont_val = PID_realize(actual);
  
  set_motor_speed(cont_val);
}
/*********************************************END OF FILE**********************/

