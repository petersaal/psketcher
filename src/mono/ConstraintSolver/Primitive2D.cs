
using System;

namespace pSketcher.ConstraintSolver
{
    /// <summary>
    /// Abstract base class for 2D primitives. Stores the sketch plane
    /// that 2D primitives are constrained to.
    /// </summary>
    abstract class Primitive2D : Primitive
    {
        //
        // Constructors
        //
        public Primitive2D (SketchPlane sketch_plane)
        {
            this.SketchPlane = sketch_plane;
        }
        
        //
        // Properties and Asseccors
        //
        public SketchPlane SketchPlane { get; private set; }
        
    } // class Primitive2D
}
