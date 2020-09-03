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
#include <stdlib.h>

SPTAData_Typedef spta_data = {0};

/**  @brief ����spta�Ӽ���
  *  @param steps���ƶ��Ĳ�����������ϸ�ֵ�������������Ϊ˳ʱ�룬����Ϊ��ʱ�롣
  *  @param speed_lim������ٶ����ƣ���ֵ��С����spta���ٶ���������Ϊ������������������
  *  @param accel�����ٶ���ֵ
  */
void Stepper_Move_SPTA(int32_t steps, uint32_t speed_lim, uint32_t accel)
{
  SPTAData_Typedef *pspta = &spta_data;
  
  /* �ж��Ƿ����ڼӼ��� */
  if(pspta->step_state != IDLE)
    return;
  
  /* �жϷ��� */
  (steps < 0) ? MOTOR_DIR(CCW) : MOTOR_DIR(CW);
  steps = abs(steps);
  
  /* SPTA���в�����ʼ�� */
  pspta->steps_required = steps * MICRO_STEP;
  pspta->steps_middle = pspta->steps_required >> 1;

  pspta->speed_lim = speed_lim;
  pspta->acceleration = accel;

  pspta->step_state = ACCELERATING;

  /* ������ʱ�� */
  HAL_TIM_Base_Start_IT(&TIM_StepperHandle);
}

/**
  * @brief  spta�ٶȾ���
  * @param  *pspta��spta���ݽṹ��ָ��
  * @retval ��
  */
void SPTA_Speed_Decision(SPTAData_Typedef *pspta)
{
  /* ���������ź� */
  MOTOR_PUL(LOW);

  /* �ٶ�ֵ�ۼӵ������ۼ��� */
  pspta->step_accumulator += pspta->step_speed;
  
  /* �����ۼ����Ƿ���� */
  if((pspta->step_accumulator >> 17) == 1)
  {
    /* �����־���� */
    pspta->step_accumulator &= ~(1 << 17);
    /* ���߲���+1 */
    pspta->steps_taken++;
    /* ���������źŲ���һ���������� */
    MOTOR_PUL(HIGH);
  }

  /* ���ݵ����״̬����״̬ת���Լ��������� */
  switch (pspta->step_state)
  {
    /* SPTA����״̬ */
    case ACCELERATING:
      /* �Ƿ�ﵽ����ٶ�����*/
      if(pspta->step_speed >= pspta->speed_lim)
      {
        /* �ﵽ����ٶ� */
        pspta->step_speed = pspta->speed_lim;
        /* ��ʱ�߹��Ĳ�����Ϊ���ٽ׶εĲ��� */
        pspta->steps_acced = pspta->steps_taken;
        /* תΪ����״̬ */
        pspta->step_state = UNIFORM;
        break;
      }
      
      /* ���ٶ�ֵ�ӵ��ٶ��ۼ��� */
      pspta->speed_accumulator += pspta->acceleration;
      /* �ٶ��ۼ����Ƿ���� */
      if((pspta->speed_accumulator >> 17) == 1)
      {
        /* �����־���� */
        pspta->speed_accumulator &= ~(1 << 17);
        /* ���ٽ׶Σ��ٶ��ۼ�����������ٶ�ֵ���� */
        pspta->step_speed++;
      }
      
      if(pspta->steps_middle != 0)
      {
        /* ����Ѿ����еĲ������е㲽������ʼ���� */
        if (pspta->steps_taken >= pspta->steps_middle)
        {
          pspta->step_state = DECELERATING;
        }
      }
      else if(pspta->steps_taken > 0)
      {
        /* ֻ����һ����ֱ�ӱ�Ϊ����״̬ */
        pspta->step_state = DECELERATING;
      }
      break;
    /* SPTA����״̬ */
    case UNIFORM:
      /* �������н׶Σ���ʣ�ಽ���ܼ��ٽ׶β�������ʼ���� */
      if ((pspta->steps_required - pspta->steps_taken) <= pspta->steps_acced)
      {
        pspta->step_state = DECELERATING;
      }
      break;
    /* SPTA����״̬ */
    case DECELERATING:
      /* �����趨�Ĳ���֮��ֹͣ���� */
      if(pspta->steps_taken >= pspta->steps_required)
      {
        /* תΪ����״̬ */
        pspta->step_state = IDLE;
        break;
      }
      
      /* ���ٶ�ֵ�ӵ��ٶ��ۼ��� */
      pspta->speed_accumulator += pspta->acceleration;
      /* �ٶ��ۼ����Ƿ���� */
      if((pspta->speed_accumulator >> 17) == 1)
      {
        /* �����־���� */
        pspta->speed_accumulator &= ~(1 << 17);
        /* ���ٽ׶Σ��ٶ��ۼ���������ٶ�ֵ�ݼ� */
        pspta->step_speed--;
      }
      break;
    /* SPTA����״̬ */
    case IDLE:
      /* ����spta������� */
      pspta->speed_lim = 0;
      pspta->step_accumulator = 0;
      pspta->speed_accumulator = 0;
      pspta->steps_acced = 0;
      pspta->step_speed = 0;
      pspta->steps_taken = 0;
      /* �رն�ʱ�� */
      HAL_TIM_Base_Stop_IT(&TIM_StepperHandle);
      break;
    default:
      break;
  }
}
