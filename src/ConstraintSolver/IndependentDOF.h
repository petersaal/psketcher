/****************************************************************************
**
** This file is part of the pSketcher project.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Copyright (C) 2006-2008 Michael Greminger. All rights reserved.
**
****************************************************************************/

#ifndef IndependentDOFH
#define IndependentDOFH

#include "DOF.h"

const std::string SQL_independent_dof_database_table_name = "independent_dof_list";

const std::string SQL_independent_dof_database_schema = "CREATE TABLE " + SQL_independent_dof_database_table_name + " (id INTEGER PRIMARY KEY, variable_name TEXT NOT NULL, bool_free INTEGER CHECK (bool_free >= 0 AND bool_free <= 1), value REAL NOT NULL);";

// IndependentDOF class
class IndependentDOF : public DOF
{
	public:
		IndependentDOF ( double value = 0.0, bool free = false );
		IndependentDOF ( const char *name, double value = 0.0, bool free = false );
		// the following constructor creates the DOF from the database stored in psketcher_model
		IndependentDOF ( unsigned id, pSketcherModel &psketcher_model );

		//Accessor methods
		void SetValue ( double value, bool update_db = true );
		double GetValue()const {return value_;}
		virtual void SetFree(bool free);

		// method for adding this object to the SQLite3 database
		virtual void AddToDatabase(sqlite3 *database);
		virtual void RemoveFromDatabase();
		void DatabaseAddDelete(bool add_to_database); // utility method called by AddToDatabase and DeleteFromDatabase since they both do similar things

		// method to synchronize this object to the database, returns false if the object does not exist in the database
		virtual bool SyncToDatabase(pSketcherModel &psketcher_model);

	private:
		double value_;
};
typedef boost::shared_ptr<IndependentDOF> IndependentDOFPointer;


#endif //IndependentDOFH
