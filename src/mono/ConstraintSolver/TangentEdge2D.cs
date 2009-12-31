
using System;
using pSketcher.ConstraintSolver.SolverFunctions;

namespace pSketcher.ConstraintSolver
{

    /// <summary>
    /// Tangency constraint between two Edge2D primitives
    /// </summary>
    class TangentEdge2D : ConstraintEquation
    {

        public TangentEdge2D (Edge2D edge1, EdgePointNumber edge1_point_number, Edge2D edge2, EdgePointNumber edge2_point_number)
        {
            this.Edge1 = edge1;
            this.Edge2 = edge2;
            this.Edge1PointNumber = edge1_point_number;
            this.Edge2PointNumber = edge2_point_number;
            
            // Make sure that the two edges share the end points specified
            if (!edge1.IsPointCoincident(edge1_point_number,edge2,edge2_point_number))
                throw new ArgumentException("The two supplied edges to not sure the supplied end points");
            
            // Now get the tangent vectors for each edge
            Vector2D tangent1;
            if(edge1_point_number == EdgePointNumber.Point1)
                tangent1 = edge1.Tangent1;
            else 
                tangent1 = edge1.Tangent2;
            
            Vector2D tangent2;
            if(edge2_point_number == EdgePointNumber.Point1)
                tangent2 = edge2.Tangent1;
            else 
                tangent2 = edge2.Tangent2;
            
            SolverFunction = new tangent_edge_2d(tangent1.S, tangent1.T, tangent2.S, tangent2.T);
            Weight = 1.0;
        }

        
        //
        // Properties and Accessors
        //
        public Edge2D Edge1 { get; private set; }
        public Edge2D Edge2 { get; private set; }
        public EdgePointNumber Edge1PointNumber { get; private set; }
        public EdgePointNumber Edge2PointNumber { get; private set; }
    }
}
