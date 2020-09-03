/**
  ******************************************************************************
  * @file    bsp_stepper_init.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   ���������ʼ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
#include "./stepper/bsp_stepper_init.h"

TIM_HandleTypeDef TIM_StepperHandle = {0};

/**
  * @brief  ����TIM�������PWMʱ�õ���I/O
  * @param  ��
  * @retval ��
  */
static void Stepper_GPIO_Config(void) 
{
	GPIO_InitTypeDef GPIO_InitStruct;
	/*����Motor��ص�GPIO����ʱ��*/
	MOTOR_DIR_GPIO_CLK_ENABLE();
	MOTOR_PUL_GPIO_CLK_ENABLE();
	MOTOR_EN_GPIO_CLK_ENABLE();

	/*ѡ��Ҫ���Ƶ�GPIO����*/															   
	GPIO_InitStruct.Pin = MOTOR_DIR_PIN;	
	/*�������ŵ��������Ϊ�������*/
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;  
	GPIO_InitStruct.Pull =GPIO_NOPULL;
	/*������������Ϊ���� */   
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	/*Motor �������� ��ʼ��*/
	HAL_GPIO_Init(MOTOR_DIR_GPIO_PORT, &GPIO_InitStruct);	
  HAL_GPIO_WritePin(MOTOR_DIR_GPIO_PORT, MOTOR_DIR_PIN, GPIO_PIN_SET);
	/*Motor ʹ������ ��ʼ��*/
	GPIO_InitStruct.Pin = MOTOR_EN_PIN;	
	HAL_GPIO_Init(MOTOR_EN_GPIO_PORT, &GPIO_InitStruct);	

	/*ѡ��Ҫ���Ƶ�GPIO����*/	
	GPIO_InitStruct.Pin = MOTOR_PUL_PIN;
	/*Motor �������� ��ʼ��*/
	HAL_GPIO_Init(MOTOR_PUL_GPIO_PORT, &GPIO_InitStruct);			
  HAL_GPIO_WritePin(MOTOR_PUL_GPIO_PORT, MOTOR_PUL_PIN, GPIO_PIN_RESET);
}

/**
  * @brief  ���ò��������TIM
  * @param  ��
  * @retval ��
  */
static void Stepper_TIM_Init(void)
{
  MOTOR_TIM_CLK_ENABLE();
  
  TIM_StepperHandle.Instance = MOTOR_TIM;
  TIM_StepperHandle.Init.Prescaler = MOTOR_TIM_PRESCALER - 1;
  TIM_StepperHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  TIM_StepperHandle.Init.Period = MOTOR_TIM_PERIOD;
  TIM_StepperHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&TIM_StepperHandle);

  /* �ر�û���õ����ж� */
  __HAL_TIM_DISABLE_IT(&TIM_StepperHandle,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4|
                                          TIM_IT_COM|TIM_IT_TRIGGER|TIM_IT_BREAK);
  /* �����жϱ�־λ */
  __HAL_TIM_CLEAR_IT(&TIM_StepperHandle,TIM_IT_UPDATE);
  /* ʹ�ܶ�ʱ���ĸ����¼��ж� */
  __HAL_TIM_ENABLE_IT(&TIM_StepperHandle,TIM_IT_UPDATE);
  /* ���ø����¼�����ԴΪ����������� */
  __HAL_TIM_URS_ENABLE(&TIM_StepperHandle);
  
  /* �رն�ʱ�� */
  HAL_TIM_Base_Stop_IT(&TIM_StepperHandle);
}

/**
  * @brief  �ж����ȼ�����
  * @param  ��
  * @retval ��
  */
static void Stepper_NVIC_Init(void)
{
  /* �����ж����� */
  HAL_NVIC_SetPriority(MOTOR_TIM_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(MOTOR_TIM_IRQn);
}

/**
  * @brief  ���������ʼ��
  * @retval ��
  */
void stepper_Init()
{
	/* ���IO���� */
	Stepper_GPIO_Config();
  /* �����ʱ����ʼ�� */
  Stepper_TIM_Init();
  /* �ж����ȼ����� */
  Stepper_NVIC_Init();
}

