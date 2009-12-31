
using System;
using pSketcher.ConstraintSolver.SolverFunctions;

namespace pSketcher.ConstraintSolver
{

    /// <summary>
    /// Constraint class for a distance constraint between a Point2D and a Line2D primitive
    /// </summary>
    class DistancePointLine2D : ConstraintEquation
    {
        //
        // Constructors
        //
        public DistancePointLine2D (Point2D point, Line2D line, double distance )
        {
            this.Point = point;
            this.Line = line;
            
            this.Distance = new IndependentDOF(distance, false /* free */);
            
            SolverFunction = new distance_point_line_2d(Point.S, Point.T, Line.Point1.S, Line.Point1.T,
                                                        Line.Point2.S, Line.Point2.T, Distance);
            Weight = 1.0;
        }
        
        //
        // Properties and Acessors
        //
        public Point2D Point { get; private set; }
        public Line2D Line { get; private set; }
        public DOF Distance { get; private set; }
    }
}
