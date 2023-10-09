#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "FFT.h"

#define FFT_N 512          // FFT采样点数

int main() {
    struct compx s1[FFT_N + 16];

    //开辟声音数据内存区
    float *audioData = (float *) malloc(sizeof(float) * FFT_N);
    for (int i = 0; i < FFT_N; i++) {
        float time = (float) i / FFT_N;
        float sample80Hz = XSin(8 * time);
        float sample500Hz = XSin(1 * time);
        float sample250Hz = XSin(8 * time);
        audioData[i] = sample80Hz + sample500Hz*sample250Hz;
    }

    //采样声音数据
    for (int i = 0; i < FFT_N; i++) {
        s1[i].real = audioData[i];
        s1[i].imag = 0;
    }

    //直流滤波, 减去模拟量信号中的直流分量
    float dc_s1 = 0;

    for (int i = 0; i < FFT_N; i++) {
        dc_s1 += s1[i].real;
    }
    dc_s1 = dc_s1 / FFT_N;
    for (int i = 0; i < FFT_N; i++) {
        s1[i].real = (s1[i].real - dc_s1);
    }

    FFT(s1);

    FILE *fp = fopen("FFT_points.txt", "w");
    //将数据输出至文件,便于python脚本进行绘图 
    for (int i = 0; i < FFT_N / 2; i++) {
        int magnitude = qsqrt((int) (s1[i].real * s1[i].real + s1[i].imag * s1[i].imag)) * (2.0f / FFT_N); //归一化
        float frequency = (float) i;
        printf("%.2f\t", s1[i].imag);
        fprintf(fp, "%d\tX=%.2f\tY=%d\r\n", i, frequency, magnitude);
    }
    fclose(fp);

    find_max_index(s1, FFT_N / 2);
    findPeaks(s1, FFT_N / 2);
    //调用绘图脚本
    system("python3 ./GUI_FFT.py");

    free(audioData);
    return 0;
}
