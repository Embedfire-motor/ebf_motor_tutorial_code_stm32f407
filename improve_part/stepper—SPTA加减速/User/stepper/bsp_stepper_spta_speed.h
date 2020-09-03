#ifndef __BSP_STEPPER_SPTA_SPEED_H
#define __BSP_STEPPER_SPTA_SPEED_H

#include "stm32f4xx.h"
#include "./stepper/bsp_stepper_init.h"

/* SPTA����״̬ */
typedef enum
{
  IDLE = 0U,         /* ֹͣ */
  ACCELERATING,      /* ���� */
  UNIFORM,           /* ���� */
  DECELERATING,      /* ���� */
}STEP_STATE_TypeDef;

/* SPTA��ز����ṹ�� */
typedef struct 
{
  __IO int32_t steps_required;       //�����в���
  __IO uint32_t steps_taken;         //�Ѿ����еĲ���
  
  __IO uint32_t steps_acced;         //���ٽ׶εĲ���
  __IO uint32_t steps_middle;        //�ܲ������е㣬��������ʱ�����ò�����Ҫ����
  
  __IO uint32_t acceleration;        //���ٶȵ���ֵ������������
  __IO uint32_t speed_accumulator;   //�ٶ��ۼ�����ÿ���ۼ�step_accel�����ۼ�����17λ��λ��step_speed����
  
  __IO uint32_t step_speed;          //SPTA�ٶ�ֵ��speed_accumulator�����ֵ�ӵ�����
  __IO uint32_t step_accumulator;    //�����ۼ�����ÿ���ۼ�step_speed�����ۼ�����17λ��λ�����һ����������

  __IO uint32_t speed_lim;           //���������ٶ����ƣ������ٶ�С���ٶ�����ʱΪ�������������ڵ����ٶ�����ʱΪ��������
  
  STEP_STATE_TypeDef step_state;     //�������״̬ö��
}SPTAData_Typedef;


/*�����Ȧ����*/
#define STEP_ANGLE        1.8f                 //��������Ĳ���� ��λ����
#define FSPR              (360.0f/STEP_ANGLE)  //���������һȦ����������

#define MICRO_STEP        32                   //ϸ����ϸ���� 
#define SPR               (FSPR*MICRO_STEP)    //ϸ�ֺ�һȦ����������


extern SPTAData_Typedef spta_data;
extern unsigned int spta_maxspeed;
extern unsigned int spta_accspeed;

void Stepper_Move_SPTA(int32_t steps, uint32_t speed_lim, uint32_t accel);
void SPTA_Speed_Decision(SPTAData_Typedef *pspta);

#endif /* __STEP_MOTOR_SPTA_H */

