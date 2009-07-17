#ifndef SolverFunctionsH
#define SolverFunctionsH

#include "SolverFunctionsBase.h"


class distance_point_2d: public SolverFunctionsBase
{
    public:
         distance_point_2d(DOFPointer point1s, DOFPointer point1t, DOFPointer point2s, DOFPointer point2t, DOFPointer distance);

        double GetValue() const;
        double GetValueSelf(const mmcMatrix &params) const;
        mmcMatrix GetGradientSelf(const mmcMatrix &params) const;
};

class angle_line_2d_interior: public SolverFunctionsBase
{
    public:
         angle_line_2d_interior(DOFPointer line1_point1s, DOFPointer line1_point1t, DOFPointer line1_point2s, DOFPointer line1_point2t, DOFPointer line2_point1s, DOFPointer line2_point1t, DOFPointer line2_point2s, DOFPointer line2_point2t, DOFPointer angle);

        double GetValue() const;
        double GetValueSelf(const mmcMatrix &params) const;
        mmcMatrix GetGradientSelf(const mmcMatrix &params) const;
};

class angle_line_2d_exterior: public SolverFunctionsBase
{
    public:
         angle_line_2d_exterior(DOFPointer line1_point1s, DOFPointer line1_point1t, DOFPointer line1_point2s, DOFPointer line1_point2t, DOFPointer line2_point1s, DOFPointer line2_point1t, DOFPointer line2_point2s, DOFPointer line2_point2t, DOFPointer angle);

        double GetValue() const;
        double GetValueSelf(const mmcMatrix &params) const;
        mmcMatrix GetGradientSelf(const mmcMatrix &params) const;
};

class tangent_edge_2d: public SolverFunctionsBase
{
    public:
         tangent_edge_2d(DOFPointer s1, DOFPointer t1, DOFPointer s2, DOFPointer t2);

        double GetValue() const;
        double GetValueSelf(const mmcMatrix &params) const;
        mmcMatrix GetGradientSelf(const mmcMatrix &params) const;
};

class parallel_line_2d: public SolverFunctionsBase
{
    public:
         parallel_line_2d(DOFPointer line1_point1s, DOFPointer line1_point1t, DOFPointer line1_point2s, DOFPointer line1_point2t, DOFPointer line2_point1s, DOFPointer line2_point1t, DOFPointer line2_point2s, DOFPointer line2_point2t);

        double GetValue() const;
        double GetValueSelf(const mmcMatrix &params) const;
        mmcMatrix GetGradientSelf(const mmcMatrix &params) const;
};



#endif //SolverFunctionsH