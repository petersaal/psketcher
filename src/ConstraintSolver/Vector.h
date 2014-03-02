/*
Copyright (c) 2006-2014, Michael Greminger
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation 
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF A
DVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef VectorH
#define VectorH

#include "PrimitiveBase.h"

const std::string SQL_vector_database_table_name = "vector_list";

const std::string SQL_vector_database_schema = "CREATE TABLE " + SQL_vector_database_table_name + " (id INTEGER PRIMARY KEY, dof_table_name TEXT NOT NULL, primitive_table_name TEXT NOT NULL, x_dof INTEGER NOT NULL, y_dof INTEGER NOT NULL, z_dof INTEGER NOT NULL, FOREIGN KEY(id) REFERENCES primitive_list(id) ON UPDATE CASCADE DEFERRABLE INITIALLY DEFERRED, FOREIGN KEY(x_dof) REFERENCES dof_list(id) ON UPDATE CASCADE DEFERRABLE INITIALLY DEFERRED, FOREIGN KEY(y_dof) REFERENCES dof_list(id) ON UPDATE CASCADE DEFERRABLE INITIALLY DEFERRED, FOREIGN KEY(z_dof) REFERENCES dof_list(id) ON UPDATE CASCADE DEFERRABLE INITIALLY DEFERRED);";

// vector class
class Vector : virtual public PrimitiveBase
{
	public:
		Vector ( double x, double y, double z, bool x_free = false, bool y_free = false, bool z_free = false);
		//Vector ( DOFPointer x, DOFPointer y, DOFPointer z );
		Vector (unsigned id, pSketcherModel &psketcher_model); // Construct from database

		double GetXValue()const {return x_->GetValue();}
		double GetYValue()const {return y_->GetValue();}
		double GetZValue()const {return z_->GetValue();}

		mmcMatrix GetmmcMatrix();  // returns mmcMatrix vector containing current location 

		void ApplySelectionMask(SelectionMask mask);

		// method for adding this object to the SQLite3 database
		virtual void AddToDatabase(sqlite3 *database);
		virtual void RemoveFromDatabase();
		void DatabaseAddRemove(bool add_to_database); // Utility method used by AddToDatabase and RemoveFromDatabase
		virtual bool SyncToDatabase(pSketcherModel &psketcher_model);

	private:
		DOFPointer x_;
		DOFPointer y_;
		DOFPointer z_;
};
typedef boost::shared_ptr<Vector> VectorPointer;



#endif //VectorH
