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
** Copyright (C) 2006-2009 Michael Greminger. All rights reserved.
**
****************************************************************************/

#ifndef DependentDOFH
#define DependentDOFH

#include <vector>

#include "DOF.h"
#include "SolverFunctionsBase.h"

const std::string SQL_dependent_dof_database_table_name = "dependent_dof_list";

const std::string SQL_dependent_dof_database_schema = "CREATE TABLE " + SQL_dependent_dof_database_table_name + " (id INTEGER PRIMARY KEY, variable_name TEXT NOT NULL, solver_function TEXT NOT NULL, source_dof_table_name TEXT NOT NULL, FOREIGN KEY(id) REFERENCES dof_list(id) ON UPDATE CASCADE DEFERRABLE INITIALLY DEFERRED);";

// DependentDOF class
class DependentDOF : public DOF
{
	public:
		DependentDOF ( SolverFunctionsBasePointer solver_function);
		DependentDOF ( const char *name, SolverFunctionsBasePointer solver_function);
		// the following constructor creates the DOF from the database stored in psketcher_model
		DependentDOF ( unsigned id, pSketcherModel &psketcher_model );
		
		//Accessor methods
		void SetValue ( double value, bool update_db = true ) { /* @fixme warn user about an attempt to modify a dependent DOF */;}
		void SetFree(bool free) { /* @fixme warn user about an attempt to modify a dependent DOF */;}
		double GetValue()const;
		const std::vector<DOFPointer> & GetDOFList() {return GetSolverFunction()->GetDOFList();}

		// methods for adding and removing this object to the SQLite3 database
		virtual void AddToDatabase(sqlite3 *database);
		virtual void RemoveFromDatabase();
		void DatabaseAddDelete(bool add_to_database); // utility method called by AddToDatabase and DeleteFromDatabase since they both do similar things

		// method to synchronize this object to the database
		virtual bool SyncToDatabase(pSketcherModel &psketcher_model);

	private:

};
typedef boost::shared_ptr<DependentDOF> DependentDOFPointer;



#endif //DependentDOFH
