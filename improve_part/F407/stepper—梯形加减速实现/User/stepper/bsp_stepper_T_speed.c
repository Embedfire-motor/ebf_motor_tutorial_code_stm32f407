/**
  ******************************************************************************
  * @file    bsp_stepper_T_speed.c
  * @author  fire
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   ����������μӼ����㷨
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
#include "./stepper/bsp_stepper_T_speed.h"


speedRampData srd               = {STOP,CW,0,0,0,0,0};         // �Ӽ������߱���
__IO int32_t  step_position     = 0;           // ��ǰλ��
__IO uint8_t  MotionStatus      = 0;           //�Ƿ����˶���0��ֹͣ��1���˶�


/*! \brief �Ը����Ĳ����ƶ��������
 *  ͨ��������ٵ�����ٶȣ��Ը����Ĳ�����ʼ����
 *  ������ٶȺͼ��ٶȺ�С������������ƶ���������û�ﵽ����ٶȾ�Ҫ��ʼ����
 *  \param step   �ƶ��Ĳ��� (����Ϊ˳ʱ�룬����Ϊ��ʱ��).
 *  \param accel  ���ٶ�,���ȡֵΪ100��ʵ��ֵΪ100*0.01*rad/sec^2=1rad/sec^2
 *  \param decel  ���ٶ�,���ȡֵΪ100��ʵ��ֵΪ100*0.01*rad/sec^2=1rad/sec^2
 *  \param speed  ����ٶ�,���ȡֵΪ100��ʵ��ֵΪ100*0.01*rad/sec=1rad/sec
 */
void stepper_move_T( int32_t step, uint32_t accel, uint32_t decel, uint32_t speed)
{  
  __IO uint16_t tim_count;
  // �ﵽ����ٶ�ʱ�Ĳ���
  __IO uint32_t max_s_lim;
  // ����Ҫ��ʼ���ٵĲ������������û�дﵽ����ٶȣ�
  __IO uint32_t accel_lim;

  if(MotionStatus != STOP)    // ֻ�����������ֹͣ��ʱ��ż���
    return;
  if(step < 0) // ����Ϊ����
  {
    srd.dir = CCW; // ��ʱ�뷽����ת
    MOTOR_DIR(ANTI_CLOCKWISE);
    step =-step;   // ��ȡ��������ֵ
  }
  else
  {
    srd.dir = CW; // ˳ʱ�뷽����ת
    MOTOR_DIR(CLOCKWISE);
  }
  
  if(step == 1)    // ����Ϊ1
  {
    srd.accel_count = -1;   // ֻ�ƶ�һ��
    srd.run_state = DECEL;  // ����״̬.
    srd.step_delay = 1000;	// ����ʱ	
  }
  else if(step != 0)  // ���Ŀ���˶�������Ϊ0
  {
    // ���ǵĵ������ר��ָ���ֲ�����ϸ�ļ��㼰�Ƶ�����

    // ��������ٶȼ���, ����õ�min_delay���ڶ�ʱ���ļ�������ֵ��
    // min_delay = (alpha / tt)/ w
    srd.min_delay = (int32_t)(A_T_x10/speed);

    // ͨ�������һ��(c0) �Ĳ�����ʱ���趨���ٶȣ�����accel��λΪ0.1rad/sec^2
    // step_delay = 1/tt * sqrt(2*alpha/accel)
    // step_delay = ( tfreq*0.676/10 )*10 * sqrt( (2*alpha*100000) / (accel*10) )/100
    srd.step_delay = (int32_t)((T1_FREQ_148 * sqrt(A_SQ / accel))/10);

    // ������ٲ�֮��ﵽ����ٶȵ�����
    // max_s_lim = speed^2 / (2*alpha*accel)
    max_s_lim = (uint32_t)(speed*speed/(A_x200*accel/10));
    // ����ﵽ����ٶ�С��0.5�������ǽ���������Ϊ0
    // ��ʵ�����Ǳ����ƶ�����һ�����ܴﵽ��Ҫ���ٶ�
    if(max_s_lim == 0){
      max_s_lim = 1;
    }

    // ������ٲ�֮�����Ǳ��뿪ʼ����
    // n1 = (n1+n2)decel / (accel + decel)
    accel_lim = (uint32_t)(step*decel/(accel+decel));
    // ���Ǳ����������1�����ܲ��ܿ�ʼ����.
    if(accel_lim == 0){
      accel_lim = 1;
    }

    // ʹ�������������ǿ��Լ�������ٽ׶β���
    if(accel_lim <= max_s_lim){
      srd.decel_val = accel_lim - step;
    }
    else{
      srd.decel_val = -(max_s_lim*accel/decel);
    }
    // ��ֻʣ��һ�����Ǳ������
    if(srd.decel_val == 0){
      srd.decel_val = -1;
    }

    // ���㿪ʼ����ʱ�Ĳ���
    srd.decel_start = step + srd.decel_val;

    // �������ٶȺ��������ǾͲ���Ҫ���м����˶�
    if(srd.step_delay <= srd.min_delay){
      srd.step_delay = srd.min_delay;
      srd.run_state = RUN;
    }
    else{
      srd.run_state = ACCEL;
    }    
    // ��λ���ٶȼ���ֵ
    srd.accel_count = 0;
  }
  MotionStatus = 1; // ���Ϊ�˶�״̬
  tim_count=__HAL_TIM_GET_COUNTER(&TIM_TimeBaseStructure);
  __HAL_TIM_SET_COMPARE(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x,tim_count+srd.step_delay); // ���ö�ʱ���Ƚ�ֵ
  TIM_CCxChannelCmd(MOTOR_PUL_TIM, MOTOR_PUL_CHANNEL_x, TIM_CCx_ENABLE);// ʹ�ܶ�ʱ��ͨ�� 
  
	MOTOR_EN(ON);
}

/**
  * @brief  �ٶȾ���
	*	@note 	���ж���ʹ�ã�ÿ��һ���жϣ�����һ��
  * @retval ��
  */
void speed_decision()
{
 __IO uint32_t tim_count=0;
  __IO uint32_t tmp = 0;
  // �����£��£�һ����ʱ����
  uint16_t new_step_delay=0;
  // ���ٹ��������һ����ʱ���������ڣ�.
  __IO static uint16_t last_accel_delay=0;
  // ���ƶ�����������
  __IO static uint32_t step_count = 0;
  // ��¼new_step_delay�е������������һ������ľ���
  __IO static int32_t rest = 0;
  //��ʱ��ʹ�÷�תģʽ����Ҫ���������жϲ����һ����������
  __IO static uint8_t i=0;
  
  if(__HAL_TIM_GET_IT_SOURCE(&TIM_TimeBaseStructure, STEPMOTOR_TIM_IT_CCx) !=RESET)
  {
    // �����ʱ���ж�
    __HAL_TIM_CLEAR_IT(&TIM_TimeBaseStructure, STEPMOTOR_TIM_IT_CCx);
    
    // ���ñȽ�ֵ
    tim_count=__HAL_TIM_GET_COUNTER(&TIM_TimeBaseStructure);
    tmp = tim_count+srd.step_delay;
    __HAL_TIM_SET_COMPARE(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x,tmp);

    i++;     // ��ʱ���жϴ�������ֵ
    if(i==2) // 2�Σ�˵���Ѿ����һ����������
    {
      i=0;   // ���㶨ʱ���жϴ�������ֵ
      switch(srd.run_state) // �Ӽ������߽׶�
      {
        case STOP:
          step_count = 0;  // ���㲽��������
          rest = 0;        // ������ֵ
          // �ر�ͨ��
          TIM_CCxChannelCmd(MOTOR_PUL_TIM, MOTOR_PUL_CHANNEL_x, TIM_CCx_DISABLE);        
          __HAL_TIM_CLEAR_FLAG(&TIM_TimeBaseStructure, STEPMOTOR_TIM_FLAG_CCx);
          
					MOTOR_EN(OFF);
				
          MotionStatus = 0;  //  ���Ϊֹͣ״̬     
          break;

        case ACCEL:
          step_count++;      // ������1
          if(srd.dir==CW)
          {	  	
            step_position++; // ����λ�ü�1
          }
          else
          {
            step_position--; // ����λ�ü�1
          }
          srd.accel_count++; // ���ټ���ֵ��1
          new_step_delay = srd.step_delay - (((2 *srd.step_delay) + rest)/(4 * srd.accel_count + 1));//������(��)һ����������(ʱ����)
          rest = ((2 * srd.step_delay)+rest)%(4 * srd.accel_count + 1);// �����������´μ��㲹���������������
          if(step_count >= srd.decel_start)// ����ǹ�Ӧ�ÿ�ʼ����
          {
            srd.accel_count = srd.decel_val; // ���ټ���ֵΪ���ٽ׶μ���ֵ�ĳ�ʼֵ
            srd.run_state = DECEL;           // �¸����������ٽ׶�
          }
          else if(new_step_delay <= srd.min_delay) // ����Ƿ񵽴�����������ٶ�
          {
            last_accel_delay = new_step_delay; // ������ٹ��������һ����ʱ���������ڣ�
            new_step_delay = srd.min_delay;    // ʹ��min_delay����Ӧ����ٶ�speed��
            rest = 0;                          // ������ֵ
            srd.run_state = RUN;               // ����Ϊ��������״̬
          }
          break;

        case RUN:
          step_count++;  // ������1
          if(srd.dir==CW)
          {	  	
            step_position++; // ����λ�ü�1
          }
          else
          {
            step_position--; // ����λ�ü�1
          }
          new_step_delay = srd.min_delay;     // ʹ��min_delay����Ӧ����ٶ�speed��
          if(step_count >= srd.decel_start)   // ��Ҫ��ʼ����
          {
            srd.accel_count = srd.decel_val;  // ���ٲ�����Ϊ���ټ���ֵ
            new_step_delay = last_accel_delay;// �ӽ׶�������ʱ��Ϊ���ٽ׶ε���ʼ��ʱ(��������)
            srd.run_state = DECEL;            // ״̬�ı�Ϊ����
          }
          break;

        case DECEL:
          step_count++;  // ������1
          if(srd.dir==CW)
          {	  	
            step_position++; // ����λ�ü�1
          }
          else
          {
            step_position--; // ����λ�ü�1
          }
          srd.accel_count++;
          new_step_delay = srd.step_delay - (((2 * srd.step_delay) + rest)/(4 * srd.accel_count + 1)); //������(��)һ����������(ʱ����)
          rest = ((2 * srd.step_delay)+rest)%(4 * srd.accel_count + 1);// �����������´μ��㲹���������������
          
          //����Ƿ�Ϊ���һ��
          if(srd.accel_count >= 0)
          {
            srd.run_state = STOP;
          }
          break;
      }      
      srd.step_delay = new_step_delay; // Ϊ�¸�(�µ�)��ʱ(��������)��ֵ
    }
  }



}

