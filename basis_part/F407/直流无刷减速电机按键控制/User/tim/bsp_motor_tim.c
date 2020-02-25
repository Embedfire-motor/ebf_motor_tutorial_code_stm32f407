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
#include "./led/bsp_led.h"
#include "./usart/bsp_debug_usart.h"

TIM_HandleTypeDef  htimx_bldcm;
TIM_OC_InitTypeDef TIM_OCInitStructure;

/* ������������ض�ʱ����ʼ�� */
TIM_HandleTypeDef htimx_hall;

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
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;   // �������ģʽ
  
  GPIO_InitStructure.Pin = ADVANCE_OCNPWM1_PIN;
	HAL_GPIO_Init(ADVANCE_OCNPWM1_GPIO_PORT, &GPIO_InitStructure);	

  GPIO_InitStructure.Pin = ADVANCE_OCNPWM2_PIN;	
	HAL_GPIO_Init(ADVANCE_OCNPWM2_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = ADVANCE_OCNPWM3_PIN;	
	HAL_GPIO_Init(ADVANCE_OCNPWM3_GPIO_PORT, &GPIO_InitStructure);	
  
  /* ͨ�� 2 */
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;  
  
  GPIO_InitStructure.Pin = ADVANCE_OCPWM1_PIN;
  GPIO_InitStructure.Alternate = ADVANCE_OCPWM1_AF;	
	HAL_GPIO_Init(ADVANCE_OCPWM1_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.Pin = ADVANCE_OCPWM2_PIN;	
	GPIO_InitStructure.Alternate = ADVANCE_OCPWM2_AF;	
	HAL_GPIO_Init(ADVANCE_OCPWM2_GPIO_PORT, &GPIO_InitStructure);
  
  /* ͨ�� 3 */
  GPIO_InitStructure.Pin = ADVANCE_OCPWM3_PIN;	
	GPIO_InitStructure.Alternate = ADVANCE_OCPWM3_AF;	
	HAL_GPIO_Init(ADVANCE_OCPWM3_GPIO_PORT, &GPIO_InitStructure);
  
  
	
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
	htimx_bldcm.Instance = ADVANCE_TIM;
	/* �ۼ� TIM_Period�������һ�����»����ж�*/		
	//����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
	htimx_bldcm.Init.Period = PWM_PERIOD_COUNT;
	// �߼����ƶ�ʱ��ʱ��ԴTIMxCLK = HCLK=216MHz 
	// �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=1MHz
	htimx_bldcm.Init.Prescaler = PWM_PRESCALER_COUNT;	
	// ����ʱ�ӷ�Ƶ
	htimx_bldcm.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	// ������ʽ
	htimx_bldcm.Init.CounterMode=TIM_COUNTERMODE_UP;
	// �ظ�������
	htimx_bldcm.Init.RepetitionCounter=0;	
	// ��ʼ����ʱ��TIMx, x[1,8]
	HAL_TIM_PWM_Init(&htimx_bldcm);

	/*PWMģʽ����*/
	//����ΪPWMģʽ1
	TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM1;
	TIM_OCInitStructure.Pulse = 0;
	TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;
	TIM_OCInitStructure.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	TIM_OCInitStructure.OCIdleState = TIM_OCIDLESTATE_SET;
	TIM_OCInitStructure.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	
	HAL_TIM_PWM_ConfigChannel(&htimx_bldcm,&TIM_OCInitStructure,TIM_CHANNEL_1);    // ��ʼ��ͨ�� 1 ��� PWM 
  HAL_TIM_PWM_ConfigChannel(&htimx_bldcm,&TIM_OCInitStructure,TIM_CHANNEL_2);    // ��ʼ��ͨ�� 2 ��� PWM
  HAL_TIM_PWM_ConfigChannel(&htimx_bldcm,&TIM_OCInitStructure,TIM_CHANNEL_3);    // ��ʼ��ͨ�� 3 ��� PWM

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
	HAL_TIM_PWM_Stop(&htimx_bldcm,TIM_CHANNEL_1);
	/* �رն�ʱ��ͨ��1�������PWM */
	HAL_TIMEx_PWMN_Stop(&htimx_bldcm,TIM_CHANNEL_1);
  
  /* �رն�ʱ��ͨ��2���PWM */
	HAL_TIM_PWM_Stop(&htimx_bldcm,TIM_CHANNEL_2);
	/* �رն�ʱ��ͨ��2�������PWM */
	HAL_TIMEx_PWMN_Stop(&htimx_bldcm,TIM_CHANNEL_2);
  
  /* �رն�ʱ��ͨ��3���PWM */
	HAL_TIM_PWM_Stop(&htimx_bldcm,TIM_CHANNEL_3);
	/* �رն�ʱ��ͨ��3�������PWM */
	HAL_TIMEx_PWMN_Stop(&htimx_bldcm,TIM_CHANNEL_3);
}

/**
  * @brief  ֹͣpwm���
  * @param  ��
  * @retval ��
  */
void stop_pwm_output(void)
{
  /* �رն�ʱ��ͨ��1���PWM */
	HAL_TIM_PWM_Stop(&htimx_bldcm,TIM_CHANNEL_1);

  /* �رն�ʱ��ͨ��2���PWM */
	HAL_TIM_PWM_Stop(&htimx_bldcm,TIM_CHANNEL_2);
  
  /* �رն�ʱ��ͨ��3���PWM */
	HAL_TIM_PWM_Stop(&htimx_bldcm,TIM_CHANNEL_3);
  
  HAL_GPIO_WritePin(ADVANCE_OCNPWM1_GPIO_PORT, ADVANCE_OCNPWM1_PIN, GPIO_PIN_RESET);    // �ر����ű�
  HAL_GPIO_WritePin(ADVANCE_OCNPWM2_GPIO_PORT, ADVANCE_OCNPWM2_PIN, GPIO_PIN_RESET);    // �ر����ű�
  HAL_GPIO_WritePin(ADVANCE_OCNPWM3_GPIO_PORT, ADVANCE_OCNPWM3_PIN, GPIO_PIN_RESET);    // �ر����ű�
}

/**
  * @brief  ����pwm�����ռ�ձ�
  * @param  pulse:Ҫ���õ�ռ�ձ�
  * @retval ��
  */
void set_pwm_pulse(uint16_t pulse)
{
  /* ���ö�ʱ��ͨ����� PWM ��ռ�ձ� */
	__HAL_TIM_SET_COMPARE(&htimx_bldcm,TIM_CHANNEL_1,pulse);
  __HAL_TIM_SET_COMPARE(&htimx_bldcm,TIM_CHANNEL_2,pulse);
  __HAL_TIM_SET_COMPARE(&htimx_bldcm,TIM_CHANNEL_3,pulse);
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
  * @brief  ʹ�ܻ���������
  * @param  ��
  * @retval ��
  */
void hall_enable(void)
{
  /* ʹ�ܻ����������ӿ� */
  __HAL_TIM_ENABLE_IT(&htimx_hall, TIM_IT_TRIGGER);
  __HAL_TIM_ENABLE_IT(&htimx_hall, TIM_IT_UPDATE);
  
  HAL_TIMEx_HallSensor_Start(&htimx_hall);

  LED1_OFF;
  
  HAL_TIM_TriggerCallback(&htimx_hall);   // ִ��һ�λ���
}

/**
  * @brief  ���û���������
  * @param  ��
  * @retval ��
  */
void hall_disable(void)
{
  /* ���û����������ӿ� */
  __HAL_TIM_DISABLE_IT(&htimx_hall, TIM_IT_TRIGGER);
  __HAL_TIM_DISABLE_IT(&htimx_hall, TIM_IT_UPDATE);
  HAL_TIMEx_HallSensor_Stop(&htimx_hall);
}

uint8_t get_hall_state(void)
{
  uint8_t state = 0;
  
  /* ��ȡ���������� 1 ��״̬ */
  if(HAL_GPIO_ReadPin(HALL_INPUT1_GPIO_PORT,HALL_INPUT1_PIN) != GPIO_PIN_RESET)
  {
    state |= 0x01U;
  }
  
  /* ��ȡ���������� 2 ��״̬ */
  if(HAL_GPIO_ReadPin(HALL_INPUT2_GPIO_PORT,HALL_INPUT2_PIN) != GPIO_PIN_RESET)  //����������״̬��ȡ
  {
    state |= 0x02U;
  }
  
  /* ��ȡ���������� 3 ��״̬ */
  if(HAL_GPIO_ReadPin(HALL_INPUT3_GPIO_PORT,HALL_INPUT3_PIN) != GPIO_PIN_RESET)  //����������״̬��ȡ
  {
    state |= 0x04U;
  }
  
  return state;    // ���ش�����״̬
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

int update = 0;     // ��ʱ�����¼���

/**
  * @brief  ���������������ص�����
  * @param  htim:��ʱ�����
  * @retval ��
  */
void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim)
{
  /* ��ȡ��������������״̬,��Ϊ��������� */
  uint8_t step = 0;
  step = get_hall_state();

  if(get_bldcm_direction() == MOTOR_FWD)
  {
    step = (uint32_t)7 - step;        // ����˳���Ĺ��� CW = 7 - CCW;
  }
  /*---- ���嶨ʱ��OC1ΪA(U)�� OC2ΪB(V)�࣬OC3ΪC(W)�� ---- */
  /*---- ����uWStep��3λΪ��������������״̬,IC1(001,U),IC2(010,V),IC3(100,W) ----*/
  switch(step)
  {
    case 1://C+ A-
      /*  Channe2 configuration  */ 
      HAL_TIM_PWM_Stop(&htimx_bldcm, TIM_CHANNEL_2);    // ֹͣPWM���
      HAL_GPIO_WritePin(ADVANCE_OCNPWM2_GPIO_PORT, ADVANCE_OCNPWM2_PIN, GPIO_PIN_RESET);    // �ر����ű�
    
      /*  Channe3 configuration */
      HAL_TIM_PWM_Start(&htimx_bldcm, TIM_CHANNEL_3);    // ��ʼPWM���

      /*  Channel configuration  */
      HAL_GPIO_WritePin(ADVANCE_OCNPWM1_GPIO_PORT, ADVANCE_OCNPWM1_PIN, GPIO_PIN_SET);    // �������ű�
      break;
    
    case 2: //A+  B-
      /*  Channe3 configuration */ 
      HAL_TIM_PWM_Stop(&htimx_bldcm, TIM_CHANNEL_3);
      HAL_GPIO_WritePin(ADVANCE_OCNPWM3_GPIO_PORT, ADVANCE_OCNPWM3_PIN, GPIO_PIN_RESET);
    
      /*  Channel configuration  */
      HAL_TIM_PWM_Start(&htimx_bldcm, TIM_CHANNEL_1);
      
      /*  Channe2 configuration */
      HAL_GPIO_WritePin(ADVANCE_OCNPWM2_GPIO_PORT, ADVANCE_OCNPWM2_PIN, GPIO_PIN_SET);
      break;
    
    case 3:// C+ B-
      /*  Channel configuration */ 
      HAL_TIM_PWM_Stop(&htimx_bldcm, TIM_CHANNEL_1);
      HAL_GPIO_WritePin(ADVANCE_OCNPWM1_GPIO_PORT, ADVANCE_OCNPWM1_PIN, GPIO_PIN_RESET);
 
      /*  Channe3 configuration  */
      HAL_TIM_PWM_Start(&htimx_bldcm, TIM_CHANNEL_3);
  
      /*  Channe2 configuration  */
      HAL_GPIO_WritePin(ADVANCE_OCNPWM2_GPIO_PORT, ADVANCE_OCNPWM2_PIN, GPIO_PIN_SET);
      break;
    
    case 4:// B+ C-
      /*  Channel configuration */ 
      HAL_TIM_PWM_Stop(&htimx_bldcm, TIM_CHANNEL_1);
      HAL_GPIO_WritePin(ADVANCE_OCNPWM1_GPIO_PORT, ADVANCE_OCNPWM1_PIN, GPIO_PIN_RESET);

      /*  Channe2 configuration */
      HAL_TIM_PWM_Start(&htimx_bldcm, TIM_CHANNEL_2);
      
      /*  Channe3 configuration */
      HAL_GPIO_WritePin(ADVANCE_OCNPWM3_GPIO_PORT, ADVANCE_OCNPWM3_PIN, GPIO_PIN_SET);    
      break;
    
    case 5: // B+ A-
      /*  Channe3 configuration */       
      HAL_TIM_PWM_Stop(&htimx_bldcm, TIM_CHANNEL_3);
      HAL_GPIO_WritePin(ADVANCE_OCNPWM3_GPIO_PORT, ADVANCE_OCNPWM3_PIN, GPIO_PIN_RESET);
    
      /*  Channe2 configuration */
      HAL_TIM_PWM_Start(&htimx_bldcm, TIM_CHANNEL_2);
      
      /*  Channel configuration */
      HAL_GPIO_WritePin(ADVANCE_OCNPWM1_GPIO_PORT, ADVANCE_OCNPWM1_PIN, GPIO_PIN_SET);
      break;
    
    case 6: // A+ C-
      /*  Channe2 configuration */ 
      HAL_TIM_PWM_Stop(&htimx_bldcm, TIM_CHANNEL_2);
      HAL_GPIO_WritePin(ADVANCE_OCNPWM2_GPIO_PORT, ADVANCE_OCNPWM2_PIN, GPIO_PIN_RESET);
    
      /*  Channel configuration */
      HAL_TIM_PWM_Start(&htimx_bldcm, TIM_CHANNEL_1); 
      
      /*  Channe3 configuration */
      HAL_GPIO_WritePin(ADVANCE_OCNPWM3_GPIO_PORT, ADVANCE_OCNPWM3_PIN, GPIO_PIN_SET);
      break;
  }

  update = 0;
}

/**
  * @brief  ��ʱ�������жϻص�����
  * @param  htim:��ʱ�����
  * @retval ��
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (update++ > 1)    // ��һ���ڲ��������ж�ǰ����������û�в���ֵ
  {
    printf("��ת��ʱ\r\n");
    update = 0;
    
    LED1_ON;     // ����LED1��ʾ��ת��ʱֹͣ
    
    /* ��ת��ʱֹͣ PWM ��� */
    hall_disable();       // ���û����������ӿ�
    stop_pwm_output();    // ֹͣ PWM ���
  }
}

/*********************************************END OF FILE**********************/
