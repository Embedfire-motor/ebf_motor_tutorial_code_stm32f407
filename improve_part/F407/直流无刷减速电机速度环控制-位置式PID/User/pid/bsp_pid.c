#include "./pid/bsp_pid.h"

//����ȫ�ֱ���

_pid pid;

/**
  * @brief  PID������ʼ��
	*	@note 	��
  * @retval ��
  */
void PID_param_init()
{
		/* ��ʼ������ */
    printf("PID_init begin \n");
    pid.target_val=0.0;				
    pid.actual_val=0.0;
    pid.err=0.0;
    pid.err_last=0.0;
    pid.integral=0.0;
//		pid.Kp = 0.31;
//		pid.Ki = 0.070;
//		pid.Kd = 0.3;


		pid.Kp = 0.01;//24
		pid.Ki = 0.80;
		pid.Kd = 0.04;
	
    printf("PID_init end \n");
}

/**
  * @brief  ����Ŀ��ֵ
  * @param  val		Ŀ��ֵ
	*	@note 	��
  * @retval ��
  */
void set_pid_actual(float temp_val)
{
  pid.target_val = temp_val;    // ���õ�ǰ��Ŀ��ֵ
}

/**
  * @brief  PID�㷨ʵ��
  * @param  actual_val:ʵ��ֵ
	*	@note 	��
  * @retval ͨ��PID���������
  */
float PID_realize(float actual_val)
{
		/*����Ŀ��ֵ��ʵ��ֵ�����*/
    pid.err=pid.target_val-actual_val;
		/*����ۻ�*/
    pid.integral+=pid.err;
		/*PID�㷨ʵ��*/
    pid.actual_val=pid.Kp*pid.err+pid.Ki*pid.integral+pid.Kd*(pid.err-pid.err_last);
		/*����*/
    pid.err_last=pid.err;
		/*���ص�ǰʵ��ֵ*/
    return pid.actual_val;
}




