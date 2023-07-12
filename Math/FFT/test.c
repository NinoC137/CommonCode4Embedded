#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "FFT.h"

#define SAMPLE_RATE 44100  // 采样率
#define DURATION 1.0       // 音频时长（秒）
#define FFT_N 512          // FFT采样点数

int main() {
    struct compx s1[FFT_N+16];

    float baseFrequency = 50.0f;  // 基础频率

    // Generate audio data
    float* audioData = (float*)malloc(sizeof(float) * FFT_N);
    for (int i = 0; i < FFT_N; i++) {
        float time = (float)i / FFT_N;
        float sample = 0.0f;
        for (int harmonic = 1; harmonic <= 5; harmonic++) {
            float frequency = baseFrequency * (float)harmonic;
            float amplitude = 1.0f / (float)harmonic;
            sample += (float)amplitude * (float)sin(2 * M_PI * frequency * time);
        }
        audioData[i] = sample;
    }

    // Generate audio data
//    float* audioData = (float*)malloc(sizeof(float) * FFT_N);
//    for (int i = 0; i < FFT_N; i++) {
//        float time = (float)i / FFT_N;
//        float sample80Hz = XSin(2 * M_PI * 100 * time);
//        float sample500Hz = XSin(2 * M_PI * 128 * time);
//        audioData[i] = sample80Hz + sample500Hz;
//    }

    // Extract FFT_N samples
    for (int i = 0; i < FFT_N; i++) {
        s1[i].real = audioData[i];
        s1[i].imag = 0;
    }

    FFT(s1);

    FILE *fp = fopen("FFT_points.txt", "w");
    // Print extracted samples
    for (int i = 0; i < FFT_N/2; i++) {
        int magnitude = qsqrt((int)(s1[i].real * s1[i].real + s1[i].imag * s1[i].imag));
        float frequency = (float)i;
        printf("%d\t", magnitude);
        fprintf(fp, "%d\tX=%.2f\tY=%d\r\n", i, frequency, magnitude);
    }
    fclose(fp);

    system("python3 ../GUI_FFT.py");

    free(audioData);
    return 0;
}
