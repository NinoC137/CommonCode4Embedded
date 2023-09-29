#ifndef __PID_H
#define __PID_H

#include <stdio.h>

typedef struct PIDERROR
{
	//��ǰƫ��ֵ, �ϴ�ƫ��ֵ, �ۼ�ƫ��ֵ, ���ϴ�ƫ��ֵ
    float err;
    float err_last;
    float err_sum;
    float err_sec_last;
}PID_Error;

typedef struct PIDarguments
{
    //Ŀ��ֵ��ʵ��ֵ
    float target_val;
    float actual_val;

	PID_Error PID_Error;

    //����, ����, ΢��ϵ��  
    float Kp;
    float Ki;
    float Kd;

    //�������ʵ��
    void (*P_set)(struct PIDarguments *pid,float aKp);
    void (*I_set)(struct PIDarguments *pid,float aKi);
    void (*D_set)(struct PIDarguments *pid,float aKd);
    void (*Target_Set)(struct PIDarguments *pid,float aTarget);

}tPID;

//���ܺ���
tPID PID_Init(void);
void Target_Set(tPID *pid, float target);
void P_Set(struct PIDarguments *pid, float p);
void I_Set(struct PIDarguments *pid, float i);
void D_Set(struct PIDarguments *pid, float d);
void Err_Calculate(tPID *pid);

//λ��ʽPID����
float PD_realize(tPID *pid, float actual_Val);
float PI_realize(tPID *pid, float actual_Val);
float PID_realize(tPID *pid, float actual_Val);

//����ʽPID����
float PI_realize_Incremental(tPID *pid, float target_Val);

extern tPID pid_Motor1_Speed;
extern tPID pid_Motor2_Speed;
#endif


