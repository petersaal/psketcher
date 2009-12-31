
using System;

namespace pSketcher.ConstraintSolver
{

    /// <summary>
    /// This class implements a SketchPlane in 2D space
    /// </summary>
    class SketchPlane : Primitive
    {   
        //
        // Constructors
        //
        public SketchPlane (Vector3D normal, Vector3D up, Point @base)
        {
            this.Normal = normal;
            this.Up = up;
            this.Base = @base;
        }
        
        //
        // Properties and Accessors
        //
        public Vector3D Normal {get; private set;}
        public Vector3D Up {get; private set;}
        public Point Base  { get; private set;}
        
    } // class SketchPlane
}
