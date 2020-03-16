/**
  ******************************************************************************
  * @file    bsp_debug_usart.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   ʹ�ô���1���ض���c��printf������usart�˿ڣ��жϽ���ģʽ
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32 F407 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./usart/bsp_debug_usart.h"

UART_HandleTypeDef UartHandle;
//extern uint8_t ucTemp;  

 /**
  * @brief  DEBUG_USART GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
  */  
void DEBUG_USART_Config(void)
{ 
  
  UartHandle.Instance          = DEBUG_USART;
  
  UartHandle.Init.BaudRate     = DEBUG_USART_BAUDRATE;
  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits     = UART_STOPBITS_1;
  UartHandle.Init.Parity       = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode         = UART_MODE_TX_RX;
  
  HAL_UART_Init(&UartHandle);
    
 /*ʹ�ܴ��ڽ��ն� */
  __HAL_UART_ENABLE_IT(&UartHandle,UART_IT_RXNE);  
}


/**
  * @brief UART MSP ��ʼ�� 
  * @param huart: UART handle
  * @retval ��
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{  
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  DEBUG_USART_CLK_ENABLE();
	
	DEBUG_USART_RX_GPIO_CLK_ENABLE();
  DEBUG_USART_TX_GPIO_CLK_ENABLE();
  
/**USART1 GPIO Configuration    
  PA9     ------> USART1_TX
  PA10    ------> USART1_RX 
  */
  /* ����Tx����Ϊ���ù���  */
  GPIO_InitStruct.Pin = DEBUG_USART_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = DEBUG_USART_TX_AF;
  HAL_GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStruct);
  
  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStruct.Pin = DEBUG_USART_RX_PIN;
  GPIO_InitStruct.Alternate = DEBUG_USART_RX_AF;
  HAL_GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStruct); 
 
  HAL_NVIC_SetPriority(DEBUG_USART_IRQ ,0,1);	//��ռ���ȼ�0�������ȼ�1
  HAL_NVIC_EnableIRQ(DEBUG_USART_IRQ );		    //ʹ��USART1�ж�ͨ��  
}


/*****************  �����ַ��� **********************/
void Usart_SendString(uint8_t *str)
{
	unsigned int k=0;
  do 
  {
      HAL_UART_Transmit(&UartHandle,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');
  
}
///�ض���c�⺯��printf������DEBUG_USART���ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ�����DEBUG_USART */
	HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 1000);	
	
	return (ch);
}

///�ض���c�⺯��scanf������DEBUG_USART����д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		
	int ch;
	HAL_UART_Receive(&UartHandle, (uint8_t *)&ch, 1, 1000);	
	return (ch);
}

/* ���ݸ�ʽ��ת�� */
typedef union {
  char Ch[4];
  float Float;
  int32_t Int;
}Format_UnionTypedef;

typedef struct {
  __IO uint8_t  Code ;  	
  __IO Format_UnionTypedef data[3];//����֡��3������
}MSG_TypeDef;

// Э����ض���
#define FRAME_LENTH               16    // ָ���
#define FRAME_START               0xAA  // Э��֡��ʼ
#define FRAME_END                 '/'   // Э��֡����
#define FRAME_CHECK_BEGIN          1    // У���뿪ʼ��λ�� RxBuf[1]
#define FRAME_CHECKSUM            14    // У�����λ��   RxBuf[14]
#define FRAME_CHECK_NUM           13    // ��ҪУ����ֽ���
#define FILL_VALUE                0x55  // ���ֵ
#define CODE_SETPID               0x07  // ����PID����
#define CODE_SETTGT               0x08  // ����Ŀ��ֵ
#define CODE_RESET                0x09   // ��λ����
#define CODE_STARTMOTOR           0x0A   // �������

__IO uint8_t RxBuf[FRAME_LENTH] ; // ���ջ�����
__IO uint8_t TxBuf[FRAME_LENTH] ; // ���ͻ�����
/* ��չ���� ------------------------------------------------------------------*/
MSG_TypeDef Msg;

uint8_t check_sum(uint8_t *ptr,uint8_t num )
{
  uint8_t Sum = 0;
  
  while(num--)
  {
    Sum += *ptr;
    ptr++;
  }
  
  return Sum;
}

/**
  * ��������: ���ڽ����жϻص�����
  * �������: huart�����ھ��ָ��
  * �� �� ֵ: ��
  * ˵    ��: ���̶��ֳ���������֡,ͬʱ��֤֡ͷ֡β��У����
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
//  if(huart == &UartHandle)
//  {
//    if(RxBuf[0] != FRAME_START )    // ֡ͷ��ȷ
//    {
//      return;
//    }
//    if(RxBuf[FRAME_LENTH-1] == FRAME_END ) // ֡β��ȷ
//    { 
//      /* �ж�У���� */
//      if(CheckSum((uint8_t*)&RxBuf[FRAME_CHECK_BEGIN],FRAME_CHECK_NUM) != RxBuf[FRAME_CHECKSUM] )
//      {
//        Msg.Code = NULL;
//        return;
//      }
//      else
//      {
//        /* ��������֡ */
////        ptr_Fun_();
//      }
//    }
//    HAL_UART_Receive_IT(huart,(uint8_t *)&RxBuf,FRAME_LENTH); // ����ʹ�ܽ����ж�
//  }
}

/**
  * @brief ������λ����ֵ 
  * @param cmd������
  * @param ch: ����ͨ��
  * @param data������
  * @retval ��
  */
void set_computer_value(uint8_t cmd, uint8_t ch, int32_t data)
{
  packet_head_t set_packet =
  {  
     /* ��λ��Ҫ���λ��ǰ */
    .head = 0x535A4859,     // ��ͷ
    .len  = 0x0F0000000,    // ������
  };
  
  set_packet.ch = ch;      // ����ͨ��
  set_packet.cmd = cmd;    // ��������
  set_packet.data = EXCHANGE_H_L_BIT(data);    // ��������
  
  set_packet.sum = check_sum((uint8_t *)&set_packet, sizeof(set_packet) - 1);       // ����У���
  
  HAL_UART_Transmit_IT(&UartHandle, (uint8_t *)&set_packet, sizeof(set_packet));    // ��������֡
}

/*********************************************END OF FILE**********************/
