//
//  DOF.cs
//
//  Author:
//       grem <${AuthorEmail}>
//
//  Copyright (c) 2009 grem
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//

using System;

namespace pSketcher.ConstraintSolver
{
    /// <summary>
    /// Abstract base class for all DOF's
    /// </summary>
	abstract class DOF
	{
        //
        // Field Data
        //
        static uint next_id_number_ = 0;

		public DOF (bool free)
		{
            this.Id = checked(next_id_number_++);
            this.Free = free;
            this.Name = "dof_" + Id;
		}


        //
        // Properties and assecors
        //
        public abstract double Value {get; set;}
        public string Name {get; private set;}
        public bool Free {get; set;}
        public uint Id { get; private set; }

        //
        // Methods used by all instances
        //

	}
}
