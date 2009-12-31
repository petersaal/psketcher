
using System;

namespace pSketcher.ConstraintSolver
{

    /// <summary>
    /// The IndependentDOF class defines a DOF that depends on no other DOF's
    /// </summary>
    class IndependentDOF : DOF
    {

        public IndependentDOF (double value, bool free) : base (free)
        {
            this.Value = value;
        }

        //
        // Properties and accessor methods
        //
        public override double Value {get; set;}

    }
}
