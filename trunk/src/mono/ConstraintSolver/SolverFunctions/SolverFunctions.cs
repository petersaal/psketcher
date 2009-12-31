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

using System;
using System.Collections.Generic;
using dnAnalytics.LinearAlgebra;



namespace pSketcher.ConstraintSolver.SolverFunctions
{
    static class Factory {
        public static SolverFunctionBase Create(string solver_function_name, Stack<DOF> dof_list)
        {
            SolverFunctionBase new_solver_function;

            if(solver_function_name == "distance_point_2d")
                new_solver_function = new distance_point_2d(dof_list);
            else if (solver_function_name == "angle_line_2d_interior")
                new_solver_function = new angle_line_2d_interior(dof_list);
            else if (solver_function_name == "angle_line_2d_exterior")
                new_solver_function = new angle_line_2d_exterior(dof_list);
            else if (solver_function_name == "tangent_edge_2d")
                new_solver_function = new tangent_edge_2d(dof_list);
            else if (solver_function_name == "parallel_line_2d")
                new_solver_function = new parallel_line_2d(dof_list);
            else if (solver_function_name == "arc2d_point_s")
                new_solver_function = new arc2d_point_s(dof_list);
            else if (solver_function_name == "arc2d_point_t")
                new_solver_function = new arc2d_point_t(dof_list);
            else if (solver_function_name == "arc2d_tangent_s")
                new_solver_function = new arc2d_tangent_s(dof_list);
            else if (solver_function_name == "arc2d_tangent_t")
                new_solver_function = new arc2d_tangent_t(dof_list);
            else if (solver_function_name == "point2d_tangent1_s")
                new_solver_function = new point2d_tangent1_s(dof_list);
            else if (solver_function_name == "point2d_tangent1_t")
                new_solver_function = new point2d_tangent1_t(dof_list);
            else if (solver_function_name == "point2d_tangent2_s")
                new_solver_function = new point2d_tangent2_s(dof_list);
            else if (solver_function_name == "point2d_tangent2_t")
                new_solver_function = new point2d_tangent2_t(dof_list);
            else if (solver_function_name == "distance_point_line_2d")
                new_solver_function = new distance_point_line_2d(dof_list);
            else if (solver_function_name == "hori_vert_2d")
                new_solver_function = new hori_vert_2d(dof_list);
            else
                throw new ArgumentException("Factory.Create: The requested solver function name was not found.", "solver_function_name");

            return new_solver_function;
        }
    }


    class distance_point_2d : SolverFunctionBase {

        public distance_point_2d(DOF point1s, DOF point1t, DOF point2s, DOF point2t, DOF distance)
        {
            AddDOF(point1s);
            AddDOF(point1t);
            AddDOF(point2s);
            AddDOF(point2t);
            AddDOF(distance);
        }

        public distance_point_2d(Stack<DOF> dof_list)
        {
            // Check to make sure the correct number of parameters have been provided
            if(dof_list.Count != 5)
                throw new ArgumentException("The DOF list for the constructor of SolverFunction distance_point_2d did not contain exactly 5 items.", "dof_list");

            foreach (DOF item in dof_list)
                AddDOF(item);
        }

        public override string Name {
            get { return "distance_point_2d" ;}
        }

        public override double GetValue()
        {
            double point1s = GetDOF(0).Value;
            double point1t = GetDOF(1).Value;
            double point2s = GetDOF(2).Value;
            double point2t = GetDOF(3).Value;
            double distance = GetDOF(4).Value;

            return -distance + Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(1.0/2.0));
        }

        public override double GetValueSelf(Vector input)
        {
            double point1s = input[0];
            double point1t = input[1];
            double point2s = input[2];
            double point2t = input[3];
            double distance = input[4];

            return -distance + Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(1.0/2.0));
        }

        public override Vector GetGradientSelf(Vector input)
        {
            #pragma warning disable 169,219

            Vector result = new DenseVector(GetNumDOFs(),1);

            double point1s = input[0];
            double point1t = input[1];
            double point2s = input[2];
            double point2t = input[3];
            double distance = input[4];

            result[0] = (point1s - point2s)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(1.0/2.0));
            result[1] = (point1t - point2t)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(1.0/2.0));
            result[2] = (point2s - point1s)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(1.0/2.0));
            result[3] = (point2t - point1t)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(1.0/2.0));
            result[4] = -1;

            return result;

            #pragma warning restore 169,219
        }

    } // class distance_point_2d

    class angle_line_2d_interior : SolverFunctionBase {

        public angle_line_2d_interior(DOF line1_point1s, DOF line1_point1t, DOF line1_point2s, DOF line1_point2t, DOF line2_point1s, DOF line2_point1t, DOF line2_point2s, DOF line2_point2t, DOF angle)
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

        public angle_line_2d_interior(Stack<DOF> dof_list)
        {
            // Check to make sure the correct number of parameters have been provided
            if(dof_list.Count != 9)
                throw new ArgumentException("The DOF list for the constructor of SolverFunction angle_line_2d_interior did not contain exactly 9 items.", "dof_list");

            foreach (DOF item in dof_list)
                AddDOF(item);
        }

        public override string Name {
            get { return "angle_line_2d_interior" ;}
        }

        public override double GetValue()
        {
            double line1_point1s = GetDOF(0).Value;
            double line1_point1t = GetDOF(1).Value;
            double line1_point2s = GetDOF(2).Value;
            double line1_point2t = GetDOF(3).Value;
            double line2_point1s = GetDOF(4).Value;
            double line2_point1t = GetDOF(5).Value;
            double line2_point2s = GetDOF(6).Value;
            double line2_point2t = GetDOF(7).Value;
            double angle = GetDOF(8).Value;

            return -Math.Cos(angle) + ((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
        }

        public override double GetValueSelf(Vector input)
        {
            double line1_point1s = input[0];
            double line1_point1t = input[1];
            double line1_point2s = input[2];
            double line1_point2t = input[3];
            double line2_point1s = input[4];
            double line2_point1t = input[5];
            double line2_point2s = input[6];
            double line2_point2t = input[7];
            double angle = input[8];

            return -Math.Cos(angle) + ((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
        }

        public override Vector GetGradientSelf(Vector input)
        {
            #pragma warning disable 169,219

            Vector result = new DenseVector(GetNumDOFs(),1);

            double line1_point1s = input[0];
            double line1_point1t = input[1];
            double line1_point2s = input[2];
            double line1_point2t = input[3];
            double line2_point1s = input[4];
            double line2_point1t = input[5];
            double line2_point2s = input[6];
            double line2_point2t = input[7];
            double angle = input[8];

            result[0] = (line2_point1s - line2_point2s)/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line1_point2s - line1_point1s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(3.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
            result[1] = (line2_point1t - line2_point2t)/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line1_point2t - line1_point1t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(3.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
            result[2] = (line2_point2s - line2_point1s)/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line1_point1s - line1_point2s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(3.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
            result[3] = (line2_point2t - line2_point1t)/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line1_point1t - line1_point2t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(3.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
            result[4] = (line1_point1s - line1_point2s)/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line2_point2s - line2_point1s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(3.0/2.0)));
            result[5] = (line1_point1t - line1_point2t)/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line2_point2t - line2_point1t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(3.0/2.0)));
            result[6] = (line1_point2s - line1_point1s)/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line2_point1s - line2_point2s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(3.0/2.0)));
            result[7] = (line1_point2t - line1_point1t)/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line2_point1t - line2_point2t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(3.0/2.0)));
            result[8] = Math.Sin(angle);

            return result;

            #pragma warning restore 169,219
        }

    } // class angle_line_2d_interior

    class angle_line_2d_exterior : SolverFunctionBase {

        public angle_line_2d_exterior(DOF line1_point1s, DOF line1_point1t, DOF line1_point2s, DOF line1_point2t, DOF line2_point1s, DOF line2_point1t, DOF line2_point2s, DOF line2_point2t, DOF angle)
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

        public angle_line_2d_exterior(Stack<DOF> dof_list)
        {
            // Check to make sure the correct number of parameters have been provided
            if(dof_list.Count != 9)
                throw new ArgumentException("The DOF list for the constructor of SolverFunction angle_line_2d_exterior did not contain exactly 9 items.", "dof_list");

            foreach (DOF item in dof_list)
                AddDOF(item);
        }

        public override string Name {
            get { return "angle_line_2d_exterior" ;}
        }

        public override double GetValue()
        {
            double line1_point1s = GetDOF(0).Value;
            double line1_point1t = GetDOF(1).Value;
            double line1_point2s = GetDOF(2).Value;
            double line1_point2t = GetDOF(3).Value;
            double line2_point1s = GetDOF(4).Value;
            double line2_point1t = GetDOF(5).Value;
            double line2_point2s = GetDOF(6).Value;
            double line2_point2t = GetDOF(7).Value;
            double angle = GetDOF(8).Value;

            return ((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + Math.Cos(angle);
        }

        public override double GetValueSelf(Vector input)
        {
            double line1_point1s = input[0];
            double line1_point1t = input[1];
            double line1_point2s = input[2];
            double line1_point2t = input[3];
            double line2_point1s = input[4];
            double line2_point1t = input[5];
            double line2_point2s = input[6];
            double line2_point2t = input[7];
            double angle = input[8];

            return ((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + Math.Cos(angle);
        }

        public override Vector GetGradientSelf(Vector input)
        {
            #pragma warning disable 169,219

            Vector result = new DenseVector(GetNumDOFs(),1);

            double line1_point1s = input[0];
            double line1_point1t = input[1];
            double line1_point2s = input[2];
            double line1_point2t = input[3];
            double line2_point1s = input[4];
            double line2_point1t = input[5];
            double line2_point2s = input[6];
            double line2_point2t = input[7];
            double angle = input[8];

            result[0] = (line2_point1s - line2_point2s)/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line1_point2s - line1_point1s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(3.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
            result[1] = (line2_point1t - line2_point2t)/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line1_point2t - line1_point1t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(3.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
            result[2] = (line2_point2s - line2_point1s)/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line1_point1s - line1_point2s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(3.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
            result[3] = (line2_point2t - line2_point1t)/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line1_point1t - line1_point2t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(3.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0)));
            result[4] = (line1_point1s - line1_point2s)/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line2_point2s - line2_point1s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(3.0/2.0)));
            result[5] = (line1_point1t - line1_point2t)/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line2_point2t - line2_point1t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(3.0/2.0)));
            result[6] = (line1_point2s - line1_point1s)/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line2_point1s - line2_point2s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(3.0/2.0)));
            result[7] = (line1_point2t - line1_point1t)/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(1.0/2.0))) + (line2_point1t - line2_point2t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),(1.0/2.0))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),(3.0/2.0)));
            result[8] = -Math.Sin(angle);

            return result;

            #pragma warning restore 169,219
        }

    } // class angle_line_2d_exterior

    class tangent_edge_2d : SolverFunctionBase {

        public tangent_edge_2d(DOF s1, DOF t1, DOF s2, DOF t2)
        {
            AddDOF(s1);
            AddDOF(t1);
            AddDOF(s2);
            AddDOF(t2);
        }

        public tangent_edge_2d(Stack<DOF> dof_list)
        {
            // Check to make sure the correct number of parameters have been provided
            if(dof_list.Count != 4)
                throw new ArgumentException("The DOF list for the constructor of SolverFunction tangent_edge_2d did not contain exactly 4 items.", "dof_list");

            foreach (DOF item in dof_list)
                AddDOF(item);
        }

        public override string Name {
            get { return "tangent_edge_2d" ;}
        }

        public override double GetValue()
        {
            double s1 = GetDOF(0).Value;
            double t1 = GetDOF(1).Value;
            double s2 = GetDOF(2).Value;
            double t2 = GetDOF(3).Value;

            return -1 + Math.Pow((s1*s2 + t1*t2),2);
        }

        public override double GetValueSelf(Vector input)
        {
            double s1 = input[0];
            double t1 = input[1];
            double s2 = input[2];
            double t2 = input[3];

            return -1 + Math.Pow((s1*s2 + t1*t2),2);
        }

        public override Vector GetGradientSelf(Vector input)
        {
            #pragma warning disable 169,219

            Vector result = new DenseVector(GetNumDOFs(),1);

            double s1 = input[0];
            double t1 = input[1];
            double s2 = input[2];
            double t2 = input[3];

            result[0] = 2*s2*(s1*s2 + t1*t2);
            result[1] = 2*t2*(s1*s2 + t1*t2);
            result[2] = 2*s1*(s1*s2 + t1*t2);
            result[3] = 2*t1*(s1*s2 + t1*t2);

            return result;

            #pragma warning restore 169,219
        }

    } // class tangent_edge_2d

    class parallel_line_2d : SolverFunctionBase {

        public parallel_line_2d(DOF line1_point1s, DOF line1_point1t, DOF line1_point2s, DOF line1_point2t, DOF line2_point1s, DOF line2_point1t, DOF line2_point2s, DOF line2_point2t)
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

        public parallel_line_2d(Stack<DOF> dof_list)
        {
            // Check to make sure the correct number of parameters have been provided
            if(dof_list.Count != 8)
                throw new ArgumentException("The DOF list for the constructor of SolverFunction parallel_line_2d did not contain exactly 8 items.", "dof_list");

            foreach (DOF item in dof_list)
                AddDOF(item);
        }

        public override string Name {
            get { return "parallel_line_2d" ;}
        }

        public override double GetValue()
        {
            double line1_point1s = GetDOF(0).Value;
            double line1_point1t = GetDOF(1).Value;
            double line1_point2s = GetDOF(2).Value;
            double line1_point2t = GetDOF(3).Value;
            double line2_point1s = GetDOF(4).Value;
            double line2_point1t = GetDOF(5).Value;
            double line2_point2s = GetDOF(6).Value;
            double line2_point2t = GetDOF(7).Value;

            return -1 + Math.Pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)/((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2))*(Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)));
        }

        public override double GetValueSelf(Vector input)
        {
            double line1_point1s = input[0];
            double line1_point1t = input[1];
            double line1_point2s = input[2];
            double line1_point2t = input[3];
            double line2_point1s = input[4];
            double line2_point1t = input[5];
            double line2_point2s = input[6];
            double line2_point2t = input[7];

            return -1 + Math.Pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)/((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2))*(Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)));
        }

        public override Vector GetGradientSelf(Vector input)
        {
            #pragma warning disable 169,219

            Vector result = new DenseVector(GetNumDOFs(),1);

            double line1_point1s = input[0];
            double line1_point1t = input[1];
            double line1_point2s = input[2];
            double line1_point2t = input[3];
            double line2_point1s = input[4];
            double line2_point1t = input[5];
            double line2_point2s = input[6];
            double line2_point2t = input[7];

            result[0] = (-2*line2_point2s + 2*line2_point1s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2))*(Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2))) + Math.Pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)*(-2*line1_point1s + 2*line1_point2s)/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),2)*(Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)));
            result[1] = (-2*line2_point2t + 2*line2_point1t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2))*(Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2))) + Math.Pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)*(-2*line1_point1t + 2*line1_point2t)/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),2)*(Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)));
            result[2] = (-2*line2_point1s + 2*line2_point2s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2))*(Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2))) + Math.Pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)*(-2*line1_point2s + 2*line1_point1s)/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),2)*(Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)));
            result[3] = (-2*line2_point1t + 2*line2_point2t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2))*(Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2))) + Math.Pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)*(-2*line1_point2t + 2*line1_point1t)/(Math.Pow((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2)),2)*(Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)));
            result[4] = (-2*line1_point2s + 2*line1_point1s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2))*(Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2))) + Math.Pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)*(-2*line2_point1s + 2*line2_point2s)/((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),2));
            result[5] = (-2*line1_point2t + 2*line1_point1t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2))*(Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2))) + Math.Pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)*(-2*line2_point1t + 2*line2_point2t)/((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),2));
            result[6] = (-2*line1_point1s + 2*line1_point2s)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2))*(Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2))) + Math.Pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)*(-2*line2_point2s + 2*line2_point1s)/((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),2));
            result[7] = (-2*line1_point1t + 2*line1_point2t)*((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t))/((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2))*(Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2))) + Math.Pow(((line1_point1s - line1_point2s)*(line2_point1s - line2_point2s) + (line1_point1t - line1_point2t)*(line2_point1t - line2_point2t)),2)*(-2*line2_point2t + 2*line2_point1t)/((Math.Pow((line1_point1s - line1_point2s),2) + Math.Pow((line1_point1t - line1_point2t),2))*Math.Pow((Math.Pow((line2_point1s - line2_point2s),2) + Math.Pow((line2_point1t - line2_point2t),2)),2));

            return result;

            #pragma warning restore 169,219
        }

    } // class parallel_line_2d

    class arc2d_point_s : SolverFunctionBase {

        public arc2d_point_s(DOF s_center, DOF radius, DOF theta)
        {
            AddDOF(s_center);
            AddDOF(radius);
            AddDOF(theta);
        }

        public arc2d_point_s(Stack<DOF> dof_list)
        {
            // Check to make sure the correct number of parameters have been provided
            if(dof_list.Count != 3)
                throw new ArgumentException("The DOF list for the constructor of SolverFunction arc2d_point_s did not contain exactly 3 items.", "dof_list");

            foreach (DOF item in dof_list)
                AddDOF(item);
        }

        public override string Name {
            get { return "arc2d_point_s" ;}
        }

        public override double GetValue()
        {
            double s_center = GetDOF(0).Value;
            double radius = GetDOF(1).Value;
            double theta = GetDOF(2).Value;

            return s_center + radius*Math.Cos(theta);
        }

        public override double GetValueSelf(Vector input)
        {
            double s_center = input[0];
            double radius = input[1];
            double theta = input[2];

            return s_center + radius*Math.Cos(theta);
        }

        public override Vector GetGradientSelf(Vector input)
        {
            #pragma warning disable 169,219

            Vector result = new DenseVector(GetNumDOFs(),1);

            double s_center = input[0];
            double radius = input[1];
            double theta = input[2];

            result[0] = 1;
            result[1] = Math.Cos(theta);
            result[2] = -radius*Math.Sin(theta);

            return result;

            #pragma warning restore 169,219
        }

    } // class arc2d_point_s

    class arc2d_point_t : SolverFunctionBase {

        public arc2d_point_t(DOF t_center, DOF radius, DOF theta)
        {
            AddDOF(t_center);
            AddDOF(radius);
            AddDOF(theta);
        }

        public arc2d_point_t(Stack<DOF> dof_list)
        {
            // Check to make sure the correct number of parameters have been provided
            if(dof_list.Count != 3)
                throw new ArgumentException("The DOF list for the constructor of SolverFunction arc2d_point_t did not contain exactly 3 items.", "dof_list");

            foreach (DOF item in dof_list)
                AddDOF(item);
        }

        public override string Name {
            get { return "arc2d_point_t" ;}
        }

        public override double GetValue()
        {
            double t_center = GetDOF(0).Value;
            double radius = GetDOF(1).Value;
            double theta = GetDOF(2).Value;

            return t_center + radius*Math.Sin(theta);
        }

        public override double GetValueSelf(Vector input)
        {
            double t_center = input[0];
            double radius = input[1];
            double theta = input[2];

            return t_center + radius*Math.Sin(theta);
        }

        public override Vector GetGradientSelf(Vector input)
        {
            #pragma warning disable 169,219

            Vector result = new DenseVector(GetNumDOFs(),1);

            double t_center = input[0];
            double radius = input[1];
            double theta = input[2];

            result[0] = 1;
            result[1] = Math.Sin(theta);
            result[2] = radius*Math.Cos(theta);

            return result;

            #pragma warning restore 169,219
        }

    } // class arc2d_point_t

    class arc2d_tangent_s : SolverFunctionBase {

        public arc2d_tangent_s(DOF theta)
        {
            AddDOF(theta);
        }

        public arc2d_tangent_s(Stack<DOF> dof_list)
        {
            // Check to make sure the correct number of parameters have been provided
            if(dof_list.Count != 1)
                throw new ArgumentException("The DOF list for the constructor of SolverFunction arc2d_tangent_s did not contain exactly 1 items.", "dof_list");

            foreach (DOF item in dof_list)
                AddDOF(item);
        }

        public override string Name {
            get { return "arc2d_tangent_s" ;}
        }

        public override double GetValue()
        {
            double theta = GetDOF(0).Value;

            return Math.Sin(theta);
        }

        public override double GetValueSelf(Vector input)
        {
            double theta = input[0];

            return Math.Sin(theta);
        }

        public override Vector GetGradientSelf(Vector input)
        {
            #pragma warning disable 169,219

            Vector result = new DenseVector(GetNumDOFs(),1);

            double theta = input[0];

            result[0] = Math.Cos(theta);

            return result;

            #pragma warning restore 169,219
        }

    } // class arc2d_tangent_s

    class arc2d_tangent_t : SolverFunctionBase {

        public arc2d_tangent_t(DOF theta)
        {
            AddDOF(theta);
        }

        public arc2d_tangent_t(Stack<DOF> dof_list)
        {
            // Check to make sure the correct number of parameters have been provided
            if(dof_list.Count != 1)
                throw new ArgumentException("The DOF list for the constructor of SolverFunction arc2d_tangent_t did not contain exactly 1 items.", "dof_list");

            foreach (DOF item in dof_list)
                AddDOF(item);
        }

        public override string Name {
            get { return "arc2d_tangent_t" ;}
        }

        public override double GetValue()
        {
            double theta = GetDOF(0).Value;

            return -Math.Cos(theta);
        }

        public override double GetValueSelf(Vector input)
        {
            double theta = input[0];

            return -Math.Cos(theta);
        }

        public override Vector GetGradientSelf(Vector input)
        {
            #pragma warning disable 169,219

            Vector result = new DenseVector(GetNumDOFs(),1);

            double theta = input[0];

            result[0] = Math.Sin(theta);

            return result;

            #pragma warning restore 169,219
        }

    } // class arc2d_tangent_t

    class point2d_tangent1_s : SolverFunctionBase {

        public point2d_tangent1_s(DOF point1s, DOF point1t, DOF point2s, DOF point2t)
        {
            AddDOF(point1s);
            AddDOF(point1t);
            AddDOF(point2s);
            AddDOF(point2t);
        }

        public point2d_tangent1_s(Stack<DOF> dof_list)
        {
            // Check to make sure the correct number of parameters have been provided
            if(dof_list.Count != 4)
                throw new ArgumentException("The DOF list for the constructor of SolverFunction point2d_tangent1_s did not contain exactly 4 items.", "dof_list");

            foreach (DOF item in dof_list)
                AddDOF(item);
        }

        public override string Name {
            get { return "point2d_tangent1_s" ;}
        }

        public override double GetValue()
        {
            double point1s = GetDOF(0).Value;
            double point1t = GetDOF(1).Value;
            double point2s = GetDOF(2).Value;
            double point2t = GetDOF(3).Value;

            return (point1s - point2s)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(1.0/2.0));
        }

        public override double GetValueSelf(Vector input)
        {
            double point1s = input[0];
            double point1t = input[1];
            double point2s = input[2];
            double point2t = input[3];

            return (point1s - point2s)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(1.0/2.0));
        }

        public override Vector GetGradientSelf(Vector input)
        {
            #pragma warning disable 169,219

            Vector result = new DenseVector(GetNumDOFs(),1);

            double point1s = input[0];
            double point1t = input[1];
            double point2s = input[2];
            double point2t = input[3];

            result[0] = (point2s - point1s)*(point1s - point2s)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(3.0/2.0)) + Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(-1.0/2.0));
            result[1] = (point2t - point1t)*(point1s - point2s)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(3.0/2.0));
            result[2] = Math.Pow((point1s - point2s),2)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(3.0/2.0)) - 1/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(1.0/2.0));
            result[3] = (point1s - point2s)*(point1t - point2t)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(3.0/2.0));

            return result;

            #pragma warning restore 169,219
        }

    } // class point2d_tangent1_s

    class point2d_tangent1_t : SolverFunctionBase {

        public point2d_tangent1_t(DOF point1s, DOF point1t, DOF point2s, DOF point2t)
        {
            AddDOF(point1s);
            AddDOF(point1t);
            AddDOF(point2s);
            AddDOF(point2t);
        }

        public point2d_tangent1_t(Stack<DOF> dof_list)
        {
            // Check to make sure the correct number of parameters have been provided
            if(dof_list.Count != 4)
                throw new ArgumentException("The DOF list for the constructor of SolverFunction point2d_tangent1_t did not contain exactly 4 items.", "dof_list");

            foreach (DOF item in dof_list)
                AddDOF(item);
        }

        public override string Name {
            get { return "point2d_tangent1_t" ;}
        }

        public override double GetValue()
        {
            double point1s = GetDOF(0).Value;
            double point1t = GetDOF(1).Value;
            double point2s = GetDOF(2).Value;
            double point2t = GetDOF(3).Value;

            return (point1t - point2t)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(1.0/2.0));
        }

        public override double GetValueSelf(Vector input)
        {
            double point1s = input[0];
            double point1t = input[1];
            double point2s = input[2];
            double point2t = input[3];

            return (point1t - point2t)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(1.0/2.0));
        }

        public override Vector GetGradientSelf(Vector input)
        {
            #pragma warning disable 169,219

            Vector result = new DenseVector(GetNumDOFs(),1);

            double point1s = input[0];
            double point1t = input[1];
            double point2s = input[2];
            double point2t = input[3];

            result[0] = (point2s - point1s)*(point1t - point2t)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(3.0/2.0));
            result[1] = (point2t - point1t)*(point1t - point2t)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(3.0/2.0)) + Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(-1.0/2.0));
            result[2] = (point1s - point2s)*(point1t - point2t)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(3.0/2.0));
            result[3] = Math.Pow((point1t - point2t),2)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(3.0/2.0)) - 1/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(1.0/2.0));

            return result;

            #pragma warning restore 169,219
        }

    } // class point2d_tangent1_t

    class point2d_tangent2_s : SolverFunctionBase {

        public point2d_tangent2_s(DOF point1s, DOF point1t, DOF point2s, DOF point2t)
        {
            AddDOF(point1s);
            AddDOF(point1t);
            AddDOF(point2s);
            AddDOF(point2t);
        }

        public point2d_tangent2_s(Stack<DOF> dof_list)
        {
            // Check to make sure the correct number of parameters have been provided
            if(dof_list.Count != 4)
                throw new ArgumentException("The DOF list for the constructor of SolverFunction point2d_tangent2_s did not contain exactly 4 items.", "dof_list");

            foreach (DOF item in dof_list)
                AddDOF(item);
        }

        public override string Name {
            get { return "point2d_tangent2_s" ;}
        }

        public override double GetValue()
        {
            double point1s = GetDOF(0).Value;
            double point1t = GetDOF(1).Value;
            double point2s = GetDOF(2).Value;
            double point2t = GetDOF(3).Value;

            return (point2s - point1s)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(1.0/2.0));
        }

        public override double GetValueSelf(Vector input)
        {
            double point1s = input[0];
            double point1t = input[1];
            double point2s = input[2];
            double point2t = input[3];

            return (point2s - point1s)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(1.0/2.0));
        }

        public override Vector GetGradientSelf(Vector input)
        {
            #pragma warning disable 169,219

            Vector result = new DenseVector(GetNumDOFs(),1);

            double point1s = input[0];
            double point1t = input[1];
            double point2s = input[2];
            double point2t = input[3];

            result[0] = Math.Pow((point2s - point1s),2)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(3.0/2.0)) - 1/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(1.0/2.0));
            result[1] = (point2s - point1s)*(point2t - point1t)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(3.0/2.0));
            result[2] = (point2s - point1s)*(point1s - point2s)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(3.0/2.0)) + Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(-1.0/2.0));
            result[3] = (point2s - point1s)*(point1t - point2t)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(3.0/2.0));

            return result;

            #pragma warning restore 169,219
        }

    } // class point2d_tangent2_s

    class point2d_tangent2_t : SolverFunctionBase {

        public point2d_tangent2_t(DOF point1s, DOF point1t, DOF point2s, DOF point2t)
        {
            AddDOF(point1s);
            AddDOF(point1t);
            AddDOF(point2s);
            AddDOF(point2t);
        }

        public point2d_tangent2_t(Stack<DOF> dof_list)
        {
            // Check to make sure the correct number of parameters have been provided
            if(dof_list.Count != 4)
                throw new ArgumentException("The DOF list for the constructor of SolverFunction point2d_tangent2_t did not contain exactly 4 items.", "dof_list");

            foreach (DOF item in dof_list)
                AddDOF(item);
        }

        public override string Name {
            get { return "point2d_tangent2_t" ;}
        }

        public override double GetValue()
        {
            double point1s = GetDOF(0).Value;
            double point1t = GetDOF(1).Value;
            double point2s = GetDOF(2).Value;
            double point2t = GetDOF(3).Value;

            return (point2t - point1t)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(1.0/2.0));
        }

        public override double GetValueSelf(Vector input)
        {
            double point1s = input[0];
            double point1t = input[1];
            double point2s = input[2];
            double point2t = input[3];

            return (point2t - point1t)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(1.0/2.0));
        }

        public override Vector GetGradientSelf(Vector input)
        {
            #pragma warning disable 169,219

            Vector result = new DenseVector(GetNumDOFs(),1);

            double point1s = input[0];
            double point1t = input[1];
            double point2s = input[2];
            double point2t = input[3];

            result[0] = (point2s - point1s)*(point2t - point1t)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(3.0/2.0));
            result[1] = Math.Pow((point2t - point1t),2)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(3.0/2.0)) - 1/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(1.0/2.0));
            result[2] = (point2t - point1t)*(point1s - point2s)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(3.0/2.0));
            result[3] = (point2t - point1t)*(point1t - point2t)/Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(3.0/2.0)) + Math.Pow((Math.Pow((point1s - point2s),2) + Math.Pow((point1t - point2t),2)),(-1.0/2.0));

            return result;

            #pragma warning restore 169,219
        }

    } // class point2d_tangent2_t

    class distance_point_line_2d : SolverFunctionBase {

        public distance_point_line_2d(DOF point_s, DOF point_t, DOF line_point1s, DOF line_point1t, DOF line_point2s, DOF line_point2t, DOF distance)
        {
            AddDOF(point_s);
            AddDOF(point_t);
            AddDOF(line_point1s);
            AddDOF(line_point1t);
            AddDOF(line_point2s);
            AddDOF(line_point2t);
            AddDOF(distance);
        }

        public distance_point_line_2d(Stack<DOF> dof_list)
        {
            // Check to make sure the correct number of parameters have been provided
            if(dof_list.Count != 7)
                throw new ArgumentException("The DOF list for the constructor of SolverFunction distance_point_line_2d did not contain exactly 7 items.", "dof_list");

            foreach (DOF item in dof_list)
                AddDOF(item);
        }

        public override string Name {
            get { return "distance_point_line_2d" ;}
        }

        public override double GetValue()
        {
            double point_s = GetDOF(0).Value;
            double point_t = GetDOF(1).Value;
            double line_point1s = GetDOF(2).Value;
            double line_point1t = GetDOF(3).Value;
            double line_point2s = GetDOF(4).Value;
            double line_point2t = GetDOF(5).Value;
            double distance = GetDOF(6).Value;

            return -Math.Pow(distance,2) + Math.Pow(((line_point1t - point_t)*(line_point2s - line_point1s) - (line_point1s - point_s)*(line_point2t - line_point1t)),2)/(Math.Pow((line_point2s - line_point1s),2) + Math.Pow((line_point2t - line_point1t),2));
        }

        public override double GetValueSelf(Vector input)
        {
            double point_s = input[0];
            double point_t = input[1];
            double line_point1s = input[2];
            double line_point1t = input[3];
            double line_point2s = input[4];
            double line_point2t = input[5];
            double distance = input[6];

            return -Math.Pow(distance,2) + Math.Pow(((line_point1t - point_t)*(line_point2s - line_point1s) - (line_point1s - point_s)*(line_point2t - line_point1t)),2)/(Math.Pow((line_point2s - line_point1s),2) + Math.Pow((line_point2t - line_point1t),2));
        }

        public override Vector GetGradientSelf(Vector input)
        {
            #pragma warning disable 169,219

            Vector result = new DenseVector(GetNumDOFs(),1);

            double point_s = input[0];
            double point_t = input[1];
            double line_point1s = input[2];
            double line_point1t = input[3];
            double line_point2s = input[4];
            double line_point2t = input[5];
            double distance = input[6];

            result[0] = (-2*line_point1t + 2*line_point2t)*((line_point1t - point_t)*(line_point2s - line_point1s) - (line_point1s - point_s)*(line_point2t - line_point1t))/(Math.Pow((line_point2s - line_point1s),2) + Math.Pow((line_point2t - line_point1t),2));
            result[1] = (-2*line_point2s + 2*line_point1s)*((line_point1t - point_t)*(line_point2s - line_point1s) - (line_point1s - point_s)*(line_point2t - line_point1t))/(Math.Pow((line_point2s - line_point1s),2) + Math.Pow((line_point2t - line_point1t),2));
            result[2] = (-2*line_point2t + 2*point_t)*((line_point1t - point_t)*(line_point2s - line_point1s) - (line_point1s - point_s)*(line_point2t - line_point1t))/(Math.Pow((line_point2s - line_point1s),2) + Math.Pow((line_point2t - line_point1t),2)) + Math.Pow(((line_point1t - point_t)*(line_point2s - line_point1s) - (line_point1s - point_s)*(line_point2t - line_point1t)),2)*(-2*line_point1s + 2*line_point2s)/Math.Pow((Math.Pow((line_point2s - line_point1s),2) + Math.Pow((line_point2t - line_point1t),2)),2);
            result[3] = (-2*point_s + 2*line_point2s)*((line_point1t - point_t)*(line_point2s - line_point1s) - (line_point1s - point_s)*(line_point2t - line_point1t))/(Math.Pow((line_point2s - line_point1s),2) + Math.Pow((line_point2t - line_point1t),2)) + Math.Pow(((line_point1t - point_t)*(line_point2s - line_point1s) - (line_point1s - point_s)*(line_point2t - line_point1t)),2)*(-2*line_point1t + 2*line_point2t)/Math.Pow((Math.Pow((line_point2s - line_point1s),2) + Math.Pow((line_point2t - line_point1t),2)),2);
            result[4] = (-2*point_t + 2*line_point1t)*((line_point1t - point_t)*(line_point2s - line_point1s) - (line_point1s - point_s)*(line_point2t - line_point1t))/(Math.Pow((line_point2s - line_point1s),2) + Math.Pow((line_point2t - line_point1t),2)) + Math.Pow(((line_point1t - point_t)*(line_point2s - line_point1s) - (line_point1s - point_s)*(line_point2t - line_point1t)),2)*(-2*line_point2s + 2*line_point1s)/Math.Pow((Math.Pow((line_point2s - line_point1s),2) + Math.Pow((line_point2t - line_point1t),2)),2);
            result[5] = (-2*line_point1s + 2*point_s)*((line_point1t - point_t)*(line_point2s - line_point1s) - (line_point1s - point_s)*(line_point2t - line_point1t))/(Math.Pow((line_point2s - line_point1s),2) + Math.Pow((line_point2t - line_point1t),2)) + Math.Pow(((line_point1t - point_t)*(line_point2s - line_point1s) - (line_point1s - point_s)*(line_point2t - line_point1t)),2)*(-2*line_point2t + 2*line_point1t)/Math.Pow((Math.Pow((line_point2s - line_point1s),2) + Math.Pow((line_point2t - line_point1t),2)),2);
            result[6] = -2*distance;

            return result;

            #pragma warning restore 169,219
        }

    } // class distance_point_line_2d

    class hori_vert_2d : SolverFunctionBase {

        public hori_vert_2d(DOF dof1, DOF dof2)
        {
            AddDOF(dof1);
            AddDOF(dof2);
        }

        public hori_vert_2d(Stack<DOF> dof_list)
        {
            // Check to make sure the correct number of parameters have been provided
            if(dof_list.Count != 2)
                throw new ArgumentException("The DOF list for the constructor of SolverFunction hori_vert_2d did not contain exactly 2 items.", "dof_list");

            foreach (DOF item in dof_list)
                AddDOF(item);
        }

        public override string Name {
            get { return "hori_vert_2d" ;}
        }

        public override double GetValue()
        {
            double dof1 = GetDOF(0).Value;
            double dof2 = GetDOF(1).Value;

            return dof1 - dof2;
        }

        public override double GetValueSelf(Vector input)
        {
            double dof1 = input[0];
            double dof2 = input[1];

            return dof1 - dof2;
        }

        public override Vector GetGradientSelf(Vector input)
        {
            #pragma warning disable 169,219

            Vector result = new DenseVector(GetNumDOFs(),1);

            double dof1 = input[0];
            double dof2 = input[1];

            result[0] = 1;
            result[1] = -1;

            return result;

            #pragma warning restore 169,219
        }

    } // class hori_vert_2d

}


