
using System;

namespace pSketcher.ConstraintSolver
{

    /// <summary>
    /// 2D Circle Primitive
    /// </summary>
    class Circle2D : Primitive2D
    {
        //
        // Constructors
        //
        public Circle2D (SketchPlane sketch_plane, double s_center, double t_center, double radius,
                         bool s_center_free, bool t_center_free, bool radius_free) :
            base (sketch_plane)
        {
            CenterPoint = new Point2D(SketchPlane, s_center, t_center, s_center_free, t_center_free);
            Radius = new IndependentDOF(radius, radius_free);
        }
        
        //
        // Properties and Accessors
        //
        public Point2D CenterPoint { get; private set; }
        public DOF Radius { get; private set;}
        
    } // class Circle2D
}
