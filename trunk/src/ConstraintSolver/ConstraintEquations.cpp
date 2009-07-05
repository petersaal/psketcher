#include "ConstraintEquations.h"
using namespace GiNaC;


REGISTER_FUNCTION(distance_point_2d,evalf_func(distance_point_2d_evalf).derivative_func(distance_point_2d_derivative));
REGISTER_FUNCTION(distance_point_2d_partial_0,evalf_func(distance_point_2d_partial_0_evalf));
REGISTER_FUNCTION(distance_point_2d_partial_1,evalf_func(distance_point_2d_partial_1_evalf));
REGISTER_FUNCTION(distance_point_2d_partial_2,evalf_func(distance_point_2d_partial_2_evalf));
REGISTER_FUNCTION(distance_point_2d_partial_3,evalf_func(distance_point_2d_partial_3_evalf));
REGISTER_FUNCTION(distance_point_2d_partial_4,evalf_func(distance_point_2d_partial_4_evalf));

static ex distance_point_2d_evalf(const ex & point1s, const ex & point1t, const ex & point2s, const ex & point2t, const ex & distance)
{
    ex equation = -distance + pow((pow((point1s - point2s),2) + pow((point1t - point2t),2)),(1.0/2.0));
    return equation.evalf();
}

static ex distance_point_2d_derivative(const ex & point1s, const ex & point1t, const ex & point2s, const ex & point2t, const ex & distance, unsigned diff_parm)
{
    ex result;
    
    switch(diff_parm){
        case 0:
            result = distance_point_2d_partial_0(point1s, point1t, point2s, point2t, distance);
            break;
        case 1:
            result = distance_point_2d_partial_1(point1s, point1t, point2s, point2t, distance);
            break;
        case 2:
            result = distance_point_2d_partial_2(point1s, point1t, point2s, point2t, distance);
            break;
        case 3:
            result = distance_point_2d_partial_3(point1s, point1t, point2s, point2t, distance);
            break;
        case 4:
            result = distance_point_2d_partial_4(point1s, point1t, point2s, point2t, distance);
            break;
    }
    
    return result;
}
    
static ex distance_point_2d_partial_0_evalf(const ex & point1s, const ex & point1t, const ex & point2s, const ex & point2t, const ex & distance)
{
    ex equation = (point1s - point2s)/pow((pow((point1s - point2s),2) + pow((point1t - point2t),2)),(1.0/2.0));
    return equation.evalf();
}
static ex distance_point_2d_partial_1_evalf(const ex & point1s, const ex & point1t, const ex & point2s, const ex & point2t, const ex & distance)
{
    ex equation = (point1t - point2t)/pow((pow((point1s - point2s),2) + pow((point1t - point2t),2)),(1.0/2.0));
    return equation.evalf();
}
static ex distance_point_2d_partial_2_evalf(const ex & point1s, const ex & point1t, const ex & point2s, const ex & point2t, const ex & distance)
{
    ex equation = (point2s - point1s)/pow((pow((point1s - point2s),2) + pow((point1t - point2t),2)),(1.0/2.0));
    return equation.evalf();
}
static ex distance_point_2d_partial_3_evalf(const ex & point1s, const ex & point1t, const ex & point2s, const ex & point2t, const ex & distance)
{
    ex equation = (point2t - point1t)/pow((pow((point1s - point2s),2) + pow((point1t - point2t),2)),(1.0/2.0));
    return equation.evalf();
}
static ex distance_point_2d_partial_4_evalf(const ex & point1s, const ex & point1t, const ex & point2s, const ex & point2t, const ex & distance)
{
    ex equation = -1;
    return equation.evalf();
}
    



