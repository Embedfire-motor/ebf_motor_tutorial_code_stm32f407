#ifndef __BSP_BLDCM_CONTROL_H
#define	__BSP_BLDCM_CONTROL_H

#include "stm32f4xx.h"
#include "./tim/bsp_motor_tim.h"
#include "main.h"

/* ����������ö�� */
typedef enum
{
  MOTOR_FWD = 0,
  MOTOR_REV,
}motor_dir_t;

typedef struct
{
  motor_dir_t direction;    // �������
  uint16_t dutyfactor;      // PWM ���ռ�ձ�
  uint8_t is_enable;        // ʹ�ܵ��
}bldcm_data_t;


#define PID_ASSISTANT_EN    1    // 1:ʹ��PID����������ʾ���Σ�0��ʹ�ô���ֱ�Ӵ�ӡ����

void bldcm_init(void);
void set_bldcm_speed(uint16_t v);
void set_bldcm_direction(motor_dir_t dir);
motor_dir_t get_bldcm_direction(void);
void set_bldcm_enable(void);
void set_bldcm_disable(void);
void bldcm_pid_control(void);

#endif /* __BSP_BLDCM_CONTROL_H */

