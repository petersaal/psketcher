
using System;
using System.Collections.Generic;
using pSketcher.ConstraintSolver.SolverFunctions;

namespace pSketcher.ConstraintSolver
{

    /// <summary>
    /// Abstract base class for contraint equations
    /// </summary>
    abstract class ConstraintEquation : Primitive
    {

        public ConstraintEquation ()
        {
        }

        //
        // Properties
        //
        public SolverFunctionBase SolverFunction { get; protected set; }
        public double Weight { get; protected set; }

        //
        // Accessor methods
        //
        public List<DOF> GetDOFList()
        {
            // Remove any duplicates by converting to a HashSet and then back to a list
            return new List<DOF> (new HashSet<DOF> (SolverFunction.GetDOFList()));
        }

    } // class ConstraintEquation
}
