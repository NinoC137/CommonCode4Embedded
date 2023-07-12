#ifndef __ALGORITHM_H
#define __ALGORITHM_H

#include "stdint.h"
#include "stdio.h"

#define FFT_N           512     //定义傅里叶变换的点数
#define START_INDEX     4       //低频过滤阈值
#define MAX_PEAKS       10      //最大峰值采集量

//定义一个复数结构
struct compx {
    float real;
    float imag;
};

double my_floor(double x);              //向下取整
double my_fmod(double x, double y);     //求余运算
double XSin(double x);                  //正弦函数
double XCos(double x);                  //余弦函数
int qsqrt(int a);                       //开平方

/*******************************************************************
函数原型：struct compx EE(struct compx b1,struct compx b2)  
函数功能：对两个复数进行乘法运算
输入参数：两个以联合体定义的复数a,b
输出参数：a和b的乘积，以联合体的形式输出
*******************************************************************/
struct compx EE(struct compx a, struct compx b);

/*****************************************************************
函数原型：void FFT(struct compx *xin,int N)
函数功能：对输入的复数组进行快速傅里叶变换（FFT）
输入参数：*xin复数结构体组的首地址指针，struct型
*****************************************************************/
void FFT(struct compx *xin);

int find_max_index(struct compx *data, int length);     //读取最大峰值
void findPeaks(struct compx *data, int length);         //读取极大峰值

extern int peakIndexes[MAX_PEAKS];      //极大峰值索引
extern int peakValues[MAX_PEAKS];       //极大峰值峰值

#endif  /*__ALGORITHM_H*/
