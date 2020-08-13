#ifndef __BSP_STEPPER_SPTA_SPEED_H
#define __BSP_STEPPER_SPTA_SPEED_H

#include "stm32f4xx.h"
#include "./stepper/bsp_stepper_init.h"
#include "math.h"

/* SPTA����״̬ */
typedef enum
{
  IDLE,              /* ֹͣ */
  ACCELERATING,      /* ���� */
  UNIFORM,           /* ���� */
  DECELERATING,      /* ���� */
}STEP_STATE_TypeDef;

//typedef __packed struct 
//{
//  uint32_t step_move     ;        //total move requested,�趨���еĲ���
//  uint32_t step_count    ;        //step counter;�Ѿ����еĲ���
//  
//  uint32_t step_acced    ;        //steps in acceled stage;���ٽ׶εļ��ٲ���
//  uint32_t step_middle   ;        //mid-point of move, = (step_move - 1) >> 1;�趨���в�����1/2�������ò�����Ҫ����
//  
//  uint32_t step_accel    ;        //accel/decel rate, 8.8 bit format���趨�ĵ�����ٶ���ֵ
//  uint32_t speed_frac    ;        //speed counter fraction;�ٶ�Ƭ���ۼ�����ÿ�ζ��ۼ�step_accel������ֵ����ĳ��ֵ��step_speed����
//  
//  uint32_t step_speed    ;        //current speed, 16.8 bit format (HI byte always 0)������Ƭ���ۼ��ٶ�ֵ��speed_frac���ֵ�ӵ�����
//  uint32_t step_frac     ;        //step counter fraction;����Ƭ���ۼ�����ÿ���ۼ�step_speed������ֵ����ĳ��ֵ�����һ���������壬����һ��

//  uint32_t step_spmax    ;        //maximum speed,�趨�ĵ������ٶ� 
//  
//  STEP_STATE_TypeDef step_state;//�����ת״̬���������١����С�����
// 
//  TIM_HandleTypeDef* Handle_x;  
//} MOTOR_CONTROL_SPTA ;

typedef volatile struct 
{
  int32_t steps_required; //Ԥ�Ƶ������в���
  uint32_t steps_taken    ;        //step counter;�Ѿ����еĲ���
  
  uint32_t steps_acced    ;        //���ٽ׶εĲ���
  uint32_t step_middle   ;        //mid-point of move, = (step_move - 1) >> 1;�趨���в�����1/2�������ò�����Ҫ����
  
  uint32_t acceleration;        //���ٶ���ֵ
  uint32_t speed_accumulator;   //�ٶ��ۼ�����ÿ�ζ��ۼ�step_accel������ֵ����ĳ��ֵ��step_speed����
  
  uint32_t step_speed    ;        //current speed, 16.8 bit format (HI byte always 0)������Ƭ���ۼ��ٶ�ֵ��speed_accumulator���ֵ�ӵ�����
  uint32_t step_accumulator;    //�����ۼ�����ÿ���ۼ�step_speed������ֵ����ĳ��ֵ�����һ���������壬����һ��

  uint32_t speed_lim;           //���������ٶ����ƣ���������ʱ��һ���ܴﵽ
  
  STEP_STATE_TypeDef step_state;//�������״̬ö��
} SPTAData_Typedef;


/*�����Ȧ����*/
#define STEP_ANGLE        1.8                 //��������Ĳ���� ��λ����
#define FSPR              (360.0/1.8)         //���������һȦ����������

#define MICRO_STEP        32                  //ϸ����ϸ���� 
#define SPR               (FSPR*MICRO_STEP)   //ϸ�ֺ�һȦ����������


extern SPTAData_Typedef spta_data;
extern unsigned int spta_maxspeed;
extern unsigned int spta_accspeed;

//void SPTA_Init(uint8_t StepDive);
void Stepper_Move_SPTA(SPTAData_Typedef *pspta, int32_t steps, uint32_t speed_lim, uint32_t accel);
void SPTA_Speed_Decision(SPTAData_Typedef *pspta);

#endif /* __STEP_MOTOR_SPTA_H */

