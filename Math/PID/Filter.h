/*
 * Filter.h
 *
 *  Created on: 2022��3��18��
 *      Author: yue
 */

#ifndef CODE_FILTER_H_
#define CODE_FILTER_H_

#define FIRST_LAG_P 0.8f     //һ���ͺ��˲�ϵ����ԽС�ͺ�Ч��Խǿ

typedef struct {
    float x;        // state
    float A;        // x(n)=A*x(n-1)+u(n),u(n)~N(0,q)
    float H;        // z(n)=H*x(n)+w(n),w(n)~N(0,r)
    float q;        // process(predict) noise convariance
    float r;        // measure noise(error) convariance
    float p;        // estimated error convariance
    float gain;     // kalman gain
} kalman1_filter_t;

extern kalman1_filter_t kalman_gyro;       //���忨�����ṹ��

float FirstOrderLagFilter(float value); //һ���ͺ��˲��㷨
float FirstOrderLagFilter_Slope(float value);
float SecondOrderLagFilter_L(float value);
float SecondOrderLagFilter_R(float value);

void kalman1_init(kalman1_filter_t* state, float q, float r);
float kalman1_filter(kalman1_filter_t* state, float z_measure);

#endif /* CODE_FILTER_H_ */


