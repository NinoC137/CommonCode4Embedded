#include "Bezier.h"
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
