#ifndef __BSP_STEPPER_S_SPEED_H
#define	__BSP_STEPPER_S_SPEED_H

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "./usart/bsp_debug_usart.h"
#include "./stepper/bsp_stepper_init.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"


#define CONVER(speed)  ((speed) * FSPR * MICRO_STEP / 60)  // ���ݵ��ת�٣�r/min�������������٣�step/s��

extern int32_t  Step_Position  ;           // ��ǰλ��
extern uint16_t    Toggle_Pulse;        // ����Ƶ�ʿ���
extern uint8_t  MotionStatus   ;  

//typedef struct 
//{
//	int		Vo;  //���ٶ� ��λ Step/s
//	int		Vt;  //ĩ�ٶ� ��λ Step/s
//	int		AccelHalfStep;   //���ٽ׶ΰ�·��	��λ Step   
//	int		AccelTotalStep;  //��·�̣����ٽ׶��ܲ��� Step
//	float   Form[FORM_LEN]; // �ٶȱ�� ��λ Step/s
//	
//}Speed_s;
/**************************************************************************************/
#define FORM_LEN 	   1000

typedef struct {
	uint8_t 	status;	//״̬
	uint8_t 	dir;		//����
	uint32_t 	pos;		//λ��
	uint32_t  pluse_time;//����ʱ��	
}Stepper_Typedef;

/*S�Ӽ������õ��Ĳ���*/
typedef struct {
  int32_t   Vo;               // ���ٶ�   ��λ Step/s
  int32_t   Vt;               // ĩ�ٶ�   ��λ Step/s
  int32_t AccelStep;          // ���ٶεĲ�����λ Step
  float   Form[FORM_LEN];       // �ٶȱ�� ��λ Step/s  �������������Ƶ��
}SpeedCalc_TypeDef;

extern SpeedCalc_TypeDef Speed ;

#define CW                0 // ˳ʱ��
#define CCW               1 // ��ʱ��

/*����ٶȾ����е��ĸ�״̬*/
#define ACCEL                 1   //  ����״̬
#define AVESPEED              2   //  ����״̬
#define DECEL                 3   //  ����״̬
#define STOP                  0   //  ֹͣ״̬
																												  

/*Ƶ����ز���*/
//��ʱ��ʵ��ʱ��Ƶ��Ϊ��168MHz/(TIM_PRESCALER+1)
//���� �߼���ʱ���� Ƶ��Ϊ168MHz,������ʱ��Ϊ84MHz
//168/(168)=1Mhz
//������Ҫ��Ƶ�ʿ����Լ�����
#define TIM_PRESCALER               168-1 
#define T1_FREQ                               (SystemCoreClock/(TIM_PRESCALER+1)) // Ƶ��ftֵ


/*�����Ȧ����*/
#define STEP_ANGLE				1.8									//��������Ĳ���� ��λ����
#define FSPR              (360.0f/1.8f)         //���������һȦ����������

#define MICRO_STEP        32          				//ϸ����ϸ���� 
#define SPR               (FSPR*MICRO_STEP)   //ϸ�ֺ�һȦ����������

/**/

#define ROUNDPS_2_STEPPS(RPM)                 ((RPM) * FSPR * MICRO_STEP / 60)         // ���ݵ��ת�٣�r/min�������������٣�step/s��
#define MIDDLEVELOCITY(Vo,Vt)                 ( ( (Vo) + (Vt) ) / 2 )                  // S�ͼӼ��ټ��ٶε��е��ٶ� 
#define INCACCEL(Vo,V,T)                      ( ( 2 * ((V) - (Vo)) ) / pow((T),2) )    // �Ӽ��ٶ�:���ٶ�������   V - V0 = 1/2 * J * t^2
#define INCACCELSTEP(J,T)                     ( ( (J) * pow( (T) , 3 ) ) / 6 )         // �Ӽ��ٶε�λ����(����)  S = 1/6 * J * t^3
#define ACCEL_TIME(T)                         ( (T) / 2 )                              // �Ӽ��ٶκͼ����ٶε�ʱ������ȵ�

#define TRUE                                   1
#define FALSE                                  0



#define SPEED_MIN                              (T1_FREQ / (65535.0f))// ���Ƶ��/�ٶ�

extern uint8_t print_flag;
void CalculateSpeedTab(int Vo, int Vt, float Time);
void CalcSpeed(int32_t Vo, int32_t Vt, float Time);
void stepper_move_S(int start_speed,int end_speed,float time);
void speed_decision(void);
#endif 
