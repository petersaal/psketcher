
using System;
using System.Collections.Generic;
using dnAnalytics.LinearAlgebra;

namespace pSketcher.ConstraintSolver.SolverFunctions
{

    abstract class SolverFunctionBase
    {
        List<DOF> dof_list_ = new List<DOF>();
        Matrix transform_;

        // Accessors methods for dof_list_
        public void AddDOF(DOF new_dof) {dof_list_.Add(new_dof);}
        public DOF GetDOF(int index) {return dof_list_[index];}
        public int GetNumDOFs() {return dof_list_.Count;}
        public List<DOF> GetDOFList() { return dof_list_; }

        // Abstract properties and methods
        public abstract string Name { get;}
        public abstract double GetValue();
        public abstract double GetValueSelf(Vector input);
        public abstract Vector GetGradientSelf(Vector input);

        //
        // Methods
        //

        /// <summary>
        /// Calculates the value of this solver function using the global x vector.
        /// </summary>
        /// <param name="x">
        /// A <see cref="Vector"/>
        /// </param>
        /// <returns>
        /// A <see cref="System.Double"/>
        /// </returns>
        public double GetValue(Vector x)
        {
            Vector local_x = transform_*x;

            for(int counter = 0; counter < dof_list_.Count; counter++)
            {
                if (dof_list_[counter] is DependentDOF)
                    local_x[counter] = ((DependentDOF)dof_list_[counter]).SolverFunction.GetValue(x);
            }

            return GetValueSelf(local_x);
        }

        /// <summary>
        /// Calculates the gradient of the solver function using the global x vector.
        /// </summary>
        /// <param name="x">
        /// A <see cref="Vector"/>
        /// </param>
        /// <returns>
        /// A <see cref="DenseMatrix"/>
        /// </returns>
        public Vector GetGradient(Vector x)
        {
            Vector local_x = transform_*x;
            Matrix temp = new DenseMatrix(dof_list_.Count);
            temp.SetDiagonal(new DenseVector(dof_list_.Count,1.0)); // set to the identity matrix
            Matrix jacobian = transform_.Transpose()*temp;

            Vector temp_gradient;

            for(int i = 0; i < dof_list_.Count; i++)
            {
                if (dof_list_[i] is DependentDOF)
                {
                    local_x[i] = ((DependentDOF)dof_list_[i]).SolverFunction.GetValue(x);
                    temp_gradient = ((DependentDOF)dof_list_[i]).SolverFunction.GetGradient(x);
                    jacobian.SetColumn(i,temp_gradient);
                }
            }

            return jacobian*GetGradientSelf(local_x);
        }


        /// <summary>
        /// Defines the transform_ matrix based on the supplied DOF mapping. The key
        /// for input_dof_map is the DOF ID and the value is the DOF location in the global
        /// x vector.
        /// </summary>
        /// <param name="input_dof_dict">
        /// A <see cref="Dictionary<System.UInt32, System.UInt32>"/>
        /// </param>
        public void DefineInputMap(Dictionary<uint,uint> input_dof_dict)
        {
            Matrix new_transform = new DenseMatrix(dof_list_.Count,input_dof_dict.Count);

            uint dof_location;

            for(int i = 0; i < dof_list_.Count; i++)
            {
                if (!(dof_list_[i] is DependentDOF))
                {

                    if(input_dof_dict.TryGetValue(dof_list_[i].Id, out dof_location))
                    {
                        // dof found in dictionary
                        new_transform[i,(int)dof_location] = 1.0;
                    } else {
                        // dof not found in dictionary, need to throw an exception
                        throw new ApplicationException("DOF with the ID " + dof_list_[i].Id + " not found in input dictionary while defining transform_ for a SolverFunctionsBase instance.");
                    }
                }
            }

            transform_ = new_transform;

            // let each dependent dof define its own transform_
            for(int i=0; i < dof_list_.Count; i++)
                if(dof_list_[i] is DependentDOF)
                    ((DependentDOF)dof_list_[i]).SolverFunction.DefineInputMap(input_dof_dict);
        }


    }
}
