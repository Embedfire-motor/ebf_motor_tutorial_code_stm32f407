#include "./stepper/bsp_stepper_init.h"
#include "./led/bsp_led.h"   
#include "./delay/core_delay.h"   
#include "stm32f4xx.h"


TIM_HandleTypeDef TIM_TimeBaseStructure;
 /**
  * @brief  ͨ�ö�ʱ�� TIMx,x[6,7]�ж����ȼ�����
  * @param  ��
  * @retval ��
  */
static void TIMx_NVIC_Configuration(void)
{
    /* �����ж����� */
    HAL_NVIC_SetPriority(GENERAL_TIM_IRQ, 0, 0);
    HAL_NVIC_EnableIRQ(GENERAL_TIM_IRQ);
}

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         ����
 * TIM_CounterMode			 TIMx,x[6,7]û�У��������У�ͨ�ö�ʱ����
 * TIM_Period            ����
 * TIM_ClockDivision     TIMx,x[6,7]û�У���������(ͨ�ö�ʱ��)
 * TIM_RepetitionCounter TIMx,x[1,8]����(�߼���ʱ��)
 *-----------------------------------------------------------------------------
 */
static void TIM_Mode_Config(void)
{

	GENERAL_TIM_CLK_ENABLE();
  TIM_OC_InitTypeDef  TIM_OCInitStructure;  
	
	int tim_per=512;
	
	/*��ʱ������*/
  TIM_TimeBaseStructure.Instance = GENERAL_TIM;
  TIM_TimeBaseStructure.Init.Period = tim_per-1;									//����
  TIM_TimeBaseStructure.Init.Prescaler = 21-1;								//��Ƶϵ��
	TIM_TimeBaseStructure.Init.CounterMode=TIM_COUNTERMODE_UP;	//���ϼ���
	TIM_TimeBaseStructure.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&TIM_TimeBaseStructure);
	
	/*pwm����*/

	TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM1; 
	TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_LOW;          
	TIM_OCInitStructure.Pulse = 0;  
	TIM_OCInitStructure.OCFastMode = TIM_OCFAST_DISABLE;   
	TIM_OCInitStructure.OCIdleState = TIM_OCIDLESTATE_RESET;  


	HAL_TIM_OC_ConfigChannel(&TIM_TimeBaseStructure, &TIM_OCInitStructure, MOTOR_PUL_CHANNEL_x);

	/* ȷ����ʱ�� */
	HAL_TIM_Base_Start(&TIM_TimeBaseStructure);
	/* �����Ƚ������ʹ���ж� */
	HAL_TIM_OC_Start_IT(&TIM_TimeBaseStructure,MOTOR_PUL_CHANNEL_x);
	/*ʹ�ܱȽ�ͨ��*/
	TIM_CCxChannelCmd(GENERAL_TIM,MOTOR_PUL_CHANNEL_x,TIM_CCx_ENABLE);

	// ������ʱ�������ж�
	HAL_TIM_Base_Start_IT(&TIM_TimeBaseStructure);	
}

/**
  * @brief  ��ʼ��ͨ�ö�ʱ����ʱ
  * @param  ��
  * @retval ��
  */
void TIMx_Configuration(void)
{
	TIMx_NVIC_Configuration();	
  
	TIM_Mode_Config();
}

/**
  * @brief  ����TIM�������PWMʱ�õ���I/O
  * @param  ��
  * @retval ��
  */
static void Stepper_GPIO_Config(void) 
{
	GPIO_InitTypeDef GPIO_InitStruct;
	/*����Motor��ص�GPIO����ʱ��*/
	MOTOR_PUL_GPIO_CLK_ENABLE();

  /* ��ʱ��ͨ��1��������IO��ʼ�� */
	/*�����������*/
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	/*������������ */ 
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	/*���ø���*/
  GPIO_InitStruct.Alternate = MOTOR_PUL_GPIO_AF;
	/*���ø���*/
	GPIO_InitStruct.Pull =GPIO_PULLUP;
	/*ѡ��Ҫ���Ƶ�GPIO����*/	
	GPIO_InitStruct.Pin = MOTOR_PUL_PIN;
	/*Motor �������� ��ʼ��*/
  HAL_GPIO_Init(MOTOR_PUL_GPIO_PORT, &GPIO_InitStruct);			
	
}


/**
  * @brief  ����TIMͨ����ռ�ձ�
	* @param  channel		ͨ��	��1,2,3,4��
	* @param  compare		ռ�ձ�
	*	@note 	��
  * @retval ��
  */
void TIM2_SetPWM_pulse(int channel,int compare)
{
		switch(channel)
	{
		case 1:  	__HAL_TIM_SET_COMPARE(&TIM_TimeBaseStructure,TIM_CHANNEL_1,compare);break;
		case 2:	  __HAL_TIM_SET_COMPARE(&TIM_TimeBaseStructure,TIM_CHANNEL_2,compare);break;
		case 3:	  __HAL_TIM_SET_COMPARE(&TIM_TimeBaseStructure,TIM_CHANNEL_3,compare);break;
		case 4:	  __HAL_TIM_SET_COMPARE(&TIM_TimeBaseStructure,TIM_CHANNEL_4,compare);break;
	}
}

/**
  * @brief  ���ų�ʼ��
  * @retval ��
  */
void stepper_Init()
{
		Stepper_GPIO_Config();

		TIMx_Configuration();
}


/**
  * @brief  ��ʱ���жϺ���
	*	@note 		��
  * @retval ��
  */
void  GENERAL_TIM_INT_IRQHandler (void)
{
	HAL_TIM_IRQHandler(&TIM_TimeBaseStructure);	 	
}


/* SPWM��,�������ߣ��˱�ʹ�ù���Ŀ¼�µ�python�ű�sin_wave.py����*/
const uint16_t indexWave[] = {
0, 9, 18, 27, 36, 45, 54, 63, 72, 81, 89, 98,
	107, 116, 125, 133, 142, 151, 159, 168, 176,
	184, 193, 201, 209, 218, 226, 234, 242, 249,
	257, 265, 273, 280, 288, 295, 302, 310, 317, 
	324, 331, 337, 344, 351, 357, 364, 370, 376, 
	382, 388, 394, 399, 405, 410, 416, 421, 426, 
	431, 436, 440, 445, 449, 454, 458, 462, 465, 
	469, 473, 476, 479, 482, 485, 488, 491, 493, 
	496, 498, 500, 502, 503, 505, 506, 508, 509, 
	510, 510, 511, 512, 512, 512, 512, 512, 512,
	511, 510, 510, 509, 508, 506, 505, 503, 502,
	500, 498, 496, 493, 491, 488, 485, 482, 479,
	476, 473, 469, 465, 462, 458, 454, 449, 445, 
	440, 436, 431, 426, 421, 416, 410, 405, 399, 
	394, 388, 382, 376, 370, 364, 357, 351, 344, 
	337, 331, 324, 	317, 310, 302, 295, 288, 280, 
	273, 265, 257, 249, 242, 234, 226, 218, 209, 
	201, 193, 184, 176, 168, 159, 151, 142, 133, 
125, 116, 107, 98, 89, 81, 72, 63, 54, 45, 36,
27, 18, 9, 0
};

//����PWM���ж��ٸ�Ԫ��
uint16_t POINT_NUM = sizeof(indexWave)/sizeof(indexWave[0]); 
/*��ѹ��ֵ�ȼ���*/
#define AMPLITUDE_CLASS 256
//����������ε�Ƶ��
__IO uint16_t period_class = 1;


//�ñ����ڶ�ʱ���жϷ�������ʹ�ã����ڿ��Ƹ�ͨ�������
//�޸ĸñ�����ֵ��ֱ�Ӹı�����Ƶ���ɫ
//������ʽ��RGB888
__IO uint32_t rgb_color = 0xFF00FF;

/**
  * @brief  �ص�����
	*	@note 		��
  * @retval ��
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint16_t pwm_index = 0;			//����PWM���
	static uint16_t period_cnt = 0;		//���ڼ���������
	static uint16_t amplitude_cnt = 0;	//���ڼ����ֵ�ȼ�
	if(htim==(&TIM_TimeBaseStructure))
	{
		amplitude_cnt++;
				
			//ÿ��PWM���е�ÿ��Ԫ����AMPLITUDE_CLASS���ȼ���
		  //ÿ����һ�������һ�����壬��PWM���е�Ԫ�ض�ʹ��һ��
		  //ʹ��256�Σ�����RGB��ɫ��������ͨ�����
			if(amplitude_cnt > (AMPLITUDE_CLASS-1))		 						
			{		
				period_cnt++;
				
				//ÿ��PWM���е�ÿ��Ԫ��ʹ��period_class��
				if(period_cnt > period_class)
				{				

					pwm_index++;												//��־PWM��ָ����һ��Ԫ��
				
					//��PWM���ѵ����β������ָ���ͷ
					if( pwm_index >=  POINT_NUM)			
					{
						pwm_index=0;								
					}
					
					period_cnt = 0;											//�������ڼ�����־
				}
				
				amplitude_cnt=0;											//���÷�ֵ+������־
			}
			else
			{	
					//ÿ��PWM���е�ÿ��Ԫ����AMPLITUDE_CLASS���ȼ���
					//ÿ����һ�������һ�����壬��PWM���е�Ԫ�ض�ʹ��һ��

					//����RGB��ɫ����ֵ�����ø���ͨ���Ƿ������ǰ��PWM��Ԫ�ر�ʾ������
			
					//��
				TIM2_SetPWM_pulse(1,indexWave[pwm_index]);
//					if((rgb_color&0x0000FF) >= amplitude_cnt)				
//						TIM2_SetPWM_pulse(1,indexWave[pwm_index]);	//����PWM���޸Ķ�ʱ���ıȽϼĴ���ֵ
//					else
//						//BRE_BLUE_TIM->BRE_BLUE_CCRx = 0;		//�ȽϼĴ���ֵΪ0��ͨ������ߵ�ƽ����ͨ��LED����	
//						TIM2_SetPWM_pulse(1,0);

			}						
	}
}

