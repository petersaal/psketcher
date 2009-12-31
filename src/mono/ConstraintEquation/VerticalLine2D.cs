
using System;
using pSketcher.ConstraintSolver.SolverFunctions;

namespace pSketcher.ConstraintSolver
{

    /// <summary>
    /// Vertical constraint for Line2D primitives
    /// </summary>
    class VerticalLine2D : ConstraintEquation
    {

        public VerticalLine2D (Line2D line)
        {
            Line = line;
            
            SolverFunction = new hori_vert_2d(Line.Point1.S, Line.Point2.S);
            
            Weight = 1.0;
        }
        
        //
        // Properties and Accessors
        //
        public Line2D Line { get; private set; }
    }
}
