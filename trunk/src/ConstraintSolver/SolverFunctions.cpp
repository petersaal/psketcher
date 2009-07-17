#include <cmath>

#include "SolverFunctions.h"



distance_point_2d::distance_point_2d(DOFPointer point1s, DOFPointer point1t, DOFPointer point2s, DOFPointer point2t, DOFPointer distance)
{
    AddDOF(point1s);
    AddDOF(point1t);
    AddDOF(point2s);
    AddDOF(point2t);
    AddDOF(distance);
}

double distance_point_2d::GetValue() const
{
    double point1s = GetDOF(0)->GetValue();
    double point1t = GetDOF(1)->GetValue();
    double point2s = GetDOF(2)->GetValue();
    double point2t = GetDOF(3)->GetValue();
    double distance = GetDOF(4)->GetValue();

    return -distance + pow((pow((point1s - point2s),2) + pow((point1t - point2t),2)),(1.0/2.0));
}

double distance_point_2d::GetValueSelf(const mmcMatrix &params) const
{
    double point1s = params(0,0);
    double point1t = params(1,0);
    double point2s = params(2,0);
    double point2t = params(3,0);
    double distance = params(4,0);

    return -distance + pow((pow((point1s - point2s),2) + pow((point1t - point2t),2)),(1.0/2.0));
}

mmcMatrix distance_point_2d::GetGradientSelf(const mmcMatrix &params) const
{
    mmcMatrix result(GetNumDOFs(),1);

    double point1s = params(0,0);
    double point1t = params(1,0);
    double point2s = params(2,0);
    double point2t = params(3,0);
    double distance = params(4,0);

    result(0,0) = (point1s - point2s)/pow((pow((point1s - point2s),2) + pow((point1t - point2t),2)),(1.0/2.0));
    result(1,0) = (point1t - point2t)/pow((pow((point1s - point2s),2) + pow((point1t - point2t),2)),(1.0/2.0));
    result(2,0) = (point2s - point1s)/pow((pow((point1s - point2s),2) + pow((point1t - point2t),2)),(1.0/2.0));
    result(3,0) = (point2t - point1t)/pow((pow((point1s - point2s),2) + pow((point1t - point2t),2)),(1.0/2.0));
    result(4,0) = -1;

    return result;
}

angle_line_2d_interior::angle_line_2d_interior(DOFPointer line1_point1s, DOFPointer line1_point1t, DOFPointer line1_point2s, DOFPointer line1_point2t, DOFPointer line2_point1s, DOFPointer line2_point1t, DOFPointer line2_point2s, DOFPointer line2_point2t, DOFPointer angle)
{
    AddDOF(line1_point1s);
    AddDOF(line1_point1t);
    AddDOF(line1_point2s);
    AddDOF(line1_point2t);
    AddDOF(line2_point1s);
    AddDOF(line2_point1t);
    AddDOF(line2_point2s);
    AddDOF(line2_point2t);
    AddDOF(angle);
}

double angle_line_2d_interior::GetValue() const
{
    double line1_point1s = GetDOF(0)->GetValue();
    double line1_point1t = GetDOF(1)->GetValue();
    double line1_point2s = GetDOF(2)->GetValue();
    double line1_point2t = GetDOF(3)->GetValue();
    double line2_point1s = GetDOF(4)->GetValue();
    double line2_point1t = GetDOF(5)->GetValue();
    double line2_point2s = GetDOF(6)->GetValue();
    double line2_point2t = GetDOF(7)->GetValue();
    double angle = GetDOF(8)->GetValue();

    return -cos(angle) + ((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
}

double angle_line_2d_interior::GetValueSelf(const mmcMatrix &params) const
{
    double line1_point1s = params(0,0);
    double line1_point1t = params(1,0);
    double line1_point2s = params(2,0);
    double line1_point2t = params(3,0);
    double line2_point1s = params(4,0);
    double line2_point1t = params(5,0);
    double line2_point2s = params(6,0);
    double line2_point2t = params(7,0);
    double angle = params(8,0);

    return -cos(angle) + ((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
}

mmcMatrix angle_line_2d_interior::GetGradientSelf(const mmcMatrix &params) const
{
    mmcMatrix result(GetNumDOFs(),1);

    double line1_point1s = params(0,0);
    double line1_point1t = params(1,0);
    double line1_point2s = params(2,0);
    double line1_point2t = params(3,0);
    double line2_point1s = params(4,0);
    double line2_point1t = params(5,0);
    double line2_point2s = params(6,0);
    double line2_point2t = params(7,0);
    double angle = params(8,0);

    result(0,0) = (line2_point1s - line2_point2s)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line1_point2s - line1_point1s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(3.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
    result(1,0) = (line2_point1t - line2_point2t)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line1_point2t - line1_point1t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(3.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
    result(2,0) = (line2_point2s - line2_point1s)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line1_point1s - line1_point2s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(3.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
    result(3,0) = (line2_point2t - line2_point1t)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line1_point1t - line1_point2t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(3.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
    result(4,0) = (line1_point1s - line1_point2s)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line2_point2s - line2_point1s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(3.0/2.0)));
    result(5,0) = (line1_point1t - line1_point2t)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line2_point2t - line2_point1t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(3.0/2.0)));
    result(6,0) = (line1_point2s - line1_point1s)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line2_point1s - line2_point2s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(3.0/2.0)));
    result(7,0) = (line1_point2t - line1_point1t)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line2_point1t - line2_point2t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(3.0/2.0)));
    result(8,0) = sin(angle);

    return result;
}

angle_line_2d_exterior::angle_line_2d_exterior(DOFPointer line1_point1s, DOFPointer line1_point1t, DOFPointer line1_point2s, DOFPointer line1_point2t, DOFPointer line2_point1s, DOFPointer line2_point1t, DOFPointer line2_point2s, DOFPointer line2_point2t, DOFPointer angle)
{
    AddDOF(line1_point1s);
    AddDOF(line1_point1t);
    AddDOF(line1_point2s);
    AddDOF(line1_point2t);
    AddDOF(line2_point1s);
    AddDOF(line2_point1t);
    AddDOF(line2_point2s);
    AddDOF(line2_point2t);
    AddDOF(angle);
}

double angle_line_2d_exterior::GetValue() const
{
    double line1_point1s = GetDOF(0)->GetValue();
    double line1_point1t = GetDOF(1)->GetValue();
    double line1_point2s = GetDOF(2)->GetValue();
    double line1_point2t = GetDOF(3)->GetValue();
    double line2_point1s = GetDOF(4)->GetValue();
    double line2_point1t = GetDOF(5)->GetValue();
    double line2_point2s = GetDOF(6)->GetValue();
    double line2_point2t = GetDOF(7)->GetValue();
    double angle = GetDOF(8)->GetValue();

    return ((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + cos(angle);
}

double angle_line_2d_exterior::GetValueSelf(const mmcMatrix &params) const
{
    double line1_point1s = params(0,0);
    double line1_point1t = params(1,0);
    double line1_point2s = params(2,0);
    double line1_point2t = params(3,0);
    double line2_point1s = params(4,0);
    double line2_point1t = params(5,0);
    double line2_point2s = params(6,0);
    double line2_point2t = params(7,0);
    double angle = params(8,0);

    return ((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + cos(angle);
}

mmcMatrix angle_line_2d_exterior::GetGradientSelf(const mmcMatrix &params) const
{
    mmcMatrix result(GetNumDOFs(),1);

    double line1_point1s = params(0,0);
    double line1_point1t = params(1,0);
    double line1_point2s = params(2,0);
    double line1_point2t = params(3,0);
    double line2_point1s = params(4,0);
    double line2_point1t = params(5,0);
    double line2_point2s = params(6,0);
    double line2_point2t = params(7,0);
    double angle = params(8,0);

    result(0,0) = (line2_point1s - line2_point2s)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line1_point2s - line1_point1s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(3.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
    result(1,0) = (line2_point1t - line2_point2t)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line1_point2t - line1_point1t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(3.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
    result(2,0) = (line2_point2s - line2_point1s)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line1_point1s - line1_point2s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(3.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
    result(3,0) = (line2_point2t - line2_point1t)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line1_point1t - line1_point2t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(3.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
    result(4,0) = (line1_point1s - line1_point2s)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line2_point2s - line2_point1s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(3.0/2.0)));
    result(5,0) = (line1_point1t - line1_point2t)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line2_point2t - line2_point1t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(3.0/2.0)));
    result(6,0) = (line1_point2s - line1_point1s)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line2_point1s - line2_point2s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(3.0/2.0)));
    result(7,0) = (line1_point2t - line1_point1t)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line2_point1t - line2_point2t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(3.0/2.0)));
    result(8,0) = -sin(angle);

    return result;
}

tangent_edge_2d::tangent_edge_2d(DOFPointer s1, DOFPointer t1, DOFPointer s2, DOFPointer t2)
{
    AddDOF(s1);
    AddDOF(t1);
    AddDOF(s2);
    AddDOF(t2);
}

double tangent_edge_2d::GetValue() const
{
    double s1 = GetDOF(0)->GetValue();
    double t1 = GetDOF(1)->GetValue();
    double s2 = GetDOF(2)->GetValue();
    double t2 = GetDOF(3)->GetValue();

    return -1 + pow((s1*s2 - t1*t2),2);
}

double tangent_edge_2d::GetValueSelf(const mmcMatrix &params) const
{
    double s1 = params(0,0);
    double t1 = params(1,0);
    double s2 = params(2,0);
    double t2 = params(3,0);

    return -1 + pow((s1*s2 - t1*t2),2);
}

mmcMatrix tangent_edge_2d::GetGradientSelf(const mmcMatrix &params) const
{
    mmcMatrix result(GetNumDOFs(),1);

    double s1 = params(0,0);
    double t1 = params(1,0);
    double s2 = params(2,0);
    double t2 = params(3,0);

    result(0,0) = 2*s2*(s1*s2 - t1*t2);
    result(1,0) = -2*t2*(s1*s2 - t1*t2);
    result(2,0) = 2*s1*(s1*s2 - t1*t2);
    result(3,0) = -2*t1*(s1*s2 - t1*t2);

    return result;
}

parallel_line_2d::parallel_line_2d(DOFPointer line1_point1s, DOFPointer line1_point1t, DOFPointer line1_point2s, DOFPointer line1_point2t, DOFPointer line2_point1s, DOFPointer line2_point1t, DOFPointer line2_point2s, DOFPointer line2_point2t)
{
    AddDOF(line1_point1s);
    AddDOF(line1_point1t);
    AddDOF(line1_point2s);
    AddDOF(line1_point2t);
    AddDOF(line2_point1s);
    AddDOF(line2_point1t);
    AddDOF(line2_point2s);
    AddDOF(line2_point2t);
}

double parallel_line_2d::GetValue() const
{
    double line1_point1s = GetDOF(0)->GetValue();
    double line1_point1t = GetDOF(1)->GetValue();
    double line1_point2s = GetDOF(2)->GetValue();
    double line1_point2t = GetDOF(3)->GetValue();
    double line2_point1s = GetDOF(4)->GetValue();
    double line2_point1t = GetDOF(5)->GetValue();
    double line2_point2s = GetDOF(6)->GetValue();
    double line2_point2t = GetDOF(7)->GetValue();

    return -1 + pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)));
}

double parallel_line_2d::GetValueSelf(const mmcMatrix &params) const
{
    double line1_point1s = params(0,0);
    double line1_point1t = params(1,0);
    double line1_point2s = params(2,0);
    double line1_point2t = params(3,0);
    double line2_point1s = params(4,0);
    double line2_point1t = params(5,0);
    double line2_point2s = params(6,0);
    double line2_point2t = params(7,0);

    return -1 + pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)));
}

mmcMatrix parallel_line_2d::GetGradientSelf(const mmcMatrix &params) const
{
    mmcMatrix result(GetNumDOFs(),1);

    double line1_point1s = params(0,0);
    double line1_point1t = params(1,0);
    double line1_point2s = params(2,0);
    double line1_point2t = params(3,0);
    double line2_point1s = params(4,0);
    double line2_point1t = params(5,0);
    double line2_point2s = params(6,0);
    double line2_point2t = params(7,0);

    result(0,0) = (-2*line2_point2s + 2*line2_point1s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2))) + pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)*(-2*line1_point1s + 2*line1_point2s)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),2)*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)));
    result(1,0) = (-2*line2_point2t + 2*line2_point1t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2))) + pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)*(-2*line1_point1t + 2*line1_point2t)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),2)*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)));
    result(2,0) = (-2*line2_point1s + 2*line2_point2s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2))) + pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)*(-2*line1_point2s + 2*line1_point1s)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),2)*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)));
    result(3,0) = (-2*line2_point1t + 2*line2_point2t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2))) + pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)*(-2*line1_point2t + 2*line1_point1t)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),2)*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)));
    result(4,0) = (-2*line1_point2s + 2*line1_point1s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2))) + pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)*(-2*line2_point1s + 2*line2_point2s)/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),2));
    result(5,0) = (-2*line1_point2t + 2*line1_point1t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2))) + pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)*(-2*line2_point1t + 2*line2_point2t)/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),2));
    result(6,0) = (-2*line1_point1s + 2*line1_point2s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2))) + pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)*(-2*line2_point2s + 2*line2_point1s)/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),2));
    result(7,0) = (-2*line1_point1t + 2*line1_point2t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2))) + pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)*(-2*line2_point2t + 2*line2_point1t)/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),2));

    return result;
}

