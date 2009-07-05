#include <ginac/ginac.h>

DECLARE_FUNCTION_5P(distance_point_2d)
DECLARE_FUNCTION_5P(distance_point_2d_partial_0)
DECLARE_FUNCTION_5P(distance_point_2d_partial_1)
DECLARE_FUNCTION_5P(distance_point_2d_partial_2)
DECLARE_FUNCTION_5P(distance_point_2d_partial_3)
DECLARE_FUNCTION_5P(distance_point_2d_partial_4)

static ex distance_point_2d_evalf(const ex & point1s, const ex & point1t, const ex & point2s, const ex & point2t, const ex & distance);
static ex distance_point_2d_derivative(const ex & point1s, const ex & point1t, const ex & point2s, const ex & point2t, const ex & distance, unsigned diff_parm);
static ex distance_point_2d_partial_0_evalf(const ex & point1s, const ex & point1t, const ex & point2s, const ex & point2t, const ex & distance);
static ex distance_point_2d_partial_1_evalf(const ex & point1s, const ex & point1t, const ex & point2s, const ex & point2t, const ex & distance);
static ex distance_point_2d_partial_2_evalf(const ex & point1s, const ex & point1t, const ex & point2s, const ex & point2t, const ex & distance);
static ex distance_point_2d_partial_3_evalf(const ex & point1s, const ex & point1t, const ex & point2s, const ex & point2t, const ex & distance);
static ex distance_point_2d_partial_4_evalf(const ex & point1s, const ex & point1t, const ex & point2s, const ex & point2t, const ex & distance);


