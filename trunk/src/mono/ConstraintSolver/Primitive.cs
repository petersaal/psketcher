
using System;

namespace pSketcher.ConstraintSolver
{
	/// <summary>
	/// Abstract base class for all primitives.
	/// </summary>
    abstract class Primitive {

        //
        // Field Data
        //
        static uint next_id_number_ = 0;

        //
        // Constructors
        //
        public Primitive ()
        {
        	Id = checked(next_id_number_++);
        }

        //
        // Assecor Methods and Properties
        //
        public uint Id { get; private set; }
        public bool Selected { get; set; }
        public bool Selectable { get; set;}
	}
}
