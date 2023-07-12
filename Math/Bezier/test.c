#include "stdio.h"
#include "stdlib.h"
#include "Bezier.h"

int main(int argc, char **argv)
{
    int derivative = 4;                                          // 求导阶数
    PointF in[4] = {{1, 100}, {314, 137}, {97, 25}, {167, 145}}; // 输入点  注:坐标点数量必须大于等于求导阶数

    int num = 1000;  // 输出点数
    PointF out[num]; // 输出点数组

    draw_bezier_curves(in, derivative, out, num); // n阶贝塞尔曲线

    FILE *fp = fopen("points.txt", "w");

    for (int j = 0; j < num; j++) // 输出路径点
    {
        printf("%d\tX=%f\tY=%f\r\n", j, out[j].X, out[j].Y);
        fprintf(fp, "%d\tX=%f\tY=%f\r\n", j, out[j].X, out[j].Y);
    }
    fclose(fp);

    // 调用GUI.py绘图
    system("python3 ../GUI_Bezier.py");
    // end

    return 0;
}