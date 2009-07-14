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
DECLARE_FUNCTION_9P(angle_line_2d_interior)
DECLARE_FUNCTION_9P(angle_line_2d_interior_partial_0)
DECLARE_FUNCTION_9P(angle_line_2d_interior_partial_1)
DECLARE_FUNCTION_9P(angle_line_2d_interior_partial_2)
DECLARE_FUNCTION_9P(angle_line_2d_interior_partial_3)
DECLARE_FUNCTION_9P(angle_line_2d_interior_partial_4)
DECLARE_FUNCTION_9P(angle_line_2d_interior_partial_5)
DECLARE_FUNCTION_9P(angle_line_2d_interior_partial_6)
DECLARE_FUNCTION_9P(angle_line_2d_interior_partial_7)
DECLARE_FUNCTION_9P(angle_line_2d_interior_partial_8)

static GiNaC::ex angle_line_2d_interior_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t, const GiNaC::ex & angle);
static GiNaC::ex angle_line_2d_interior_derivative(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t, const GiNaC::ex & angle, unsigned diff_parm);
static GiNaC::ex angle_line_2d_interior_partial_0_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t, const GiNaC::ex & angle);
static GiNaC::ex angle_line_2d_interior_partial_1_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t, const GiNaC::ex & angle);
static GiNaC::ex angle_line_2d_interior_partial_2_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t, const GiNaC::ex & angle);
static GiNaC::ex angle_line_2d_interior_partial_3_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t, const GiNaC::ex & angle);
static GiNaC::ex angle_line_2d_interior_partial_4_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t, const GiNaC::ex & angle);
static GiNaC::ex angle_line_2d_interior_partial_5_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t, const GiNaC::ex & angle);
static GiNaC::ex angle_line_2d_interior_partial_6_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t, const GiNaC::ex & angle);
static GiNaC::ex angle_line_2d_interior_partial_7_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t, const GiNaC::ex & angle);
static GiNaC::ex angle_line_2d_interior_partial_8_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t, const GiNaC::ex & angle);
DECLARE_FUNCTION_9P(angle_line_2d_exterior)
DECLARE_FUNCTION_9P(angle_line_2d_exterior_partial_0)
DECLARE_FUNCTION_9P(angle_line_2d_exterior_partial_1)
DECLARE_FUNCTION_9P(angle_line_2d_exterior_partial_2)
DECLARE_FUNCTION_9P(angle_line_2d_exterior_partial_3)
DECLARE_FUNCTION_9P(angle_line_2d_exterior_partial_4)
DECLARE_FUNCTION_9P(angle_line_2d_exterior_partial_5)
DECLARE_FUNCTION_9P(angle_line_2d_exterior_partial_6)
DECLARE_FUNCTION_9P(angle_line_2d_exterior_partial_7)
DECLARE_FUNCTION_9P(angle_line_2d_exterior_partial_8)

static GiNaC::ex angle_line_2d_exterior_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t, const GiNaC::ex & angle);
static GiNaC::ex angle_line_2d_exterior_derivative(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t, const GiNaC::ex & angle, unsigned diff_parm);
static GiNaC::ex angle_line_2d_exterior_partial_0_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t, const GiNaC::ex & angle);
static GiNaC::ex angle_line_2d_exterior_partial_1_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t, const GiNaC::ex & angle);
static GiNaC::ex angle_line_2d_exterior_partial_2_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t, const GiNaC::ex & angle);
static GiNaC::ex angle_line_2d_exterior_partial_3_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t, const GiNaC::ex & angle);
static GiNaC::ex angle_line_2d_exterior_partial_4_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t, const GiNaC::ex & angle);
static GiNaC::ex angle_line_2d_exterior_partial_5_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t, const GiNaC::ex & angle);
static GiNaC::ex angle_line_2d_exterior_partial_6_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t, const GiNaC::ex & angle);
static GiNaC::ex angle_line_2d_exterior_partial_7_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t, const GiNaC::ex & angle);
static GiNaC::ex angle_line_2d_exterior_partial_8_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t, const GiNaC::ex & angle);
DECLARE_FUNCTION_4P(tangent_edge_2d)
DECLARE_FUNCTION_4P(tangent_edge_2d_partial_0)
DECLARE_FUNCTION_4P(tangent_edge_2d_partial_1)
DECLARE_FUNCTION_4P(tangent_edge_2d_partial_2)
DECLARE_FUNCTION_4P(tangent_edge_2d_partial_3)

static GiNaC::ex tangent_edge_2d_evalf(const GiNaC::ex & s1, const GiNaC::ex & t1, const GiNaC::ex & s2, const GiNaC::ex & t2);
static GiNaC::ex tangent_edge_2d_derivative(const GiNaC::ex & s1, const GiNaC::ex & t1, const GiNaC::ex & s2, const GiNaC::ex & t2, unsigned diff_parm);
static GiNaC::ex tangent_edge_2d_partial_0_evalf(const GiNaC::ex & s1, const GiNaC::ex & t1, const GiNaC::ex & s2, const GiNaC::ex & t2);
static GiNaC::ex tangent_edge_2d_partial_1_evalf(const GiNaC::ex & s1, const GiNaC::ex & t1, const GiNaC::ex & s2, const GiNaC::ex & t2);
static GiNaC::ex tangent_edge_2d_partial_2_evalf(const GiNaC::ex & s1, const GiNaC::ex & t1, const GiNaC::ex & s2, const GiNaC::ex & t2);
static GiNaC::ex tangent_edge_2d_partial_3_evalf(const GiNaC::ex & s1, const GiNaC::ex & t1, const GiNaC::ex & s2, const GiNaC::ex & t2);
DECLARE_FUNCTION_8P(parallel_line_2d)
DECLARE_FUNCTION_8P(parallel_line_2d_partial_0)
DECLARE_FUNCTION_8P(parallel_line_2d_partial_1)
DECLARE_FUNCTION_8P(parallel_line_2d_partial_2)
DECLARE_FUNCTION_8P(parallel_line_2d_partial_3)
DECLARE_FUNCTION_8P(parallel_line_2d_partial_4)
DECLARE_FUNCTION_8P(parallel_line_2d_partial_5)
DECLARE_FUNCTION_8P(parallel_line_2d_partial_6)
DECLARE_FUNCTION_8P(parallel_line_2d_partial_7)

static GiNaC::ex parallel_line_2d_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t);
static GiNaC::ex parallel_line_2d_derivative(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t, unsigned diff_parm);
static GiNaC::ex parallel_line_2d_partial_0_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t);
static GiNaC::ex parallel_line_2d_partial_1_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t);
static GiNaC::ex parallel_line_2d_partial_2_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t);
static GiNaC::ex parallel_line_2d_partial_3_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t);
static GiNaC::ex parallel_line_2d_partial_4_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t);
static GiNaC::ex parallel_line_2d_partial_5_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t);
static GiNaC::ex parallel_line_2d_partial_6_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t);
static GiNaC::ex parallel_line_2d_partial_7_evalf(const GiNaC::ex & line1_point1s, const GiNaC::ex & line1_point1t, const GiNaC::ex & line1_point2s, const GiNaC::ex & line1_point2t, const GiNaC::ex & line2_point1s, const GiNaC::ex & line2_point1t, const GiNaC::ex & line2_point2s, const GiNaC::ex & line2_point2t);


