#include "SolverFunctions.h"
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
    
REGISTER_FUNCTION(angle_line_2d_interior,evalf_func(angle_line_2d_interior_evalf).derivative_func(angle_line_2d_interior_derivative));
REGISTER_FUNCTION(angle_line_2d_interior_partial_0,evalf_func(angle_line_2d_interior_partial_0_evalf));
REGISTER_FUNCTION(angle_line_2d_interior_partial_1,evalf_func(angle_line_2d_interior_partial_1_evalf));
REGISTER_FUNCTION(angle_line_2d_interior_partial_2,evalf_func(angle_line_2d_interior_partial_2_evalf));
REGISTER_FUNCTION(angle_line_2d_interior_partial_3,evalf_func(angle_line_2d_interior_partial_3_evalf));
REGISTER_FUNCTION(angle_line_2d_interior_partial_4,evalf_func(angle_line_2d_interior_partial_4_evalf));
REGISTER_FUNCTION(angle_line_2d_interior_partial_5,evalf_func(angle_line_2d_interior_partial_5_evalf));
REGISTER_FUNCTION(angle_line_2d_interior_partial_6,evalf_func(angle_line_2d_interior_partial_6_evalf));
REGISTER_FUNCTION(angle_line_2d_interior_partial_7,evalf_func(angle_line_2d_interior_partial_7_evalf));
REGISTER_FUNCTION(angle_line_2d_interior_partial_8,evalf_func(angle_line_2d_interior_partial_8_evalf));

static ex angle_line_2d_interior_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t, const ex & angle)
{
    ex equation = -cos(angle) + ((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
    return equation.evalf();
}

static ex angle_line_2d_interior_derivative(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t, const ex & angle, unsigned diff_parm)
{
    ex result;
    
    switch(diff_parm){
        case 0:
            result = angle_line_2d_interior_partial_0(line1_point1s, line1_point1t, line1_point2s, line1_point2t, line2_point1s, line2_point1t, line2_point2s, line2_point2t, angle);
            break;
        case 1:
            result = angle_line_2d_interior_partial_1(line1_point1s, line1_point1t, line1_point2s, line1_point2t, line2_point1s, line2_point1t, line2_point2s, line2_point2t, angle);
            break;
        case 2:
            result = angle_line_2d_interior_partial_2(line1_point1s, line1_point1t, line1_point2s, line1_point2t, line2_point1s, line2_point1t, line2_point2s, line2_point2t, angle);
            break;
        case 3:
            result = angle_line_2d_interior_partial_3(line1_point1s, line1_point1t, line1_point2s, line1_point2t, line2_point1s, line2_point1t, line2_point2s, line2_point2t, angle);
            break;
        case 4:
            result = angle_line_2d_interior_partial_4(line1_point1s, line1_point1t, line1_point2s, line1_point2t, line2_point1s, line2_point1t, line2_point2s, line2_point2t, angle);
            break;
        case 5:
            result = angle_line_2d_interior_partial_5(line1_point1s, line1_point1t, line1_point2s, line1_point2t, line2_point1s, line2_point1t, line2_point2s, line2_point2t, angle);
            break;
        case 6:
            result = angle_line_2d_interior_partial_6(line1_point1s, line1_point1t, line1_point2s, line1_point2t, line2_point1s, line2_point1t, line2_point2s, line2_point2t, angle);
            break;
        case 7:
            result = angle_line_2d_interior_partial_7(line1_point1s, line1_point1t, line1_point2s, line1_point2t, line2_point1s, line2_point1t, line2_point2s, line2_point2t, angle);
            break;
        case 8:
            result = angle_line_2d_interior_partial_8(line1_point1s, line1_point1t, line1_point2s, line1_point2t, line2_point1s, line2_point1t, line2_point2s, line2_point2t, angle);
            break;
    }
    
    return result;
}
    
static ex angle_line_2d_interior_partial_0_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t, const ex & angle)
{
    ex equation = (line2_point1s - line2_point2s)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line1_point2s - line1_point1s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(3.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
    return equation.evalf();
}
static ex angle_line_2d_interior_partial_1_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t, const ex & angle)
{
    ex equation = (line2_point1t - line2_point2t)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line1_point2t - line1_point1t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(3.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
    return equation.evalf();
}
static ex angle_line_2d_interior_partial_2_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t, const ex & angle)
{
    ex equation = (line2_point2s - line2_point1s)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line1_point1s - line1_point2s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(3.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
    return equation.evalf();
}
static ex angle_line_2d_interior_partial_3_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t, const ex & angle)
{
    ex equation = (line2_point2t - line2_point1t)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line1_point1t - line1_point2t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(3.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
    return equation.evalf();
}
static ex angle_line_2d_interior_partial_4_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t, const ex & angle)
{
    ex equation = (line1_point1s - line1_point2s)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line2_point2s - line2_point1s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(3.0/2.0)));
    return equation.evalf();
}
static ex angle_line_2d_interior_partial_5_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t, const ex & angle)
{
    ex equation = (line1_point1t - line1_point2t)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line2_point2t - line2_point1t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(3.0/2.0)));
    return equation.evalf();
}
static ex angle_line_2d_interior_partial_6_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t, const ex & angle)
{
    ex equation = (line1_point2s - line1_point1s)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line2_point1s - line2_point2s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(3.0/2.0)));
    return equation.evalf();
}
static ex angle_line_2d_interior_partial_7_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t, const ex & angle)
{
    ex equation = (line1_point2t - line1_point1t)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line2_point1t - line2_point2t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(3.0/2.0)));
    return equation.evalf();
}
static ex angle_line_2d_interior_partial_8_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t, const ex & angle)
{
    ex equation = sin(angle);
    return equation.evalf();
}
    
REGISTER_FUNCTION(angle_line_2d_exterior,evalf_func(angle_line_2d_exterior_evalf).derivative_func(angle_line_2d_exterior_derivative));
REGISTER_FUNCTION(angle_line_2d_exterior_partial_0,evalf_func(angle_line_2d_exterior_partial_0_evalf));
REGISTER_FUNCTION(angle_line_2d_exterior_partial_1,evalf_func(angle_line_2d_exterior_partial_1_evalf));
REGISTER_FUNCTION(angle_line_2d_exterior_partial_2,evalf_func(angle_line_2d_exterior_partial_2_evalf));
REGISTER_FUNCTION(angle_line_2d_exterior_partial_3,evalf_func(angle_line_2d_exterior_partial_3_evalf));
REGISTER_FUNCTION(angle_line_2d_exterior_partial_4,evalf_func(angle_line_2d_exterior_partial_4_evalf));
REGISTER_FUNCTION(angle_line_2d_exterior_partial_5,evalf_func(angle_line_2d_exterior_partial_5_evalf));
REGISTER_FUNCTION(angle_line_2d_exterior_partial_6,evalf_func(angle_line_2d_exterior_partial_6_evalf));
REGISTER_FUNCTION(angle_line_2d_exterior_partial_7,evalf_func(angle_line_2d_exterior_partial_7_evalf));
REGISTER_FUNCTION(angle_line_2d_exterior_partial_8,evalf_func(angle_line_2d_exterior_partial_8_evalf));

static ex angle_line_2d_exterior_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t, const ex & angle)
{
    ex equation = ((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + cos(angle);
    return equation.evalf();
}

static ex angle_line_2d_exterior_derivative(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t, const ex & angle, unsigned diff_parm)
{
    ex result;
    
    switch(diff_parm){
        case 0:
            result = angle_line_2d_exterior_partial_0(line1_point1s, line1_point1t, line1_point2s, line1_point2t, line2_point1s, line2_point1t, line2_point2s, line2_point2t, angle);
            break;
        case 1:
            result = angle_line_2d_exterior_partial_1(line1_point1s, line1_point1t, line1_point2s, line1_point2t, line2_point1s, line2_point1t, line2_point2s, line2_point2t, angle);
            break;
        case 2:
            result = angle_line_2d_exterior_partial_2(line1_point1s, line1_point1t, line1_point2s, line1_point2t, line2_point1s, line2_point1t, line2_point2s, line2_point2t, angle);
            break;
        case 3:
            result = angle_line_2d_exterior_partial_3(line1_point1s, line1_point1t, line1_point2s, line1_point2t, line2_point1s, line2_point1t, line2_point2s, line2_point2t, angle);
            break;
        case 4:
            result = angle_line_2d_exterior_partial_4(line1_point1s, line1_point1t, line1_point2s, line1_point2t, line2_point1s, line2_point1t, line2_point2s, line2_point2t, angle);
            break;
        case 5:
            result = angle_line_2d_exterior_partial_5(line1_point1s, line1_point1t, line1_point2s, line1_point2t, line2_point1s, line2_point1t, line2_point2s, line2_point2t, angle);
            break;
        case 6:
            result = angle_line_2d_exterior_partial_6(line1_point1s, line1_point1t, line1_point2s, line1_point2t, line2_point1s, line2_point1t, line2_point2s, line2_point2t, angle);
            break;
        case 7:
            result = angle_line_2d_exterior_partial_7(line1_point1s, line1_point1t, line1_point2s, line1_point2t, line2_point1s, line2_point1t, line2_point2s, line2_point2t, angle);
            break;
        case 8:
            result = angle_line_2d_exterior_partial_8(line1_point1s, line1_point1t, line1_point2s, line1_point2t, line2_point1s, line2_point1t, line2_point2s, line2_point2t, angle);
            break;
    }
    
    return result;
}
    
static ex angle_line_2d_exterior_partial_0_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t, const ex & angle)
{
    ex equation = (line2_point1s - line2_point2s)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line1_point2s - line1_point1s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(3.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
    return equation.evalf();
}
static ex angle_line_2d_exterior_partial_1_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t, const ex & angle)
{
    ex equation = (line2_point1t - line2_point2t)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line1_point2t - line1_point1t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(3.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
    return equation.evalf();
}
static ex angle_line_2d_exterior_partial_2_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t, const ex & angle)
{
    ex equation = (line2_point2s - line2_point1s)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line1_point1s - line1_point2s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(3.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
    return equation.evalf();
}
static ex angle_line_2d_exterior_partial_3_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t, const ex & angle)
{
    ex equation = (line2_point2t - line2_point1t)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line1_point1t - line1_point2t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(3.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
    return equation.evalf();
}
static ex angle_line_2d_exterior_partial_4_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t, const ex & angle)
{
    ex equation = (line1_point1s - line1_point2s)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line2_point2s - line2_point1s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(3.0/2.0)));
    return equation.evalf();
}
static ex angle_line_2d_exterior_partial_5_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t, const ex & angle)
{
    ex equation = (line1_point1t - line1_point2t)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line2_point2t - line2_point1t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(3.0/2.0)));
    return equation.evalf();
}
static ex angle_line_2d_exterior_partial_6_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t, const ex & angle)
{
    ex equation = (line1_point2s - line1_point1s)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line2_point1s - line2_point2s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(3.0/2.0)));
    return equation.evalf();
}
static ex angle_line_2d_exterior_partial_7_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t, const ex & angle)
{
    ex equation = (line1_point2t - line1_point1t)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line2_point1t - line2_point2t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),(3.0/2.0)));
    return equation.evalf();
}
static ex angle_line_2d_exterior_partial_8_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t, const ex & angle)
{
    ex equation = -sin(angle);
    return equation.evalf();
}
    
REGISTER_FUNCTION(tangent_edge_2d,evalf_func(tangent_edge_2d_evalf).derivative_func(tangent_edge_2d_derivative));
REGISTER_FUNCTION(tangent_edge_2d_partial_0,evalf_func(tangent_edge_2d_partial_0_evalf));
REGISTER_FUNCTION(tangent_edge_2d_partial_1,evalf_func(tangent_edge_2d_partial_1_evalf));
REGISTER_FUNCTION(tangent_edge_2d_partial_2,evalf_func(tangent_edge_2d_partial_2_evalf));
REGISTER_FUNCTION(tangent_edge_2d_partial_3,evalf_func(tangent_edge_2d_partial_3_evalf));

static ex tangent_edge_2d_evalf(const ex & s1, const ex & t1, const ex & s2, const ex & t2)
{
    ex equation = -1 + pow((s1*s2 - t1*t2),2);
    return equation.evalf();
}

static ex tangent_edge_2d_derivative(const ex & s1, const ex & t1, const ex & s2, const ex & t2, unsigned diff_parm)
{
    ex result;
    
    switch(diff_parm){
        case 0:
            result = tangent_edge_2d_partial_0(s1, t1, s2, t2);
            break;
        case 1:
            result = tangent_edge_2d_partial_1(s1, t1, s2, t2);
            break;
        case 2:
            result = tangent_edge_2d_partial_2(s1, t1, s2, t2);
            break;
        case 3:
            result = tangent_edge_2d_partial_3(s1, t1, s2, t2);
            break;
    }
    
    return result;
}
    
static ex tangent_edge_2d_partial_0_evalf(const ex & s1, const ex & t1, const ex & s2, const ex & t2)
{
    ex equation = 2*s2*(s1*s2 - t1*t2);
    return equation.evalf();
}
static ex tangent_edge_2d_partial_1_evalf(const ex & s1, const ex & t1, const ex & s2, const ex & t2)
{
    ex equation = -2*t2*(s1*s2 - t1*t2);
    return equation.evalf();
}
static ex tangent_edge_2d_partial_2_evalf(const ex & s1, const ex & t1, const ex & s2, const ex & t2)
{
    ex equation = 2*s1*(s1*s2 - t1*t2);
    return equation.evalf();
}
static ex tangent_edge_2d_partial_3_evalf(const ex & s1, const ex & t1, const ex & s2, const ex & t2)
{
    ex equation = -2*t1*(s1*s2 - t1*t2);
    return equation.evalf();
}
    
REGISTER_FUNCTION(parallel_line_2d,evalf_func(parallel_line_2d_evalf).derivative_func(parallel_line_2d_derivative));
REGISTER_FUNCTION(parallel_line_2d_partial_0,evalf_func(parallel_line_2d_partial_0_evalf));
REGISTER_FUNCTION(parallel_line_2d_partial_1,evalf_func(parallel_line_2d_partial_1_evalf));
REGISTER_FUNCTION(parallel_line_2d_partial_2,evalf_func(parallel_line_2d_partial_2_evalf));
REGISTER_FUNCTION(parallel_line_2d_partial_3,evalf_func(parallel_line_2d_partial_3_evalf));
REGISTER_FUNCTION(parallel_line_2d_partial_4,evalf_func(parallel_line_2d_partial_4_evalf));
REGISTER_FUNCTION(parallel_line_2d_partial_5,evalf_func(parallel_line_2d_partial_5_evalf));
REGISTER_FUNCTION(parallel_line_2d_partial_6,evalf_func(parallel_line_2d_partial_6_evalf));
REGISTER_FUNCTION(parallel_line_2d_partial_7,evalf_func(parallel_line_2d_partial_7_evalf));

static ex parallel_line_2d_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t)
{
    ex equation = -1 + pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)));
    return equation.evalf();
}

static ex parallel_line_2d_derivative(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t, unsigned diff_parm)
{
    ex result;
    
    switch(diff_parm){
        case 0:
            result = parallel_line_2d_partial_0(line1_point1s, line1_point1t, line1_point2s, line1_point2t, line2_point1s, line2_point1t, line2_point2s, line2_point2t);
            break;
        case 1:
            result = parallel_line_2d_partial_1(line1_point1s, line1_point1t, line1_point2s, line1_point2t, line2_point1s, line2_point1t, line2_point2s, line2_point2t);
            break;
        case 2:
            result = parallel_line_2d_partial_2(line1_point1s, line1_point1t, line1_point2s, line1_point2t, line2_point1s, line2_point1t, line2_point2s, line2_point2t);
            break;
        case 3:
            result = parallel_line_2d_partial_3(line1_point1s, line1_point1t, line1_point2s, line1_point2t, line2_point1s, line2_point1t, line2_point2s, line2_point2t);
            break;
        case 4:
            result = parallel_line_2d_partial_4(line1_point1s, line1_point1t, line1_point2s, line1_point2t, line2_point1s, line2_point1t, line2_point2s, line2_point2t);
            break;
        case 5:
            result = parallel_line_2d_partial_5(line1_point1s, line1_point1t, line1_point2s, line1_point2t, line2_point1s, line2_point1t, line2_point2s, line2_point2t);
            break;
        case 6:
            result = parallel_line_2d_partial_6(line1_point1s, line1_point1t, line1_point2s, line1_point2t, line2_point1s, line2_point1t, line2_point2s, line2_point2t);
            break;
        case 7:
            result = parallel_line_2d_partial_7(line1_point1s, line1_point1t, line1_point2s, line1_point2t, line2_point1s, line2_point1t, line2_point2s, line2_point2t);
            break;
    }
    
    return result;
}
    
static ex parallel_line_2d_partial_0_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t)
{
    ex equation = (-2*line2_point2s + 2*line2_point1s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2))) + pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)*(-2*line1_point1s + 2*line1_point2s)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),2)*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)));
    return equation.evalf();
}
static ex parallel_line_2d_partial_1_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t)
{
    ex equation = (-2*line2_point2t + 2*line2_point1t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2))) + pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)*(-2*line1_point1t + 2*line1_point2t)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),2)*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)));
    return equation.evalf();
}
static ex parallel_line_2d_partial_2_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t)
{
    ex equation = (-2*line2_point1s + 2*line2_point2s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2))) + pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)*(-2*line1_point2s + 2*line1_point1s)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),2)*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)));
    return equation.evalf();
}
static ex parallel_line_2d_partial_3_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t)
{
    ex equation = (-2*line2_point1t + 2*line2_point2t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2))) + pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)*(-2*line1_point2t + 2*line1_point1t)/(pow((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2)),2)*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)));
    return equation.evalf();
}
static ex parallel_line_2d_partial_4_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t)
{
    ex equation = (-2*line1_point2s + 2*line1_point1s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2))) + pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)*(-2*line2_point1s + 2*line2_point2s)/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),2));
    return equation.evalf();
}
static ex parallel_line_2d_partial_5_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t)
{
    ex equation = (-2*line1_point2t + 2*line1_point1t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2))) + pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)*(-2*line2_point1t + 2*line2_point2t)/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),2));
    return equation.evalf();
}
static ex parallel_line_2d_partial_6_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t)
{
    ex equation = (-2*line1_point1s + 2*line1_point2s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2))) + pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)*(-2*line2_point2s + 2*line2_point1s)/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),2));
    return equation.evalf();
}
static ex parallel_line_2d_partial_7_evalf(const ex & line1_point1s, const ex & line1_point1t, const ex & line1_point2s, const ex & line1_point2t, const ex & line2_point1s, const ex & line2_point1t, const ex & line2_point2s, const ex & line2_point2t)
{
    ex equation = (-2*line1_point1t + 2*line1_point2t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*(pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2))) + pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)*(-2*line2_point2t + 2*line2_point1t)/((pow((line1_point1s - line1_point2s),2) + pow((line1_point1t - line1_point2t),2))*pow((pow((line2_point1s - line2_point2s),2) + pow((line2_point1t - line2_point2t),2)),2));
    return equation.evalf();
}
    



