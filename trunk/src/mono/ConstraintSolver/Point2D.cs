
using System;

namespace pSketcher.ConstraintSolver
{

    /// <summary>
    /// 2D Point
    /// </summary>
    class Point2D : Primitive2D
    {

        //
        // Constructors
        //
        public Point2D (SketchPlane sketch_plane, double s, double t, bool s_free, bool t_free) : 
            base (sketch_plane)
        {
            this.S = new IndependentDOF(s,s_free);
            this.T = new IndependentDOF(t,t_free);
        }

        public Point2D (SketchPlane sketch_plane, DOF s, DOF t) : 
            base (sketch_plane)
        {
            this.S = s;
            this.T = t;
        }
        
        //
        // Properties and Asseccors
        //
        public DOF S { get; private set; }
        public DOF T { get; private set; }
        
    } // class Point2D
}
