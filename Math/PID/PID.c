/*****************************************
�ļ���: PID.c
����: ����ʩ
�汾:   2.0
����:   2023.5.12
����:
    ���ļ���ʵ����λ��ʽPID�ļ���(������������˼��)
    !���齫PID���ƺ������ڶ�ʱ���жϻص�������ִ��,�Ա�֤��������(�Ƽ�2~20ms,����ʵ���������)
    ʹ�÷���:  �ٽ��г�ʼ���������
              �ڽ�PID������������    (ͨ���������������ֻ����һ�εĳ�ʼ����������)
              �۸�������ѡȡ���㺯��
*****************************************/

#include "PID.h"
#include "Filter.h"

#define PWM_OUT_MAX	7200	//PID����޷�
#define ERR_SUM_MAX 2000

// ����һ���ṹ�����ͱ�����ȫ�ֱ�����    �˴�����Ϊpid_Motor1_Speed(����Ϊʾ��ʹ��, ����ʵ������)
tPID pid_Motor1_Speed;
tPID pid_Motor2_Speed;

// ��ʼ��PID�ṹ��
tPID PID_Init()
{
    tPID pid;
    pid.target_val = 0.00f;
    pid.actual_val = 0.00f;

    pid.PID_Error.err = 0.0f;
    pid.PID_Error.err_last = 0.0f;
    pid.PID_Error.err_sum = 0.0f;

    pid.Kp = 0.0f;
    pid.Ki = 0.0f;
    pid.Kd = 0.0f;

    pid.P_set = P_Set;
    pid.I_set = I_Set;
    pid.D_set = D_Set;
    pid.Target_Set = Target_Set;

    return pid;
}


/**********************************************************************
    ��������:   PID�������ʽ����ʵ��
    �������:
                1.PID�ṹ��
                2.Ŀ��ֵ
                3.PID����ϵ��
    ����ֵ:     ��
    ����:   ����ʩ
    �޸�ʱ��:   2023.2.14
**********************************************************************/
void Target_Set(tPID *pid, float target)    { pid->target_val = target; }
void P_Set(tPID *pid, float p)  { pid->Kp = p; }
void I_Set(tPID *pid, float i)  { pid->Ki = i; }
void D_Set(tPID *pid, float d)  { pid->Kd = d; }
//����һ���ͺ��˲���������(slope�˲���)
void Err_Calculate(tPID *pid){
    float Error_temp;   //����һ���ֲ�������,���������˲��㷨

    Error_temp = pid->actual_val - pid->target_val;

    pid->PID_Error.err = FirstOrderLagFilter_Slope(Error_temp);

}


/**********************************************************************
    ��������:   ʹ��PIDϵ������PID����(λ��ʽPosition)
    �������:
                1.PID�ṹ��
                2.��ǰ��ʵֵ
    ����ֵ:
                Kp*��ǰ��� + Ki*������ֵ + Kd*����������ϴ����֮��
    ע��:
        �ٽ����˻�����(Ki)���޷�,�������ֵ��������,���³����Գ���
        �ڽ�����PID����ֵ(����ٶ�ΪPWM)���޷�
    ����:   ����ʩ
    �޸�ʱ��:   2023.5.13
**********************************************************************/
float PID_realize(tPID *pid, float actual_Val)
{
    pid->actual_val = actual_Val;                 // ���ݵ�ǰ��ʵֵ
    Err_Calculate(pid); // ����ó���ǰ���(�ڲ�������slope�˲�ģ�͵�һ���ͺ��˲��㷨)
    pid->PID_Error.err_sum += pid->PID_Error.err;                     // �ۼ����ֵ��ȡ

    //�������޷�
    if(pid->PID_Error.err_sum >=  ERR_SUM_MAX)  { pid->PID_Error.err_sum =  ERR_SUM_MAX; }
    if(pid->PID_Error.err_sum <= -ERR_SUM_MAX)  { pid->PID_Error.err_sum = -ERR_SUM_MAX; }

    // PID����ʵ�ֲ���
    pid->actual_val = pid->Kp * pid->PID_Error.err 
						+ pid->Ki * pid->PID_Error.err_sum 
						+ pid->Kd * (pid->PID_Error.err - pid->PID_Error.err_last);

    pid->PID_Error.err_last = pid->PID_Error.err; // ����ǰ�������ϴ����֮��, ʵ�ֺ�������
	
	//PID����޷�
	if(pid->actual_val >= PWM_OUT_MAX)
	{
		pid->actual_val = PWM_OUT_MAX;
	}
	if(pid->actual_val <= -PWM_OUT_MAX)
	{
		pid->actual_val = -PWM_OUT_MAX;
	}

    return pid->actual_val;
}

/**********************************************************************
    ��������:   ʹ��PIϵ������PID����(����ʽ)
    �������:
                1.PID�ṹ��
                2.Ŀ��ֵ
    ����ֵ:
        pwm += Kp[e��k��- e(k-1)] + Ki*e(k) + Kd*[e(k)-2e(k-1)+e(k-2)]
        e(k)������ƫ��   e(k-1)������һ�ε�ƫ��  �Դ����� 
        pwm�����������
    ע��:
        �ڽ�����PID����ֵ(����ٶ�ΪPWM)���޷�
    ����:   ����ʩ
    �޸�ʱ��:   2023.5.13
**********************************************************************/
float PI_realize_Incremental(tPID *pid, float target_Val)
{
    Target_Set(pid,target_Val); //�趨Ŀ��ֵ
    Err_Calculate(pid);   // ����ó���ǰ���(�ڲ�������slope�˲�ģ�͵�һ���ͺ��˲��㷨)

    //PI����ʵ�ֲ���
    pid->actual_val += pid->Kp * (pid->PID_Error.err - pid->PID_Error.err_last) 
						+ pid->Ki * pid->PID_Error.err
                        + pid->Kd * (pid->PID_Error.err - 2*pid->PID_Error.err_last + pid->PID_Error.err_sec_last);

    pid->PID_Error.err_sec_last = pid->PID_Error.err_last;
    pid->PID_Error.err_last = pid->PID_Error.err;   //����ǰ�������ϴ������, ʵ�ֺ�������

	//PID����޷�
	if(pid->actual_val >= PWM_OUT_MAX)
	{
		pid->actual_val = PWM_OUT_MAX;
	}
	if(pid->actual_val <= -PWM_OUT_MAX)
	{
		pid->actual_val = -PWM_OUT_MAX;
	}

    return pid->actual_val;
}


