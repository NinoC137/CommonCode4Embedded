/*
 * Filter.c
 *
 *  Created on: 2022��3��18��
 *      Author: yue
 */

#include "Filter.h"

/*
 *******************************************************************************************
 ** ��������: һ���ͺ��˲��㷨��Bias�˲���
 ** ��    ��: value����Ҫ�����˲���ֵ
 ** �� �� ֵ: �˲����ֵ
 ** ��    ��: WBN
 ********************************************************************************************
 */
float FirstOrderLagFilter(float value)
{
    static float last_value;  //��һ���˲����

    value=FIRST_LAG_P*value+(1.0f-FIRST_LAG_P)*last_value; //һ���ͺ��˲�
    last_value=value;   //����˴��˲����Ϊ��һ���˲����

    return value;
}

/*
 *******************************************************************************************
 ** ��������: һ���ͺ��˲��㷨��Slope�˲���
 ** ��    ��: value����Ҫ�����˲���ֵ
 ** �� �� ֵ: �˲����ֵ
 ** ��    ��: WBN
 ********************************************************************************************
 */
float FirstOrderLagFilter_Slope(float value)
{
    static float last_value;  //��һ���˲����

    value=FIRST_LAG_P*value+(1.0f-FIRST_LAG_P)*last_value; //һ���ͺ��˲�
    last_value=value;   //����˴��˲����Ϊ��һ���˲����

    return value;
}

/*
 *******************************************************************************************
 ** ��������: �����ͺ��˲��㷨����������˲���
 ** ��    ��: value����Ҫ�����˲���ֵ
 ** �� �� ֵ: �˲����ֵ
 ** ��    ��: WBN
 ********************************************************************************************
 */
float SecondOrderLagFilter_L(float value)
{
    static float last_value,last_2_value;

    value=0.2f*value+0.4f*last_value+0.4f*last_2_value;     //�����ͺ��˲�

    last_2_value=last_value;
    last_value=value;

    return value;
}

/*
 *******************************************************************************************
 ** ��������: �����ͺ��˲��㷨���ұ������˲���
 ** ��    ��: value����Ҫ�����˲���ֵ
 ** �� �� ֵ: �˲����ֵ
 ** ��    ��: WBN
 ********************************************************************************************
 */
float SecondOrderLagFilter_R(float value)
{
    static float last_value,last_2_value;

    value=0.2f*value+0.4f*last_value+0.4f*last_2_value;     //�����ͺ��˲�

    last_2_value=last_value;
    last_value=value;

    return value;
}

/**
 * @brief           һ�׿������˲���ʼ��
 * @param[out]      state : �˲��ṹ����ָ��
 * @param[in]       q & r
 */
void kalman1_init(kalman1_filter_t* state, float q, float r)
{
    state->x = 0;
    state->p = 0.0f;
    state->A = 1.0f;
    state->H = 1.0f;
    state->q = q;
    state->r = r;
}

/**
 * @brief           һ�׿������˲�
 * @param[out]      state : �˲��ṹ����ָ��
 * @param[in]       z_measure : ԭʼ����
 */
float kalman1_filter(kalman1_filter_t* state, float z_measure)
{
    /* Predict */
    // ʱ�����(Ԥ��): X(k|k-1) = A(k,k-1)*X(k-1|k-1) + B(k)*u(k)
    state->x = state->A * state->x;
    // ��������Э����: P(k|k-1) = A(k,k-1)*A(k,k-1)^T*P(k-1|k-1)+Q(k)
    state->p = state->A * state->A * state->p + state->q;

    /* Measurement */
    // ���㿨��������: K(k) = P(k|k-1)*H(k)^T/(P(k|k-1)*H(k)*H(k)^T + R(k))
    state->gain = state->p * state->H / (state->p * state->H * state->H + state->r);
    // ��������(У��): X(k|k) = X(k|k-1)+K(k)*(Z(k)-H(k)*X(k|k-1))
    state->x = state->x + state->gain * (z_measure - state->H * state->x);
    // ���º���Э����: P(k|k) =��I-K(k)*H(k))*P(k|k-1)
    state->p = (1 - state->gain * state->H) * state->p;

    return state->x;
}

