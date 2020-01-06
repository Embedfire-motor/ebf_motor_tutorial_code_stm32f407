#ifndef __BSP_STEPPER_T_SPEED_H
#define	__BSP_STEPPER_T_SPEED_H

#include "stm32f4xx.h"
#include "./stepper/bsp_stepper_init.h"
#include "math.h"

//���μӼ�����ر���
typedef struct {
  __IO uint8_t  run_state ;  // �����ת״̬
  __IO uint8_t  dir ;        // �����ת����
  __IO int32_t  step_delay;  // �¸��������ڣ�ʱ������������ʱΪ���ٶ�
  __IO uint32_t decel_start; // ��������λ��
  __IO int32_t  decel_val;   // ���ٽ׶β���
  __IO int32_t  min_delay;   // ��С��������(����ٶȣ������ٶ��ٶ�)
  __IO int32_t  accel_count; // �Ӽ��ٽ׶μ���ֵ
}speedRampData;





#define FALSE             0
#define TRUE              1
#define CW                0 // ˳ʱ��
#define CCW               1 // ��ʱ��

/*����ٶȾ����е��ĸ�״̬*/
#define STOP              0 // �Ӽ�������״̬��ֹͣ
#define ACCEL             1 // �Ӽ�������״̬�����ٽ׶�
#define DECEL             2 // �Ӽ�������״̬�����ٽ׶�
#define RUN               3 // �Ӽ�������״̬�����ٽ׶�

/*Ƶ����ز���*/
//��ʱ��ʵ��ʱ��Ƶ��Ϊ��168MHz/(TIM_PRESCALER+1)
//���� �߼���ʱ���� Ƶ��Ϊ168MHz,������ʱ��Ϊ84MHz
//168/(5+1)=28Mhz
#define TIM_PRESCALER      5 
#define T1_FREQ           (SystemCoreClock/(TIM_PRESCALER+1)) // Ƶ��ftֵ

/*�����Ȧ����*/
#define STEP_ANGLE				1.8									//��������Ĳ���� ��λ����
#define FSPR              (360.0/1.8)         //���������һȦ����������
#define MICRO_STEP        32          				//ϸ����ϸ���� 
#define SPR               (FSPR*MICRO_STEP)   //ϸ�ֺ�һȦ����������

/*��ѧ����*/
#define ALPHA             ((float)(2*3.14159/SPR))       // ��= 2*pi/spr
#define A_T_x10           ((float)(10*ALPHA*T1_FREQ))
#define T1_FREQ_148       ((float)((T1_FREQ*0.676)/10)) // 0.676Ϊ�������ֵ
#define A_SQ              ((float)(2*100000*ALPHA)) 
#define A_x200            ((float)(200*ALPHA))


extern void speed_decision(void);

#endif
