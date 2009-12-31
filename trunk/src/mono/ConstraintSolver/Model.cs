
using System;
using System.Collections.Generic;
using dnAnalytics.LinearAlgebra;
using pSketcher.ConstraintSolver.SolverFunctions;
using pSketcher.NumericalOptimization;

namespace pSketcher.ConstraintSolver
{


    class Model
    {
        //
        // Field data
        //
        private Dictionary<uint,ConstraintEquation> constraint_equation_dict_ = new Dictionary<uint,ConstraintEquation>();
        private Dictionary<uint,Primitive> primitive_dict_ = new Dictionary<uint,Primitive>();

        //
        // Properties
        //
        public Dictionary<uint,ConstraintEquation> ConstraintEquationDictionary { get {return constraint_equation_dict_;}}
        public Dictionary<uint,Primitive> PrimitiveDictionary { get {return primitive_dict_;}}

        //
        // Constructors
        //
        public Model ()
        {
        }

        //
        // Methods to add and remove primitives and constraints
        //
        public void AddPrimitive(Primitive new_primitive)
        {
             primitive_dict_[new_primitive.Id] = new_primitive;
        }

        public void AddPrimitives(params Primitive[] new_primitives)
        {
             for(int i = 0; i < new_primitives.Length; i++)
                AddPrimitive(new_primitives[i]);
        }

        public void AddConstraint(ConstraintEquation new_constraint)
        {
             constraint_equation_dict_[new_constraint.Id] = new_constraint;
        }

        public void AddConstraints(params ConstraintEquation[] new_constraints)
        {
             for(int i = 0; i < new_constraints.Length; i++)
                AddConstraint(new_constraints[i]);
        }

        //
        // General Methods
        //

        // event handler for printing progress updates for the MeritFunction class instance
        static void MeritFunction_PrintProgressUpdate (object sender, PrintProgressUpdateEventArgs e)
        {
             Console.Write (e.ProgressUpdate);
        }

        // This method solves the system of constraint equations
        public void SolveConstraints()
        {
            // only procedd if at least one constraint equation exists
            if(constraint_equation_dict_.Count > 0)
            {
                // create constraint equation and constraint equation weight vectors
                var constraints = new List<SolverFunctionBase>();
                var weights = new List<double>();

                foreach( ConstraintEquation current_constraint in constraint_equation_dict_.Values ) {
                    constraints.Add(current_constraint.SolverFunction);
                    weights.Add(current_constraint.Weight);
                }

                // create the free parameters, free_values, fixed_parameters, and fixed_values lists
                var free_parameters = new List<DOF>();
                var free_values = new List<double>();
                var fixed_parameters = new List<DOF>();
                var fixed_values = new List<double>();

                // Need to make a list of all of the DOF's referenced by the constraint equations
                // Will used a Dictionary since we can't have duplicates in the list and Dictionary's simply overwrite duplicate keys
                var dof_dict = new Dictionary<uint,DOF>();
                List<DOF> current_dof_list;
                foreach (ConstraintEquation current_constraint in constraint_equation_dict_.Values) {
                    current_dof_list = current_constraint.GetDOFList();
                    foreach (DOF current_dof in current_dof_list) {
                        if (!(current_dof is DependentDOF)) {
                            dof_dict[current_dof.Id] = current_dof;
                        } else {
                            // This is a dependent DOF
                            // Add the DOF's that this DOF depends on to the DOF list
                            foreach (DOF inner_current_dof in ((DependentDOF)current_dof).GetDOFList()) {
                                dof_dict[inner_current_dof.Id] = inner_current_dof;
                            }
                        }
                    }
                }

                // Now iterate through the DOF list and assign the dof's to the free_parameters or the fixed_parameters lists
                // Need to use lists now instead of dictionaries since insertion order matters for free_parameter and fixed_parameters
                foreach (DOF current_dof in dof_dict.Values) {
                    if (current_dof.Free) {
                        // free parameter
                        free_parameters.Add(current_dof);
                        free_values.Add(current_dof.Value);
                    } else if (!(current_dof is DependentDOF)) {
                        // fixed, independent parameter
                        fixed_parameters.Add(current_dof);
                        fixed_values.Add(current_dof.Value);
                    } else {
                        // A dependent DOF has been incountered
                        // This shouldn't happen and is an error condition
                        throw new Exception("A dependent DOF was encountered in dof_dict where none is expected.");
                    }
                }

                // only continue if there are actually some free values to solve for
                if (free_values.Count > 0)
                {
                    // populate the initial condition matrix using free_values
                    Vector initial_free_values = new DenseVector(free_values);

                    ConstraintSolver my_constraint_solver = new ConstraintSolver(constraints, weights, free_parameters, fixed_parameters, fixed_values);

                    my_constraint_solver.PrintProgressUpdate += MeritFunction_PrintProgressUpdate;

                    Vector computed_free_values = my_constraint_solver.Minimize(initial_free_values, 1000, 1e-10, 1e-15, 100, 1, 0);

                    // Update the free DOF's with the solution
                    for(int current_dof = 0; current_dof < free_parameters.Count; current_dof++)
                        free_parameters[current_dof].Value = computed_free_values[current_dof];
                }
            }
        }
    }
}
