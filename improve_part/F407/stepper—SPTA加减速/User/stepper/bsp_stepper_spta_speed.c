/**
  ******************************************************************************
  * @file    bsp_stepper_spta_speed.c
  * @author  fire
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   �������SPTA�Ӽ����㷨
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
#include "./stepper/bsp_stepper_spta_speed.h"
#include "./stepper/bsp_stepper_init.h"
#include <stdlib.h>

#define MAXSPEED_SPTA 90000 //SPTA����ٶ�  80000
#define ACCSPEED_SPTA 10000 //SPTA���ٶ�   150000

unsigned int spta_maxspeed = MAXSPEED_SPTA;
unsigned int spta_accspeed = ACCSPEED_SPTA;

SPTAData_Typedef spta_data = {0};

/**  @brief ����spta�Ӽ���
  *  @param *pspta��spta���ݽṹ��ָ��
  *  @param steps���ƶ��Ĳ���������Ϊ˳ʱ�룬����Ϊ��ʱ�롣
  *  @param speed_lim������ٶȣ������л�������������������
  *  @param accel�����ٶ�,���ȡֵΪ100��ʵ��ֵΪ100*0.01*rad/sec^2=1rad/sec^2
  */
void Stepper_Move_SPTA(SPTAData_Typedef *pspta, int32_t steps, uint32_t speed_lim, uint32_t accel)
{
  /* �жϷ��� */
  (steps < 0) ? MOTOR_DIR(CCW) : MOTOR_DIR(CW);
  steps = abs(steps);
  
  /*������в�����ʼ��*/
  pspta->steps_required = steps * MICRO_STEP;
  pspta->step_middle = pspta->steps_required >> 1;

  pspta->speed_lim = spta_maxspeed;
  pspta->acceleration = spta_accspeed;

  pspta->step_state = ACCELERATING;

  HAL_TIM_Base_Start_IT(&TIM_StepperHandle);
}

/**
  * @brief  spta�ٶȾ���
  * @param  *pspta��spta���ݽṹ��ָ��
  * @retval ��
  */
void SPTA_Speed_Decision(SPTAData_Typedef *pspta)
{
  static uint8_t overflow_flag = 0;

  /* ���������ź� */
  MOTOR_PUL(LOW);

  /*���ݲ����ۼ����Ƿ�����������Ƿ����һ����������
  �����ۼ����ɲ����ٶ����ۼ��������*/
  pspta->step_accumulator += pspta->step_speed;
  //�ж��Ƿ����
//  overflow_flag = pspta->step_accumulator >> 17;
//  if(overflow_flag != 0)
//  {
//    //��������һ������
//    pspta->steps_taken++;
//    /*���������źŲ���һ����������*/
//    MOTOR_PUL(HIGH);
//    pspta->step_accumulator &= (0 << 17);
//  }
  overflow_flag = pspta->step_accumulator >> 17;
  pspta->step_accumulator -= overflow_flag << 17;
  if (overflow_flag != 0)
  {
    //��������һ������
    pspta->steps_taken++;
    /*���������źŲ���һ����������*/
    MOTOR_PUL(HIGH);
  }

  /*���ݵ����״̬����״̬ת���Լ������任*/
  switch (pspta->step_state)
  {
  /* �����������״̬ */
  case ACCELERATING:
    if (pspta->step_speed >= pspta->speed_lim)
    {
      //���ٽ׶ε���һ����ʱ�̣��ʹﵽ���趨������ٶ�
      pspta->step_speed = pspta->speed_lim;
      //תΪ����ٶ�״̬
      pspta->step_state = UNIFORM;
      break;
    }
    //������ٽ׶εĲ���
    if (overflow_flag)
    {
      pspta->steps_acced++;
    }
    //���ٽ׶Σ��ٶ��ۼ���Ҫ�����趨�ļ��ٶ�ֵ�����ۼ�
    pspta->speed_accumulator += pspta->acceleration;
    //����ж�
//    if((pspta->speed_accumulator >> 17) == 1)
//    {
//      /*����ٶ��ۼ�������������ٶ�����Ҫ���ϸ�ֵ,
//        �Ա��ƶ������ٶ�����������Ϊ�����ۼ������׼��*/
//      pspta->step_speed++;
//      pspta->speed_accumulator &= (0 << 17);
//      //��¼���ٽ׶εĲ���
//      pspta->steps_acced++;
//    }
    overflow_flag = pspta->speed_accumulator >> 17;
    pspta->speed_accumulator -= (overflow_flag << 17);
    if (overflow_flag)
    {
      //����ٶ��ۼ�������������ٶ�����Ҫ���ϸ�ֵ���Ա��ƶ�
      //�����ٶ�����������Ϊ�����ۼ������׼��
      pspta->step_speed += overflow_flag;
    }
    /*although we are ACCELERATING,but middle point reached,we need DECELERATING*/
    if (pspta->step_middle != 0)
    {
      if (pspta->steps_taken > pspta->step_middle)
      {
        pspta->step_state = DECELERATING;
      }
    }
    else if (pspta->steps_taken > 0)
    {
      //ֻ����һ����
      pspta->step_state = DECELERATING;
    }
    break;
  /* �����������״̬ */
  case UNIFORM:
    //������ٶ�����ʱ�����ʣ����Ҫ���еĲ���С����ڼ��ٲ�����Ҫ�����ˣ����������
    //������ͬ
    if (pspta->steps_required - pspta->steps_taken < pspta->steps_acced)
    {
      pspta->step_state = DECELERATING;
    }
    break;
  /* �����������״̬ */
  case DECELERATING:
    //�������趨�����Ժ�ֹͣ����
    if (pspta->steps_taken >= pspta->steps_required)
    {
      //�趨���״̬
      pspta->step_state = IDLE;
      break;
    }
//    //���ٽ׶�����ٽ׶����෴�Ĺ��̣�ԭ��Ҳ��ͬ��ֻ�Ǻܶ�+�ű��-��
//    if (overflow_flag && pspta->steps_acced > 0)
//    {
//      pspta->steps_acced--;
//    }
    pspta->speed_accumulator += pspta->acceleration;
    overflow_flag = pspta->speed_accumulator >> 17;
    pspta->speed_accumulator -= (overflow_flag << 17);
    if (overflow_flag && pspta->step_speed > overflow_flag)
    {
      pspta->step_speed -= overflow_flag;
    }
    break;
  case IDLE:
    pspta->speed_lim = 0;
    pspta->step_accumulator = 0;
    pspta->speed_accumulator = 0;
    pspta->steps_acced = 0;
    pspta->step_speed = 0;
    pspta->steps_taken = 0;
    //ֹͣ��ʱ��
    HAL_TIM_Base_Stop_IT(&TIM_StepperHandle);
    break;
  default:
    break;
  }
}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//  TIMX_IRQHandler_SPTA(&stepper_motor);
//}
