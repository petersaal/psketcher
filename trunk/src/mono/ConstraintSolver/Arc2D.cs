
using System;
using pSketcher.ConstraintSolver.SolverFunctions;

namespace pSketcher.ConstraintSolver
{

    /// <summary>
    /// 2D Arc Primitive
    /// </summary>
    class Arc2D : Edge2D
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
        public Arc2D (SketchPlane sketch_plane, double s_center, double t_center, 
                     double theta1, double theta2, double radius, 
                     bool s_center_free, bool t_center_free, bool theta1_free,
                     bool theta2_free, bool radius_free) : 
            base (sketch_plane)
        {
            CenterPoint = new Point2D(SketchPlane, s_center, t_center, s_center_free, t_center_free);
            Theta1 = new IndependentDOF(theta1, theta1_free);
            Theta2 = new IndependentDOF(theta2, theta2_free);
            Radius = new IndependentDOF(radius, radius_free);
            
            // define the end points
            // end point 1
            DependentDOF s_dof = new DependentDOF(new arc2d_point_s(CenterPoint.S,Radius,Theta1));
            DependentDOF t_dof = new DependentDOF(new arc2d_point_t(CenterPoint.T,Radius,Theta1));            
            
            point1_ = new Point2D(SketchPlane, s_dof, t_dof);
            
            // end point 2
            s_dof = new DependentDOF(new arc2d_point_s(CenterPoint.S,Radius,Theta2));
            t_dof = new DependentDOF(new arc2d_point_t(CenterPoint.T,Radius,Theta2));            
            
            point2_ = new Point2D(SketchPlane, s_dof, t_dof);
            
            // define the tangent vectors
            // tangent vector for end point 1
            s_dof = new DependentDOF(new arc2d_tangent_s(Theta1));
            t_dof = new DependentDOF(new arc2d_tangent_t(Theta1));
        
            tangent1_ = new Vector2D(sketch_plane,s_dof,t_dof);   
            
            // tangent vector for end point 2
            s_dof = new DependentDOF(new arc2d_tangent_s(Theta2));
            t_dof = new DependentDOF(new arc2d_tangent_t(Theta2));
        
            tangent2_ = new Vector2D(sketch_plane,s_dof,t_dof);
        }
        
        //
        // Properties and Accessors
        //
        public override Point2D Point1 { get {return point1_;} }
        public override Point2D Point2 { get {return point2_;} }
        public override Vector2D Tangent1 { get {return tangent1_;} }
        public override Vector2D Tangent2 { get {return tangent2_;} }
        
        public Point2D CenterPoint { get; private set; }
        public DOF Radius { get; private set;}
        public DOF Theta1 { get; private set; }
        public DOF Theta2 { get; private set; }
        
    } // Class Arc2D
}
