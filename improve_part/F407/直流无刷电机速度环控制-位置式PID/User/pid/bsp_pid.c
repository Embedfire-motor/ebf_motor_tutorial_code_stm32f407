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
    pid.target_val=500.0;				
    pid.actual_val=0.0;
    pid.err=0.0;
    pid.err_last=0.0;
    pid.integral=0.0;

		pid.Kp = 0.4;//24
		pid.Ki = 0.03;
		pid.Kd = 0.0;
	
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
  * @param  actual_val:ʵ��ֵ
	*	@note 	��
  * @retval ͨ��PID���������
  */
float PID_realize(float actual_val)
{
		/*����Ŀ��ֵ��ʵ��ֵ�����*/
    pid.err=pid.target_val-actual_val;
    pid.integral+=pid.err;
		/*PID�㷨ʵ��*/
    pid.actual_val=pid.Kp*pid.err+pid.Ki*pid.integral+pid.Kd*(pid.err-pid.err_last);
		/*����*/
    pid.err_last=pid.err;
		/*���ص�ǰʵ��ֵ*/
    return pid.actual_val;
}




