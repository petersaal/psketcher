
using System;
using System.Collections.Generic;

namespace pSketcher.ConstraintSolver
{
    /// <summary>
    /// DependentDOF implements a DOF that depends on one or more other DOF's
    /// </summary>
    class DependentDOF : DOF
    {
        //
        // Constructors
        //
        public DependentDOF (SolverFunctions.SolverFunctionBase solver_function) : base (false /*free*/)
        {
            SolverFunction = solver_function;
        }

        //
        // Properties and accessor methods
        //
        public override double Value {get {return SolverFunction.GetValue();}
                                      set {}}

        public SolverFunctions.SolverFunctionBase SolverFunction{ get; private set; }

        // Get a list of DOF's that this DOF depends on
        // Recurse on any DependentDOF's
        // Also, there should be no duplicates in the resulting list
        public List<DOF> GetDOFList()
        {
            var dof_set = new HashSet<DOF> ();

            foreach (DOF current_dof in SolverFunction.GetDOFList()) {
                if (!(current_dof is DependentDOF)) {
                    dof_set.Add(current_dof);
                } else {
                    // this dof is also dependent, recurse
                    foreach (DOF inner_current_dof in ((DependentDOF)current_dof).GetDOFList()) {
                        dof_set.Add(inner_current_dof);
                    }
                }
            }
            return new List<DOF> (dof_set);
        }
    }
}
