#ifndef __BEZIER_H
#define __BEZIER_H

typedef struct
{
    float X;
    float Y;
} PointF;

PointF bezier_interpolation_func(float t, PointF *points, int count);                   //通过迭代应用贝塞尔插值算法来计算插值点
void draw_bezier_curves(PointF *points, int count, PointF *out_points, int out_count);  //绘制贝塞尔曲线

#endif