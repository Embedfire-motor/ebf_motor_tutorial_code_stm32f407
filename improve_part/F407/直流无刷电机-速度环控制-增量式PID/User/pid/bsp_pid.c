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
    pid.target_val=500;				
    pid.actual_val=0.0;
		pid.err = 0.0;
		pid.err_last = 0.0;
		pid.err_next = 0.0;
		
		pid.Kp = 0.30;
		pid.Ki = 0.08;
		pid.Kd = 0.01;
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
  * @brief  ��ȡĿ��ֵ
  * @param  ��
	*	@note 	��
  * @retval Ŀ��ֵ
  */
float get_pid_actual(void)
{
  return pid.target_val;    // ���õ�ǰ��Ŀ��ֵ
}

/**
  * @brief  ���ñ��������֡�΢��ϵ��
  * @param  p������ϵ�� P
  * @param  i������ϵ�� i
  * @param  d��΢��ϵ�� d
	*	@note 	��
  * @retval ��
  */
void set_p_i_d(float p, float i, float d)
{
  	pid.Kp = p;    // ���ñ���ϵ�� P
		pid.Ki = i;    // ���û���ϵ�� I
		pid.Kd = d;    // ����΢��ϵ�� D
}

/**
  * @brief  PID�㷨ʵ��
  * @param  val		Ŀ��ֵ
	*	@note 	��
  * @retval ͨ��PID���������
  */
float PID_realize(float temp_val) 
{
	/*����Ŀ��ֵ*/
	pid.actual_val = temp_val;
	/*����Ŀ��ֵ��ʵ��ֵ�����*/
  pid.err=pid.target_val-pid.actual_val;
	/*PID�㷨ʵ��*/
	pid.actual_val = pid.Kp*(pid.err - pid.err_next) 
                 + pid.Ki*pid.err 
                 + pid.Kd*(pid.err - 2 * pid.err_next + pid.err_last);
	/*�������*/
	pid.err_last = pid.err_next;
	pid.err_next = pid.err;
	/*���ص�ǰʵ��ֵ*/
	return pid.actual_val;
}




