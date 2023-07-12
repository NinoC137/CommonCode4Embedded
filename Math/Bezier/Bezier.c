#include "stdio.h"
#include "math.h"
#include "assert.h"

typedef struct
{
    float X;
    float Y;
} PointF;

/*
Bezier interpolation funtion
    计算给定控制点的贝塞尔插值点
    通过迭代应用贝塞尔插值算法来计算插值点
参数：
    t：要计算插值点的参数值，范围从0到1
    points：定义贝塞尔曲线的控制点数组
    count：控制点数组的数量
返回值：
    表示参数t处的插值点的 xy坐标值
*/
PointF bezier_interpolation_func(float t, PointF *points, int count)
{
    assert(count > 0);

    PointF tmp_points[count];
    for (int i = 1; i < count; ++i)
    {
        for (int j = 0; j < count - i; ++j)
        {
            if (i == 1)
            {
                tmp_points[j].X = (float)(points[j].X * (1 - t) + points[j + 1].X * t);
                tmp_points[j].Y = (float)(points[j].Y * (1 - t) + points[j + 1].Y * t);
                continue;
            }
            tmp_points[j].X = (float)(tmp_points[j].X * (1 - t) + tmp_points[j + 1].X * t);
            tmp_points[j].Y = (float)(tmp_points[j].Y * (1 - t) + tmp_points[j + 1].Y * t);
        }
    }
    return tmp_points[0];
}

/*
Draw bezier curves
    绘制贝塞尔曲线，生成沿曲线的插值点
    函数使用 bezier_interpolation_func 来计算插值点
参数：
    points：定义贝塞尔曲线的控制点数组
    count：控制点数组的数量
    out_points：用于存储生成的插值点的数组
    out_count：要生成的插值点数量
*/
void draw_bezier_curves(PointF *points, int count, PointF *out_points, int out_count)
{
    float step = 1.0 / out_count;
    float t = 0;
    for (int i = 0; i < out_count; i++)
    {
        PointF temp_point = bezier_interpolation_func(t, points, count); // 计算插值点
        t += step;
        out_points[i] = temp_point;
    }
}

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