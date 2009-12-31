using System;

namespace pSketcher.ConstraintSolver
{
    /// <summary>
    /// Class that defines a vector in 3D space
    /// </summary>
    class Vector3D : Primitive
    {
        //
        // Field Data
        //

        //
        // Constructors
        //
        public Vector3D (double x, double y, double z, bool x_free, bool y_free, bool z_free)
        {
            X = new IndependentDOF(x,x_free);
            Y = new IndependentDOF(x,x_free);
            Z = new IndependentDOF(x,x_free);
        }
        
        public Vector3D(double x, double y, double z) :
            this (x,y,z,false,false,false)
        {
            
        }
        
        // 
        // Properties and Accessors
        //
        public DOF X {get; private set;}
        public DOF Y {get; private set;}
        public DOF Z {get; private set;}
        
    } // class Vector
}