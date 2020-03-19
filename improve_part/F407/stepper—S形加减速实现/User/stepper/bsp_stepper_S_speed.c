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
#include "./stepper/bsp_stepper_S_speed.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

/*

��������õ�����ѧ�ȱ��ٵ���ѧģ�ͣ������ٶ�Ҳ��

�������ٱ仯�ģ�����׼ȷ��˵�Ǽ��ٶȵ��ȱ���ѧģ�͡�

a-t �������� ��V-t���� �뿴 �ĵ���

	|    .
 a|   /|\             
	|  / | \
	| /  |  \            
	|/___|___\_t 		
	0   t/2
		
���Ͼ��Ǽ��ٶȵ��ȱ�ģ�ͣ�Ĭ�ϳ�ʼ�ļ��ٶ�Ϊ0��

��ô�������ģ�;ͺ����׿������һЩ��Ϊ��֪�����ܣ�����

�����������ϵ�ģ�;Ϳ������ÿһʱ�̵ļ��ٶȣ������ȿ� 

0~t/2 ��� a=K*T ������KΪ���ٶ�a��б��

�����ٶȻ��־����ٶȣ����� V = a dt = K*T dt

�ã� V = K*T^2/2

���ٶȻ��־���λ�ƣ����� S = V dt = K*T^2/2 dt 

�ã� S = K*T^3/6

*/

SpeedCalc_TypeDef Speed ;
uint8_t print_flag=0;



/** 
  * �������ܣ� �ٶȱ���㺯��
  * ��������� V0      ���ٶȣ���λ��ת/min
  *           Vt      ĩ�ٶȣ���λ��ת/min
  *           Time    ����ʱ�� ��λ: s
  * �� �� ֵ����
  * ˵    ��: �����ٶ����ߺͼ���ʱ��,�������ܼ���,������ÿһ�����ٶ�ֵ,��������ڴ浱��
  *           ������õ���ѧģ�����ȱ���ֱ���˶�
  *           ���ٶε����������������,��һ�Ǽ��ٶȵ����ļӼ��ٶ�,
  *           �ڶ��Ǽ��ٶȵݼ��ļ����ٶ�,�������߿�����Ϊ�������ĶԳ�(�������е��ٶ�).��������������ʱ�����
  *           �����е��ٶ� Vm = (Vt + Vo)/2;
  *       �Ӽ��ٶ�:
  *           �Ӽ��ٶεļ��ٶ�������һ����ԭ��ĵ�����ֱ��,�Լ��ٶȻ��ֵõ��ľ����ٶȵ�������
  *           ������:Vm - Vo = 1/2 * Jerk * t^2,�õ��Ӽ��ٶ�Jerk.
  *           ���õ�λ�Ʒ��� S = 1/6 * Jerk * t^3
  *       �����ٶ�:
  *           �����ٶε����߸����ڼӼ��ٶζԳ�,�ٶȹ�ʽV = Vt - 1/2 * Jerk * (T-t)^2,
  *           λ�ƹ�ʽ����Ҫͨ�����ٶ���������ֵķ����õ�,S = Vt * t - 1/6 * Jerk * t^3
  *
  *           :�����������Ϲ����ļ����µ�����ͼ���.
  *     
  */
void CalcSpeed(int32_t Vo, int32_t Vt, float Time)
{
  uint8_t Is_Dec = FALSE;     
  int32_t i = 0;
  int32_t Vm =0;              // �м���ٶ�
  int32_t INCACCELStep;       // �Ӽ�������Ĳ���
  int32_t DecAccelStep;       // ����������Ĳ���
  float Jerk = 0;             // �Ӽ��ٶ�
  float Ti = 0;               // ʱ���� dt
  float Sumt = 0;             // ʱ���ۼ���
  float DeltaV = 0;           // �ٶȵ�����dv  
  float TiCube = 0;           // ʱ����������

  if(Vo > Vt )                          // ���ٶȱ�ĩ�ٶȴ�,�������˶�,��ֵ�仯�������˶���ͬ,
  {                                     // ֻ�ǽ����ʱ��ע�⽫�ٶȵ���.    
    Is_Dec = TRUE;
    Speed.Vo = ROUNDPS_2_STEPPS(Vt);    // ����:Step/s
    Speed.Vt = ROUNDPS_2_STEPPS(Vo);    // ĩ��:Step/s
  }
  else
  {
    Is_Dec = FALSE;
    Speed.Vo = ROUNDPS_2_STEPPS(Vo);    
    Speed.Vt = ROUNDPS_2_STEPPS(Vt);    
  }

  Time = ACCEL_TIME(Time);                                    // �õ��Ӽ��ٶε�ʱ��
  Vm =  MIDDLEVELOCITY( Speed.Vo , Speed.Vt );                // �����е��ٶ�
  Jerk = fabs(INCACCEL( Speed.Vo, Vm, Time ));                // �����е��ٶȼ���Ӽ��ٶ�
  INCACCELStep = (int32_t)INCACCELSTEP(Jerk,Time);            // �Ӽ�����Ҫ�Ĳ���
  DecAccelStep = (int32_t)(Speed.Vt * Time - INCACCELStep);   // ��������Ҫ�Ĳ��� S = Vt * Time - S1
  
  /* �����ڴ�ռ����ٶȱ� */
  Speed.AccelStep = DecAccelStep + INCACCELStep;              // ������Ҫ�Ĳ��� 
  if( Speed.AccelStep  % 2 != 0)     // ���ڸ���������ת�����������ݴ��������,���������1
    Speed.AccelStep  += 1;
//  /* mallo�����ڴ�ռ�,�ǵ��ͷ� */
//  Speed.VelocityTab = (float*)(malloc((Speed.AccelStep + 1) * sizeof(float) ));
//  if(Speed.VelocityTab == NULL)
//  {
//    printf("�ڴ治��!���޸����߲���,�����޸�Heap��С\n");
//    return ;
//  }
  /* 
   * Ŀ���S���ٶ������Ƕ�ʱ��ķ���,�����ڿ��Ƶ����ʱ�������Բ����ķ�ʽ����,���������V-t������ת��,
   * �õ�V-S����,����õ����ٶȱ��ǹ��ڲ������ٶ�ֵ.ʹ�ò������ÿһ�����ڿ��Ƶ���.
   */ 
// �����һ���ٶ�  //���ݵ�һ�����ٶ�ֵ�ﵽ��һ����ʱ��
  TiCube  = 6.0f * 1.0f / Jerk;                 // ����λ�ƺ�ʱ��Ĺ�ʽS = 1/2 * J * Ti^3 ��1����ʱ��:Ti^3 = 6 * 1 / Jerk ;
  Ti = pow(TiCube,(1 / 3.0f) );                 // Ti
  Sumt += Ti;
  DeltaV = 0.5f * Jerk * pow(Sumt,2);//��һ�����ٶ�
  Speed.VelocityTab[0] = Speed.Vo + DeltaV;
  
  if( Speed.VelocityTab[0] <= SPEED_MIN )//�Ե�ǰ��ʱ��Ƶ�����ܴﵽ������ٶ�
    Speed.VelocityTab[0] = SPEED_MIN;
  
  for(i = 1; i < Speed.AccelStep; i++)
  {
    /* ����������ٶȾ��Ƕ�ʱ���������Ƶ��,���Լ����ÿһ����ʱ�� */
    /* �õ���i-1����ʱ�� */
     Ti = 1.0f / Speed.VelocityTab[i-1];             // ���ÿ��һ����ʱ�� Ti = 1 / Vn-1
    /* �Ӽ��ٶ��ٶȼ��� */
    if( i < INCACCELStep)
    {
      Sumt += Ti;//��0��ʼ��i��ʱ���ۻ�
      DeltaV = 0.5f * Jerk * pow(Sumt,2);            // �ٶȵı仯��: dV = 1/2 * Jerk * Ti^2;
      Speed.VelocityTab[i] = Speed.Vo + DeltaV;      // �õ��Ӽ��ٶ�ÿһ����Ӧ���ٶ�
      // �����һ����ʱ��,ʱ�䲢���ϸ����Time,��������Ҫ��������,��Ϊ�����ٶε�ʱ��
      if(i == INCACCELStep - 1)
        Sumt  = fabs(Sumt - Time );
    }
    /* �����ٶ��ٶȼ��� */
    else
    {
      Sumt += Ti;                                        // ʱ���ۼ�
      DeltaV = 0.5f * Jerk * pow(fabs( Time - Sumt),2);  // dV = 1/2 * Jerk *(T-t)^2;
      Speed.VelocityTab[i] = Speed.Vt - DeltaV;          // V = Vt - DeltaV ;
    }
  }
  if(Is_Dec == TRUE) // �����˶�
  {
    float tmp_Speed = 0;  
    /* �������� */
    for(i = 0; i< (Speed.AccelStep / 2); i++)
    {
      tmp_Speed = Speed.VelocityTab[i];
      Speed.VelocityTab[i] = Speed.VelocityTab[Speed.AccelStep-1 - i];
      Speed.VelocityTab[Speed.AccelStep-1 - i] = tmp_Speed;
    }
  }
//  ��ӡ����ٶ�ֵ
//  for(i= 0; i < Speed.AccelStep;i++)
//  {
//    printf("Speed[%d] = %f\n",i,Speed.VelocityTab[i]);
//  }
}


///**
//  * @brief  ������ٱ�
//  * @param  Vo  ��ʼ�ٶ�
//  * @param  Vt	tʱ�̵��ٶ�
//  * @param  T	�������ʱ��
//  *	@note 		��
//  * @retval ��
//  */

//void CalculateSpeedTab(int Vo, int Vt, float T)
//{
//	int32_t i = 0;

//	int32_t Vm =0;      // �м���ٶ�
//	float K = 0;   		// �Ӽ��ٶ� ���ٶȵ�б��
//	float Tn = 0;     	// Tnʱ��
//	float DeltaV = 0; 	// ÿһʱ�̵��ٶ�
//	float TimeDel = 0;	// ʱ����

//	Speed.Vo = CONVER(Vo);    // ����:Step/s
//	Speed.Vt = CONVER(Vt);    // ĩ��:Step/s
//	
//	T = T / 2;						//�Ӽ��ٶε�ʱ�䣨���ٶ�б��>0��ʱ�䣩
//	
//	Vm = (Speed.Vo + Speed.Vt) / 2;	//�����е���ٶ�
//	
//	K = ( ( 2 * ((Vm) - (Speed.Vo)) ) / pow((T),2) );// �����е��ٶȼ���Ӽ��ٶ�
//	
//	Speed.AccelHalfStep  = (int)( ( (K) * pow( (T) ,3) ) / 6 );// �Ӽ�����Ҫ�Ĳ���
//	
//	/* �����ڴ�ռ����ٶȱ� */
//	Speed.AccelHalfStep  = abs(Speed.AccelHalfStep ); // ���ټ����ʱ���ֹ���ָ���
//	if( Speed.AccelHalfStep  % 2 != 0)     // ���ڸ���������ת�����������ݴ��������,���������1
//	{
//		Speed.AccelHalfStep  += 1;
//	}
//	Speed.AccelTotalStep = Speed.AccelHalfStep * 2;           // ���ٶεĲ���
//	
//	if(FORM_LEN<Speed.AccelTotalStep)
//	{
//		printf("FORM_LEN ���泤�Ȳ���\r\n,�뽫 FORM_LEN �޸�Ϊ %d \r\n",Speed.AccelTotalStep);
//		return ;
//	}
//	  
//	/* Ŀ����ٶ������Ƕ�ʱ��ķ���,��ʱ���벽����Ӧ�������ڴ˽�ʱ��ֳ�
//		�벽����Ӧ�ķ���,���Ҽ���� ,�����Ϳ��Լ������Ӧ���ٶ�*/
//	TimeDel = T / Speed.AccelHalfStep;

//	for(i = 0; i <= Speed.AccelHalfStep; i++)
//	{
//		Tn = i * TimeDel;						// ��i��ʱ�̵�Tn
//		DeltaV = 0.5f * K * pow(Tn,2);        	// ��Tnʱ������Ӧ���ٶ�  dv = 1/2 * K * t^2;
//		Speed.Form[i] = Speed.Vo + DeltaV;		// �õ�ÿһʱ�̶�Ӧ���ٶ�,���ڿ��ǵ����ٶȲ�Ϊ0�������������Vo������
//																				
//		Speed.Form [ Speed.AccelTotalStep - i] = Speed.Vt - DeltaV ;        // �Ӽ��ٹ���������������ĶԳ�,����ֱ����������ٶ�
//																			// �����ٹ��̶ԳƵ���ٶ�
//	}
//	/* ����ٶȱ����� */
//	for(i = 0; i <= Speed.AccelTotalStep ; i++)
//	{
//		printf("i,%.3f;speed,%.3f\n",(float)i,Speed.Form[i]);	
//	}
//	/* ��ձ� */
//	memset(Speed.Form,0,FORM_LEN*sizeof(float));
//}


/**
  * @brief  �ٶȾ���
	*	@note 	���ж���ʹ�ã�ÿ��һ���жϣ�����һ��
  * @retval ��
  */
void speed_decision(void)
{
	  __IO uint32_t Tim_Count = 0;
  __IO uint32_t tmp = 0;
  __IO float Tim_Pulse = 0;
  __IO static uint8_t i = 0;  
	
  
	if(__HAL_TIM_GET_IT_SOURCE(&TIM_TimeBaseStructure, MOTOR_TIM_IT_CCx) !=RESET)
	{
		// �����ʱ���ж�
		__HAL_TIM_CLEAR_IT(&TIM_TimeBaseStructure, MOTOR_TIM_IT_CCx);
		/******************************************************************/
		
		    i++;     // ��ʱ���жϴ�������ֵ
    if(i == 2) // 2�Σ�˵���Ѿ����һ����������
    {
      i = 0;   // ���㶨ʱ���жϴ�������ֵ
      if(MotionStatus == ACCEL || MotionStatus == DECEL)
      {
        Step_Position ++;
        if(Step_Position  < Speed.AccelStep )
        { 
          Tim_Pulse = T1_FREQ / Speed.VelocityTab[Step_Position];// ���ٶȱ�õ�ÿһ���Ķ�ʱ������ֵ
          if((Tim_Pulse / 2) >= 0xFFFF)
            Tim_Pulse = 0xFFFF;
          Toggle_Pulse = (uint16_t) (Tim_Pulse / 2);
        }
        else
        {
          if(MotionStatus == ACCEL)   
            MotionStatus = AVESPEED;
          else
          {
            MotionStatus = STOP; 
//            free(Speed.VelocityTab);          //  �˶���Ҫ�ͷ��ڴ�
            TIM_CCxChannelCmd(MOTOR_PUL_TIM, MOTOR_PUL_CHANNEL_x, TIM_CCx_DISABLE);// ʹ�ܶ�ʱ��ͨ�� 
            
          }
        }
      }
    }
	
		/**********************************************************************/
		// ���ñȽ�ֵ
		uint32_t tim_count=__HAL_TIM_GET_COUNTER(&TIM_TimeBaseStructure);
		uint32_t tmp = tim_count+Toggle_Pulse;
		__HAL_TIM_SET_COMPARE(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x,tmp);
		
		
	}
}



int32_t  Step_Position   = 0;           // ��ǰλ��
uint16_t    Toggle_Pulse = 0;        // ����Ƶ�ʿ���
uint8_t  MotionStatus    = 0;  



/**
  * ��������: ���������S�ͼӼ����˶�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void stepper_start_run()
{
  Step_Position = 0;
  MotionStatus = ACCEL; // ���Ϊ�˶�״̬
  // ��һ���ٶ���0,��ʱ����0xFFFF��ʼ;
  if(Speed.VelocityTab[0] == 0)
    Toggle_Pulse = 0xFFFF;
  else
    Toggle_Pulse  = (uint32_t)(T1_FREQ/Speed.VelocityTab[0]);
  __HAL_TIM_SET_COUNTER(&TIM_TimeBaseStructure,0);
  __HAL_TIM_SET_COMPARE(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x,(uint16_t)(Toggle_Pulse)); // ���ö�ʱ���Ƚ�ֵ
  TIM_CCxChannelCmd(MOTOR_PUL_TIM, MOTOR_PUL_CHANNEL_x, TIM_CCx_ENABLE);// ʹ�ܶ�ʱ��ͨ�� 
  //STEPMOTOR_OUTPUT_ENABLE();
}


/*! \brief ���̶���ʱ����ٶȣ�ʹ�ò�������ڹ̶�ʱ���ڴﵽĿ���ٶ�
 *  \param start_speed   	��ʼ�ٶ�
 *  \param end_speed  		�����ٶ�
 *  \param time  					ʱ��
 */
void stepper_move_S(int start_speed,int end_speed,float time)
{
	/*�������*/
	CalcSpeed(start_speed,end_speed,time);
	/*��ʼ��ת*/
	stepper_start_run();
}
