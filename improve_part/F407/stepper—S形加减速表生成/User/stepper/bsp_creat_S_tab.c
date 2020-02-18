/**
  ******************************************************************************
  * @file    bsp_creat_S_tab.c
  * @author  fire
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   S�Ӽ������ɱ�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
#include "./stepper/bsp_creat_S_tab.h"

/*
��������õ�����ѧ�ȱ��ٵ���ѧģ�ͣ������ٶ�Ҳ���������ٱ仯�ģ�����׼ȷ��˵�Ǽ��ٶȵ��ȱ���ѧģ�͡�

	| 
   a|   /|\             
	|  / | \
	| /  |  \            
	|/___|___\_t 
	
���Ͼ��Ǽ��ٶȵ��ȱ�ģ�ͣ�Ĭ�ϳ�ʼ�ļ��ٶ�Ϊ0��
��ô�������ģ�;ͺ����׿������һЩ��Ϊ��֪�����ܣ�����


*/

Speed_s Speed ;
uint8_t print_flag=0;


/**
  * @brief  ������ٱ�
  * @param  Vo  ��ʼ�ٶ�
  * @param  Vt	tʱ�̵��ٶ�
  * @param  T	�������ʱ��
  *	@note 		��
  * @retval ��
  */

void CalculateSpeedTab(int Vo, int Vt, float T)
{
  int32_t i = 0;
  
  int32_t Vm =0;      	// �м���ٶ�
  float K = 0;   		// �Ӽ��ٶ� ���ٶȵ�б��
  float Tn = 0;     	// ʱ����
  float DeltaV = 0; 	// �ٶȵ�����dv  
  float TimeDel = 0;

  /* ������õ���ѧģ�����ȱ���ֱ���˶�
   * ���ٶε����������������,��һ�Ǽ��ٶȵ����ļӼ��ٶ�,
   * �ڶ��Ǽ��ٶȵݼ��ļ����ٶ�,�������߿�����Ϊ�������ĶԳ�(�������е��ٶ�).��������������ʱ�����
   * �����е��ٶ� Vm = (Vt + Vo)/2;
   * �Ӽ��ٶ�:
   *     �Ӽ��ٶεļ��ٶ�������һ����ԭ��ĵ�����ֱ��,�Լ��ٶȻ��ֵõ��ľ����ٶȵ�������
   *     ������:Vm - Vo = 1/2 * K * t^2,�õ��Ӽ��ٶ�K.
   *     ���õ�λ�Ʒ��� S = 1/6 K * t^3
   *  :�����������Ϲ����ļ����µ�����ͼ���.
   *     ����S��������V-t����,���������ʱ����еȷ�,Ȼ�����ÿһ�ݵ�ʱ�������ٶȱ�
   */

  Speed.Vo = ROUNDPS_2_STEPPS(Vo);    // ����:Step/s
  Speed.Vt = ROUNDPS_2_STEPPS(Vt);    // ĩ��:Step/s
    
  T = T / 2;						//�Ӽ��ٶε�ʱ�䣨���ٶ�б��>0��ʱ�䣩
  
  Vm = (Speed.Vo + Speed.Vt) / 2;	//�����е���ٶ�
  
  K = ( ( 2 * ((Vm) - (Speed.Vo)) ) / pow((T),2) );// �����е��ٶȼ���Ӽ��ٶ�
		
  
  Speed.AccelHalfStep  = (int)( ( (K) * pow( (T) ,3) ) / 6 );// �Ӽ�����Ҫ�Ĳ���
							
  
  /* �����ڴ�ռ����ٶȱ� */
  Speed.AccelHalfStep  = abs(Speed.AccelHalfStep ); // ���ټ����ʱ���ֹ���ָ���
  if( Speed.AccelHalfStep  % 2 != 0)     // ���ڸ���������ת�����������ݴ��������,���������1
    Speed.AccelHalfStep  += 1;
  
  
  
  Speed.AccelTotalStep = Speed.AccelHalfStep * 2;           // ���ٶεĲ���
	
//	printf("\n���貽�� ��%d\r\n",Speed.AccelTotalStep);
//	printf("\n�����ֽ��� ��%d\r\n",Speed.AccelTotalStep*sizeof(float)+1);
//	return ;

  
	/* Ŀ����ٶ������Ƕ�ʱ��ķ���,��ʱ���벽����Ӧ�������ڴ˽�ʱ��ֳ�
		�벽����Ӧ�ķ���,���Ҽ���� ,�����Ϳ��Լ������Ӧ���ٶ�*/
	TimeDel = T / Speed.AccelHalfStep;

	for(i = 0; i <= Speed.AccelHalfStep; i++)
	{
		Tn = i * TimeDel;						// ��i��ʱ�̵�Tn
		DeltaV = 0.5 * K * pow(Tn,2);        	// ��Tnʱ������Ӧ���ٶ�  dv = 1/2 * K * t^2;
		Speed.Form[i] = Speed.Vo + DeltaV;		// �õ�ÿһʱ�̶�Ӧ���ٶ�,���ڿ��ǵ����ٶȲ�Ϊ0�������������Vo������
																				
		Speed.Form [ Speed.AccelTotalStep - i] = Speed.Vt - DeltaV ;       // �Ӽ��ٹ���������������ĶԳ�,����ֱ����������ٶ�
																				// �����ٹ��̶ԳƵ���ٶ�
	}
  
	/* ����ٶȱ����� */
	for(i = 0; i <= Speed.AccelTotalStep ; i++)
	{
		printf("i,%.3f;speed,%.3f\n",(float)i,Speed.Form[i]);	
	}
	/* ��ձ� */
	memset(Speed.Form,0,1000*sizeof(float));
  
}


