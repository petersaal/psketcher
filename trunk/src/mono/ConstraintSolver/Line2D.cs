
using System;
using pSketcher.ConstraintSolver.SolverFunctions;

namespace pSketcher.ConstraintSolver
{

    /// <summary>
    /// 2D Line Primitive
    /// </summary>
    class Line2D : Edge2D
    {
        //
        // Field Data
        //
        Point2D point1_;
        Point2D point2_;
        Vector2D tangent1_;
        Vector2D tangent2_;
        
        //
        // Constructors
        //
        public Line2D (SketchPlane sketch_plane, Point2D point1, Point2D point2) : 
            base (sketch_plane)
        {
            this.point1_ = point1;
            this.point2_ = point2;
            
            // define the tangent vectors
            // tangent vector for end point 1
            DependentDOF s_dof = new DependentDOF(new point2d_tangent1_s(point1_.S,point1_.T,point2_.S,point2_.T));
            DependentDOF t_dof = new DependentDOF(new point2d_tangent1_t(point1_.S,point1_.T,point2_.S,point2_.T));
        
            tangent1_ = new Vector2D(sketch_plane,s_dof,t_dof);   
            
            // tangent vector for end point 2
            s_dof = new DependentDOF(new point2d_tangent2_s(point1_.S,point1_.T,point2_.S,point2_.T));
            t_dof = new DependentDOF(new point2d_tangent2_t(point1_.S,point1_.T,point2_.S,point2_.T));
        
            tangent2_ = new Vector2D(sketch_plane,s_dof,t_dof);
        }
        
        //
        // Properties and Accessors
        //
        public override Point2D Point1 { get {return point1_;} }
        public override Point2D Point2 { get {return point2_;} }
        public override Vector2D Tangent1 { get {return tangent1_;} }
        public override Vector2D Tangent2 { get {return tangent2_;} }
        
    } // Class Line2D
}
