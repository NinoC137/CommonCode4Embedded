/*
 * Filter.h
 *
 *  Created on: 2022年3月18日
 *      Author: yue
 */

#ifndef CODE_FILTER_H_
#define CODE_FILTER_H_

#define FIRST_LAG_P 0.8f     //一阶滞后滤波系数，越小滞后效果越强

typedef struct {
    float x;        // state
    float A;        // x(n)=A*x(n-1)+u(n),u(n)~N(0,q)
    float H;        // z(n)=H*x(n)+w(n),w(n)~N(0,r)
    float q;        // process(predict) noise convariance
    float r;        // measure noise(error) convariance
    float p;        // estimated error convariance
    float gain;     // kalman gain
} kalman1_filter_t;

extern kalman1_filter_t kalman_gyro;       //定义卡尔曼结构体

float FirstOrderLagFilter(float value); //一阶滞后滤波算法
float FirstOrderLagFilter_Slope(float value);
float SecondOrderLagFilter_L(float value);
float SecondOrderLagFilter_R(float value);

void kalman1_init(kalman1_filter_t* state, float q, float r);
float kalman1_filter(kalman1_filter_t* state, float z_measure);

#endif /* CODE_FILTER_H_ */


