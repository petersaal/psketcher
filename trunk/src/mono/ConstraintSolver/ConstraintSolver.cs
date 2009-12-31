
using System;
using System.Collections.Generic;
using pSketcher.NumericalOptimization;
using pSketcher.ConstraintSolver.SolverFunctions;
using dnAnalytics.LinearAlgebra;

namespace pSketcher.ConstraintSolver
{

    class ConstraintSolver : MeritFunction
    {
        //
        // Field data and properties
        //
        private List<SolverFunctionBase> constraints_;
        private List<double> weights_;
        List<DOF> free_parameters_;
        List<DOF> fixed_parameters_;
        Vector fixed_values_;

        //
        // Constructors
        //
        public ConstraintSolver (List<SolverFunctionBase> constraints,
                                 List<double> weights,
                                 List<DOF> free_parameters,
                                 List<DOF> fixed_parameters,
                                 List<double> fixed_values) :
            base (free_parameters.Count, LineSearchMethod.BackTrack)
        {
            // Do some sanity checks
            if(constraints.Count < 1)
                throw new ArgumentException("The list constraints must contain at least one item");

            if(constraints.Count != weights.Count)
                throw new ArgumentException("The list constraints must contain the same number of items as the list weights.");

            if(free_parameters.Count < 1)
                throw new ArgumentException("The list free_parameters must contain at least one item.");

            if(fixed_parameters.Count != fixed_values.Count)
                throw new ArgumentNullException("The list fixed_parameters must contain the same number of items as the list fixed_values.");

            // Initialize the field data
            this.constraints_ = constraints;
            this.weights_ = weights;
            this.free_parameters_ = free_parameters;
            this.fixed_parameters_ = fixed_parameters;

            this.fixed_values_ = new DenseVector(fixed_values);

            // Define the dof dict, used by each solver function to map the global parameter list to their local parameter list.
            var dof_dict = new Dictionary<uint,uint>();
            uint location = 0;
            for(int i=0; i < free_parameters_.Count; i++)
            {
                dof_dict[free_parameters_[i].Id] = location++;
            }
            for(int i=0; i < fixed_parameters_.Count; i++)
            {
                dof_dict[fixed_parameters_[i].Id] = location++;
            }

            // let each constraint create its own transform from the global parameter list to their local parameter list
            // each constraint also takes care of any dependent DOFs that also neet to define a transform
            for(int i=0; i < constraints_.Count; i++)
                constraints_[i].DefineInputMap(dof_dict);
        }


        //
        // Methods that override base class methods
        //

        public override double GetMeritValue(Vector x)
        {
            double result = 0;

            Vector full_input_vector = new DenseVector(x.Count + fixed_values_.Count);
            x.CopyTo(full_input_vector,0,0,x.Count);
            fixed_values_.CopyTo(full_input_vector,0,x.Count,fixed_values_.Count);

            for(int i=0; i < constraints_.Count; i++)
                result += weights_[i]*Math.Pow(constraints_[i].GetValue(full_input_vector),2);

            return result;
        }

        public override Vector GetMeritGradient(Vector x)
        {
            Vector full_input_vector = new DenseVector(x.Count + fixed_values_.Count);
            x.CopyTo(full_input_vector,0,0,x.Count);
            fixed_values_.CopyTo(full_input_vector,0,x.Count,fixed_values_.Count);

            Vector gradient = new DenseVector(full_input_vector.Count);

            for(int i=0; i < constraints_.Count; i++)
                gradient += weights_[i]*2.0*constraints_[i].GetValue(full_input_vector)*constraints_[i].GetGradient(full_input_vector);

            return gradient.GetSubVector(0,free_parameters_.Count);
        }


    }
}
