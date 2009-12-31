
using System;
using pSketcher.ConstraintSolver.SolverFunctions;

namespace pSketcher.ConstraintSolver
{

    /// <summary>
    /// Angle constraint between two Line2D primitives
    /// </summary>
    class AngleLine2D : ConstraintEquation
    {
        // 
        // Constructors
        //
        public AngleLine2D (Line2D line1, Line2D line2, double angle, bool interior_angle)
        {
            this.Line1 = line1;
            this.Line2 = line2;
            this.Angle = new IndependentDOF(angle, false /* free */);
            this.InteriorAngle = interior_angle;
            
            if (InteriorAngle)
                SolverFunction = new angle_line_2d_interior(Line1.Point1.S, Line1.Point1.T, Line1.Point2.S, Line1.Point2.T,
                                                            Line2.Point1.S, Line2.Point1.T, Line2.Point2.S, Line2.Point2.T,
                                                            Angle); 
            else
                SolverFunction = new angle_line_2d_exterior(Line1.Point1.S, Line1.Point1.T, Line1.Point2.S, Line1.Point2.T,
                                                            Line2.Point1.S, Line2.Point1.T, Line2.Point2.S, Line2.Point2.T,
                                                            Angle);
            
            Weight = 1.0;
        }

        
        //
        // Properties and Accessors
        //
        public Line2D Line1 { get; private set; }
        public Line2D Line2 { get; private set; }
        public DOF Angle { get; private set; }
        public bool InteriorAngle { get; private set; }
    }
}
