
using System;
using pSketcher.ConstraintSolver.SolverFunctions;

namespace pSketcher.ConstraintSolver
{

    /// <summary>
    /// Parallelism constraint between two Line2D primitives
    /// </summary>
    class ParallelLine2D : ConstraintEquation
    {
        //
        // Constructors
        //
        public ParallelLine2D (Line2D line1, Line2D line2)
        {
            this.Line1 = line1;
            this.Line2 = line2; 
            
            SolverFunction = new parallel_line_2d(Line1.Point1.S, Line1.Point1.T, Line1.Point2.S, Line1.Point2.T,
                                                  Line2.Point1.S, Line2.Point1.T, Line2.Point2.S, Line2.Point2.T);
            Weight = 1.0;
        }
        
        //
        // Properties and Accessors
        //
        public Line2D Line1 { get; private set; }
        public Line2D Line2 { get; private set; }
    }
}
