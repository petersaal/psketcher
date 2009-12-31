
using System;

namespace pSketcher.ConstraintSolver
{
    /// <summary>
    /// Class that defines a point in 3D space
    /// </summary>
    class Point : Primitive
    {
        //
        // Field Data
        //

        //
        // Constructors
        //
        public Point (double x, double y, double z, bool x_free, bool y_free, bool z_free)
        {
            X = new IndependentDOF(x,x_free);
            Y = new IndependentDOF(x,x_free);
            Z = new IndependentDOF(x,x_free);
        }
        
        public Point(double x, double y, double z) :
            this (x,y,z,false,false,false)
        {
            
        }
        
        // 
        // Properties and Accessors
        //
        public DOF X {get; private set;}
        public DOF Y {get; private set;}
        public DOF Z {get; private set;}
        
    } // class Point
}
