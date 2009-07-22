/****************************************************************************
**
** This file is part of the pSketcher project.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Copyright (C) 2006-2009 Michael Greminger. All rights reserved.
**
****************************************************************************/

#ifndef SolverFunctionsH
#define SolverFunctionsH

#include <string>
#include <vector>
#include "SolverFunctionsBase.h"

SolverFunctionsBasePointer SolverFunctionsFactory(std::string solver_function_name, std::vector<DOFPointer> dof_list);


class distance_point_2d: public SolverFunctionsBase
{
    public:
        distance_point_2d(DOFPointer point1s, DOFPointer point1t, DOFPointer point2s, DOFPointer point2t, DOFPointer distance);
        distance_point_2d(std::vector<DOFPointer> dof_list);

        double GetValue() const;
        double GetValueSelf(const mmcMatrix &params) const;
        mmcMatrix GetGradientSelf(const mmcMatrix &params) const;
        std::string GetName() const {return "distance_point_2d";}
};

class angle_line_2d_interior: public SolverFunctionsBase
{
    public:
        angle_line_2d_interior(DOFPointer line1_point1s, DOFPointer line1_point1t, DOFPointer line1_point2s, DOFPointer line1_point2t, DOFPointer line2_point1s, DOFPointer line2_point1t, DOFPointer line2_point2s, DOFPointer line2_point2t, DOFPointer angle);
        angle_line_2d_interior(std::vector<DOFPointer> dof_list);

        double GetValue() const;
        double GetValueSelf(const mmcMatrix &params) const;
        mmcMatrix GetGradientSelf(const mmcMatrix &params) const;
        std::string GetName() const {return "angle_line_2d_interior";}
};

class angle_line_2d_exterior: public SolverFunctionsBase
{
    public:
        angle_line_2d_exterior(DOFPointer line1_point1s, DOFPointer line1_point1t, DOFPointer line1_point2s, DOFPointer line1_point2t, DOFPointer line2_point1s, DOFPointer line2_point1t, DOFPointer line2_point2s, DOFPointer line2_point2t, DOFPointer angle);
        angle_line_2d_exterior(std::vector<DOFPointer> dof_list);

        double GetValue() const;
        double GetValueSelf(const mmcMatrix &params) const;
        mmcMatrix GetGradientSelf(const mmcMatrix &params) const;
        std::string GetName() const {return "angle_line_2d_exterior";}
};

class tangent_edge_2d: public SolverFunctionsBase
{
    public:
        tangent_edge_2d(DOFPointer s1, DOFPointer t1, DOFPointer s2, DOFPointer t2);
        tangent_edge_2d(std::vector<DOFPointer> dof_list);

        double GetValue() const;
        double GetValueSelf(const mmcMatrix &params) const;
        mmcMatrix GetGradientSelf(const mmcMatrix &params) const;
        std::string GetName() const {return "tangent_edge_2d";}
};

class parallel_line_2d: public SolverFunctionsBase
{
    public:
        parallel_line_2d(DOFPointer line1_point1s, DOFPointer line1_point1t, DOFPointer line1_point2s, DOFPointer line1_point2t, DOFPointer line2_point1s, DOFPointer line2_point1t, DOFPointer line2_point2s, DOFPointer line2_point2t);
        parallel_line_2d(std::vector<DOFPointer> dof_list);

        double GetValue() const;
        double GetValueSelf(const mmcMatrix &params) const;
        mmcMatrix GetGradientSelf(const mmcMatrix &params) const;
        std::string GetName() const {return "parallel_line_2d";}
};

class arc2d_point_s: public SolverFunctionsBase
{
    public:
        arc2d_point_s(DOFPointer s_center, DOFPointer radius, DOFPointer theta);
        arc2d_point_s(std::vector<DOFPointer> dof_list);

        double GetValue() const;
        double GetValueSelf(const mmcMatrix &params) const;
        mmcMatrix GetGradientSelf(const mmcMatrix &params) const;
        std::string GetName() const {return "arc2d_point_s";}
};

class arc2d_point_t: public SolverFunctionsBase
{
    public:
        arc2d_point_t(DOFPointer t_center, DOFPointer radius, DOFPointer theta);
        arc2d_point_t(std::vector<DOFPointer> dof_list);

        double GetValue() const;
        double GetValueSelf(const mmcMatrix &params) const;
        mmcMatrix GetGradientSelf(const mmcMatrix &params) const;
        std::string GetName() const {return "arc2d_point_t";}
};

class arc2d_tangent_s: public SolverFunctionsBase
{
    public:
        arc2d_tangent_s(DOFPointer theta);
        arc2d_tangent_s(std::vector<DOFPointer> dof_list);

        double GetValue() const;
        double GetValueSelf(const mmcMatrix &params) const;
        mmcMatrix GetGradientSelf(const mmcMatrix &params) const;
        std::string GetName() const {return "arc2d_tangent_s";}
};

class arc2d_tangent_t: public SolverFunctionsBase
{
    public:
        arc2d_tangent_t(DOFPointer theta);
        arc2d_tangent_t(std::vector<DOFPointer> dof_list);

        double GetValue() const;
        double GetValueSelf(const mmcMatrix &params) const;
        mmcMatrix GetGradientSelf(const mmcMatrix &params) const;
        std::string GetName() const {return "arc2d_tangent_t";}
};

class point2d_tangent1_s: public SolverFunctionsBase
{
    public:
        point2d_tangent1_s(DOFPointer point1s, DOFPointer point1t, DOFPointer point2s, DOFPointer point2t);
        point2d_tangent1_s(std::vector<DOFPointer> dof_list);

        double GetValue() const;
        double GetValueSelf(const mmcMatrix &params) const;
        mmcMatrix GetGradientSelf(const mmcMatrix &params) const;
        std::string GetName() const {return "point2d_tangent1_s";}
};

class point2d_tangent1_t: public SolverFunctionsBase
{
    public:
        point2d_tangent1_t(DOFPointer point1s, DOFPointer point1t, DOFPointer point2s, DOFPointer point2t);
        point2d_tangent1_t(std::vector<DOFPointer> dof_list);

        double GetValue() const;
        double GetValueSelf(const mmcMatrix &params) const;
        mmcMatrix GetGradientSelf(const mmcMatrix &params) const;
        std::string GetName() const {return "point2d_tangent1_t";}
};

class point2d_tangent2_s: public SolverFunctionsBase
{
    public:
        point2d_tangent2_s(DOFPointer point1s, DOFPointer point1t, DOFPointer point2s, DOFPointer point2t);
        point2d_tangent2_s(std::vector<DOFPointer> dof_list);

        double GetValue() const;
        double GetValueSelf(const mmcMatrix &params) const;
        mmcMatrix GetGradientSelf(const mmcMatrix &params) const;
        std::string GetName() const {return "point2d_tangent2_s";}
};

class point2d_tangent2_t: public SolverFunctionsBase
{
    public:
        point2d_tangent2_t(DOFPointer point1s, DOFPointer point1t, DOFPointer point2s, DOFPointer point2t);
        point2d_tangent2_t(std::vector<DOFPointer> dof_list);

        double GetValue() const;
        double GetValueSelf(const mmcMatrix &params) const;
        mmcMatrix GetGradientSelf(const mmcMatrix &params) const;
        std::string GetName() const {return "point2d_tangent2_t";}
};

class distance_point_line_2d: public SolverFunctionsBase
{
    public:
        distance_point_line_2d(DOFPointer point_s, DOFPointer point_t, DOFPointer line_point1s, DOFPointer line_point1t, DOFPointer line_point2s, DOFPointer line_point2t, DOFPointer distance);
        distance_point_line_2d(std::vector<DOFPointer> dof_list);

        double GetValue() const;
        double GetValueSelf(const mmcMatrix &params) const;
        mmcMatrix GetGradientSelf(const mmcMatrix &params) const;
        std::string GetName() const {return "distance_point_line_2d";}
};

class hori_vert_2d: public SolverFunctionsBase
{
    public:
        hori_vert_2d(DOFPointer dof1, DOFPointer dof2);
        hori_vert_2d(std::vector<DOFPointer> dof_list);

        double GetValue() const;
        double GetValueSelf(const mmcMatrix &params) const;
        mmcMatrix GetGradientSelf(const mmcMatrix &params) const;
        std::string GetName() const {return "hori_vert_2d";}
};





#endif //SolverFunctionsH