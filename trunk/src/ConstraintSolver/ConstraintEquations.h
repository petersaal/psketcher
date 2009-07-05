#include <ginac/ginac.h>

DECLARE_FUNCTION_5P(distance_point_2d)
DECLARE_FUNCTION_5P(distance_point_2d_partial_0)
DECLARE_FUNCTION_5P(distance_point_2d_partial_1)
DECLARE_FUNCTION_5P(distance_point_2d_partial_2)
DECLARE_FUNCTION_5P(distance_point_2d_partial_3)
DECLARE_FUNCTION_5P(distance_point_2d_partial_4)

static GiNaC::ex distance_point_2d_evalf(const GiNaC::ex & point1s, const GiNaC::ex & point1t, const GiNaC::ex & point2s, const GiNaC::ex & point2t, const GiNaC::ex & distance);
static GiNaC::ex distance_point_2d_derivative(const GiNaC::ex & point1s, const GiNaC::ex & point1t, const GiNaC::ex & point2s, const GiNaC::ex & point2t, const GiNaC::ex & distance, unsigned diff_parm);
static GiNaC::ex distance_point_2d_partial_0_evalf(const GiNaC::ex & point1s, const GiNaC::ex & point1t, const GiNaC::ex & point2s, const GiNaC::ex & point2t, const GiNaC::ex & distance);
static GiNaC::ex distance_point_2d_partial_1_evalf(const GiNaC::ex & point1s, const GiNaC::ex & point1t, const GiNaC::ex & point2s, const GiNaC::ex & point2t, const GiNaC::ex & distance);
static GiNaC::ex distance_point_2d_partial_2_evalf(const GiNaC::ex & point1s, const GiNaC::ex & point1t, const GiNaC::ex & point2s, const GiNaC::ex & point2t, const GiNaC::ex & distance);
static GiNaC::ex distance_point_2d_partial_3_evalf(const GiNaC::ex & point1s, const GiNaC::ex & point1t, const GiNaC::ex & point2s, const GiNaC::ex & point2t, const GiNaC::ex & distance);
static GiNaC::ex distance_point_2d_partial_4_evalf(const GiNaC::ex & point1s, const GiNaC::ex & point1t, const GiNaC::ex & point2s, const GiNaC::ex & point2t, const GiNaC::ex & distance);


