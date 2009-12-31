
using System;
using pSketcher.ConstraintSolver.SolverFunctions;

namespace pSketcher.ConstraintSolver
{

    /// <summary>
    /// Horizontal constraint for Line2D primitives
    /// </summary>
    class HorizontalLine2D : ConstraintEquation
    {

        public HorizontalLine2D (Line2D line)
        {
            Line = line;
            
            SolverFunction = new hori_vert_2d(Line.Point1.T, Line.Point2.T);
            
            Weight = 1.0;
        }
        
        //
        // Properties and Accessors
        //
        public Line2D Line { get; private set; }
    }
}
