
using System;
using pSketcher.ConstraintSolver.SolverFunctions;

namespace pSketcher.ConstraintSolver
{

    /// <summary>
    /// Constraint class for a distance constraint between two Point2D primitives
    /// </summary>
    class DistancePoint2D : ConstraintEquation
    {
        //
        // Constructors
        //
        public DistancePoint2D (Point2D point1, Point2D Point2, double distance )
        {
            this.Point1 = point1;
            this.Point2 = point1;
            
            this.Distance = new IndependentDOF(distance, false /* free */);
            
            SolverFunction = new distance_point_2d(Point1.S, Point1.T, Point2.S, Point2.T, Distance);
            
            Weight = 1.0;
        }
        
        //
        // Properties and Acessors
        //
        public Point2D Point1 { get; private set; }
        public Point2D Point2 { get; private set; }
        public DOF Distance { get; private set; }
    }
}
