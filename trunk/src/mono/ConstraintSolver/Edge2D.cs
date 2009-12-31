
using System;

namespace pSketcher.ConstraintSolver
{

    
    enum EdgePointNumber {Point1, Point2}

    /// <summary>
    /// Abstract base class for 2D edge primitives
    /// </summary>
    abstract class Edge2D : Primitive2D
    {
        // 
        // Constructors
        //
        public Edge2D (SketchPlane sketch_plane) : base (sketch_plane)
        {
        }
        
        //
        // Abstract properties and accessors
        //
        public abstract Point2D Point1 { get; }
        public abstract Point2D Point2 { get; }
        public abstract Vector2D Tangent1 { get; }
        public abstract Vector2D Tangent2 { get; }
        
        //
        // Methods
        //
        public bool IsPointCoincident(EdgePointNumber my_point_number, Edge2D other_edge, EdgePointNumber other_point_number)
        {
            bool result;
        
            Point2D my_point;
            Point2D other_point;
            
            // assign the Point pointers
            if(my_point_number == EdgePointNumber.Point1)
                my_point = Point1;
            else
                my_point = Point2;
        
            if(other_point_number == EdgePointNumber.Point1)
                other_point = other_edge.Point1;
            else
                other_point = other_edge.Point2;
        
            // perform the comparison
            if(my_point.S == other_point.S && my_point.T == other_point.T)
                result = true;
            else
                result = false;
        
            return result;
        }
        
    } // Class Edge2D
}
