/**
  ******************************************************************************
  * @file    bsp_motor_tim.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   �����ض�ʱ������
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./tim/bsp_motor_tim.h"

TIM_HandleTypeDef  TIM_TimeBaseStructure;
TIM_OC_InitTypeDef TIM_OCInitStructure;

__IO uint16_t ChannelPulse = 500;

/**
  * @brief  ����TIM�������PWMʱ�õ���I/O
  * @param  ��
  * @retval ��
  */
static void TIMx_GPIO_Config(void) 
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*������ʱ����ص�GPIO����ʱ��*/
	ADVANCE_OCPWM1_GPIO_CLK_ENABLE();
	ADVANCE_OCNPWM1_GPIO_CLK_ENABLE();
  ADVANCE_OCPWM2_GPIO_CLK_ENABLE();
	ADVANCE_OCNPWM2_GPIO_CLK_ENABLE();
  ADVANCE_OCPWM3_GPIO_CLK_ENABLE();
	ADVANCE_OCNPWM3_GPIO_CLK_ENABLE();
//	ADVANCE_BKIN_GPIO_CLK_ENABLE(); 

	/* ��ʱ���������ų�ʼ�� */															   
	GPIO_InitStructure.Pin = ADVANCE_OCPWM1_PIN;	
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;    
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH; 	
	GPIO_InitStructure.Alternate = ADVANCE_OCPWM1_AF;
	HAL_GPIO_Init(ADVANCE_OCPWM1_GPIO_PORT, &GPIO_InitStructure);	

	GPIO_InitStructure.Pin = ADVANCE_OCNPWM1_PIN;	
	GPIO_InitStructure.Alternate = ADVANCE_OCNPWM1_AF;	
	HAL_GPIO_Init(ADVANCE_OCNPWM1_GPIO_PORT, &GPIO_InitStructure);
  
  /* ͨ�� 2 */
  GPIO_InitStructure.Pin = ADVANCE_OCPWM2_PIN;	
	GPIO_InitStructure.Alternate = ADVANCE_OCPWM2_AF;	
	HAL_GPIO_Init(ADVANCE_OCPWM2_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = ADVANCE_OCNPWM2_PIN;	
	GPIO_InitStructure.Alternate = ADVANCE_OCNPWM2_AF;	
	HAL_GPIO_Init(ADVANCE_OCNPWM2_GPIO_PORT, &GPIO_InitStructure);
  
  
  /* ͨ�� 3 */
  GPIO_InitStructure.Pin = ADVANCE_OCPWM3_PIN;	
	GPIO_InitStructure.Alternate = ADVANCE_OCPWM3_AF;	
	HAL_GPIO_Init(ADVANCE_OCPWM3_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = ADVANCE_OCNPWM3_PIN;	
	GPIO_InitStructure.Alternate = ADVANCE_OCNPWM3_AF;	
	HAL_GPIO_Init(ADVANCE_OCNPWM3_GPIO_PORT, &GPIO_InitStructure);
	
//	GPIO_InitStructure.Pin = ADVANCE_BKIN_PIN;	
//	GPIO_InitStructure.Alternate = ADVANCE_BKIN_AF;	
//	HAL_GPIO_Init(ADVANCE_BKIN_GPIO_PORT, &GPIO_InitStructure);
}

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         ����
 * TIM_CounterMode			 TIMx,x[6,7]û�У��������У�������ʱ����
 * TIM_Period            ����
 * TIM_ClockDivision     TIMx,x[6,7]û�У���������(������ʱ��)
 * TIM_RepetitionCounter TIMx,x[1,8]����(�߼���ʱ��)
 *-----------------------------------------------------------------------------
 */
static void TIM_Mode_Config(void)
{
//	TIM_BreakDeadTimeConfigTypeDef TIM_BDTRInitStructure;
	// ����TIMx_CLK,x[1,8] 
	ADVANCE_TIM_CLK_ENABLE(); 
	/* ���嶨ʱ���ľ����ȷ����ʱ���Ĵ����Ļ���ַ*/
	TIM_TimeBaseStructure.Instance = ADVANCE_TIM;
	/* �ۼ� TIM_Period�������һ�����»����ж�*/		
	//����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
	TIM_TimeBaseStructure.Init.Period = PWM_PERIOD_COUNT;
	// �߼����ƶ�ʱ��ʱ��ԴTIMxCLK = HCLK=216MHz 
	// �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=1MHz
	TIM_TimeBaseStructure.Init.Prescaler = PWM_PRESCALER_COUNT;	
	// ����ʱ�ӷ�Ƶ
	TIM_TimeBaseStructure.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	// ������ʽ
	TIM_TimeBaseStructure.Init.CounterMode=TIM_COUNTERMODE_UP;
	// �ظ�������
	TIM_TimeBaseStructure.Init.RepetitionCounter=0;	
	// ��ʼ����ʱ��TIMx, x[1,8]
	HAL_TIM_PWM_Init(&TIM_TimeBaseStructure);

	/*PWMģʽ����*/
	//����ΪPWMģʽ1
	TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM1;
	TIM_OCInitStructure.Pulse = ChannelPulse;
	TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;
	TIM_OCInitStructure.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	TIM_OCInitStructure.OCIdleState = TIM_OCIDLESTATE_SET;
	TIM_OCInitStructure.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	
	HAL_TIM_PWM_ConfigChannel(&TIM_TimeBaseStructure,&TIM_OCInitStructure,TIM_CHANNEL_1);    // ��ʼ��ͨ�� 1 ��� PWM 
  HAL_TIM_PWM_ConfigChannel(&TIM_TimeBaseStructure,&TIM_OCInitStructure,TIM_CHANNEL_2);    // ��ʼ��ͨ�� 2 ��� PWM
  HAL_TIM_PWM_ConfigChannel(&TIM_TimeBaseStructure,&TIM_OCInitStructure,TIM_CHANNEL_3);    // ��ʼ��ͨ�� 3 ��� PWM

	/* �Զ����ʹ�ܣ���·������ʱ����������� */
//	TIM_BDTRInitStructure.OffStateRunMode = TIM_OSSR_ENABLE;
//	TIM_BDTRInitStructure.OffStateIDLEMode = TIM_OSSI_ENABLE;
//	TIM_BDTRInitStructure.LockLevel = TIM_LOCKLEVEL_1;
//	TIM_BDTRInitStructure.DeadTime = 11;
//	TIM_BDTRInitStructure.BreakState = TIM_BREAK_ENABLE;
//	TIM_BDTRInitStructure.BreakPolarity = TIM_BREAKPOLARITY_LOW;
//	TIM_BDTRInitStructure.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE;
//	HAL_TIMEx_ConfigBreakDeadTime(&TIM_TimeBaseStructure, &TIM_BDTRInitStructure);

	/* �رն�ʱ��ͨ��1���PWM */
	HAL_TIM_PWM_Stop(&TIM_TimeBaseStructure,TIM_CHANNEL_1);
	/* �رն�ʱ��ͨ��1�������PWM */
	HAL_TIMEx_PWMN_Stop(&TIM_TimeBaseStructure,TIM_CHANNEL_1);
  
  /* �رն�ʱ��ͨ��2���PWM */
	HAL_TIM_PWM_Stop(&TIM_TimeBaseStructure,TIM_CHANNEL_2);
	/* �رն�ʱ��ͨ��2�������PWM */
	HAL_TIMEx_PWMN_Stop(&TIM_TimeBaseStructure,TIM_CHANNEL_2);
  
  /* �رն�ʱ��ͨ��3���PWM */
	HAL_TIM_PWM_Stop(&TIM_TimeBaseStructure,TIM_CHANNEL_3);
	/* �رն�ʱ��ͨ��3�������PWM */
	HAL_TIMEx_PWMN_Stop(&TIM_TimeBaseStructure,TIM_CHANNEL_3);
}

/**
  * @brief  ��ʼ���߼����ƶ�ʱ��
  * @param  ��
  * @retval ��
  */
void TIMx_Configuration(void)
{
	TIMx_GPIO_Config();
	TIM_Mode_Config();
}

/* ������������ض�ʱ����ʼ�� */
TIM_HandleTypeDef htimx_hall;

/**
  * ��������: ��ʱ�������������ӿ�Ӳ����ʼ������
  * �������: htim����ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú����ɿ⺯������
  */
static void hall_gpio_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  HALL_INPUT1_GPIO_CLK_ENABLE();
  HALL_INPUT2_GPIO_CLK_ENABLE();
  HALL_INPUT3_GPIO_CLK_ENABLE();
  
  /* ��ʱ��ͨ�� 1 ���ų�ʼ�� */
  GPIO_InitStruct.Pin = HALL_INPUT1_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull=GPIO_PULLUP;
  GPIO_InitStruct.Alternate = HALL_INPUT1_AF;
  HAL_GPIO_Init(HALL_INPUT1_GPIO_PORT, &GPIO_InitStruct);
  
  /* ��ʱ��ͨ�� 2 ���ų�ʼ�� */
  GPIO_InitStruct.Pin = HALL_INPUT2_PIN;
  HAL_GPIO_Init(HALL_INPUT2_GPIO_PORT, &GPIO_InitStruct);
  
  /* ��ʱ��ͨ�� 3 ���ų�ʼ�� */
  GPIO_InitStruct.Pin = HALL_INPUT3_PIN;
  HAL_GPIO_Init(HALL_INPUT3_GPIO_PORT, &GPIO_InitStruct);
}
/**
  * ��������: ��ʱ�������������ӿڳ�ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
static void hall_tim_init(void)
{
  TIM_HallSensor_InitTypeDef  hall_sensor_onfig;  
  
  /* ������ʱ������ʱ��ʹ�� */
  HALL_TIM_CLK_ENABLE();
  
  /* ��ʱ�������������� */
  htimx_hall.Instance = HALL_TIM;
  htimx_hall.Init.Prescaler = HALL_PRESCALER_COUNT;         // Ԥ��Ƶ
  htimx_hall.Init.CounterMode = TIM_COUNTERMODE_UP;         // ���ϼ���
  htimx_hall.Init.Period = HALL_PERIOD_COUNT;               // ��������
  htimx_hall.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;     // ʱ�ӷ�Ƶ
  
  hall_sensor_onfig.IC1Prescaler=TIM_ICPSC_DIV1;            // ���벶���Ƶ
  hall_sensor_onfig.IC1Polarity=TIM_ICPOLARITY_BOTHEDGE;    // ���벶����
  hall_sensor_onfig.IC1Filter= 10;                          // �����˲�
  hall_sensor_onfig.Commutation_Delay = 0U;                 // ��ʹ���ӳٴ���
  HAL_TIMEx_HallSensor_Init(&htimx_hall,&hall_sensor_onfig);
  
  HAL_NVIC_SetPriority(HALL_TIM_IRQn, 0, 0);    // �����ж����ȼ�
  HAL_NVIC_EnableIRQ(HALL_TIM_IRQn);            // ʹ���ж�
}

/**
  * @brief  ��ʼ��������������ʱ��
  * @param  ��
  * @retval ��
  */
void hall_tim_config(void)
{
	hall_gpio_init();	    // ��ʼ������
	hall_tim_init();      // ��ʼ����ʱ��
}

/*********************************************END OF FILE**********************/
