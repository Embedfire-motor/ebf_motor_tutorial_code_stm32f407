#ifndef __BSP_CREAT_S_TAB_H
#define	__BSP_CREAT_S_TAB_H

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "./usart/bsp_debug_usart.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"

extern uint8_t print_flag;
void CalculateSpeedTab(int Vo, int Vt, float Time);

typedef struct 
{
	int		Vo;  //���ٶ� ��λ Step/s
	int		Vt;  //ĩ�ٶ� ��λ Step/s
	int		AccelHalfStep;   //���ٽ׶ΰ�·��	��λ Step   
	int		AccelTotalStep;  //��·�̣����ٽ׶��ܲ��� Step
	float   Form[1000]; // �ٶȱ�� ��λ Step/s
	
}Speed_s;


#define MICRO_STEP                    32    // ������ϸ����
#define FSPR                          200   // ���������Ȧ����


#define ROUNDPS_2_STEPPS(RoundSpeed)  ((RoundSpeed) * FSPR * MICRO_STEP / 60)  // ���ݵ��ת�٣�r/min�������������٣�step/s��

#define MIDDLEVELOCITY(Vo,Vt)         ( ( (Vo) + (Vt) ) / 2 )                  // S�ͼӼ��ټ��ٶε��е��ٶ�
#define ACCELL_INCREASE(Vo,V,T)       ( ( 2 * ((V) - (Vo)) ) / pow((T),2) )    // �Ӽ��ٶ�:���ٶ������� V - V0 = 1/2 *J*t^2
#define ACCEL_DISPLACEMENT(J,T)       ( ( (J) * pow( (T) ,3) ) / 6 )           // ���ٶε�λ����(����)  S = 1/6 *J *t^3
#define ACCEL_TIME(T)                 ( (T) / 2 )    


//#define MICRO_STEP                    32    // ������ϸ����
//#define FSPR                          200   // ���������Ȧ����


//#define ROUNDPS_2_STEPPS(RoundSpeed)  ((RoundSpeed) * FSPR * MICRO_STEP / 60)  // ���ݵ��ת�٣�r/min�������������٣�step/s��

//#define MIDDLEVELOCITY(Vo,Vt)         ( ( (Vo) + (Vt) ) / 2 )                  // S�ͼӼ��ټ��ٶε��е��ٶ�
//#define ACCELL_INCREASE(Vo,V,T)       ( ( 2 * ((V) - (Vo)) ) / pow((T),2) )    // �Ӽ��ٶ�:���ٶ������� V - V0 = 1/2 *J*t^2
//#define ACCEL_DISPLACEMENT(J,T)       ( ( (J) * pow( (T) ,3) ) / 6 )           // ���ٶε�λ����(����)  S = 1/6 *J *t^3
//#define ACCEL_TIME(T)                 ( (T) / 2 )                              // ���ٶεļ� ��������Ҫ��ʱ�� 





#endif 
