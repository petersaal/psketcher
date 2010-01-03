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

#include <string>
#include <sstream>

#include "DependentDOF.h"
#include "PrimitiveBase.h"
#include "pSketcherModel.h"

using namespace std;

DependentDOF :: DependentDOF (SolverFunctionsBasePointer solver_function):
DOF(false /*free*/,true /*dependent*/)
{
	SetSolverFunction(solver_function);
}

DependentDOF :: DependentDOF ( const char *name, SolverFunctionsBasePointer solver_function):
DOF(name,false /*free*/,true /*dependent*/)
{
	SetSolverFunction(solver_function);
}

// the following constructor creates the DOF from the database stored in psketcher_model
DependentDOF :: DependentDOF ( unsigned id, pSketcherModel &psketcher_model ):
DOF(id,true /* bool dependent */)
{
	SetID(id);
    bool exists = SyncToDatabase(psketcher_model);
	
	if(!exists) // this object does not exist in the table
	{
		stringstream error_description;
		error_description << "SQLite rowid " << id << " in table " << SQL_dependent_dof_database_table_name << " does not exist";
		throw pSketcherException(error_description.str());
	}
}

bool DependentDOF :: SyncToDatabase(pSketcherModel &psketcher_model)
{
	database_ = psketcher_model.GetDatabase();
	free_ =false;

	string table_name = SQL_dependent_dof_database_table_name;

	char *zErrMsg = 0;
	int rc;
	sqlite3_stmt *statement;
	stringstream source_dof_table_name;
	stringstream solver_function_name;
    vector<DOFPointer> solver_function_dof_list;

	// "CREATE TABLE dependent_dof_list (id INTEGER PRIMARY KEY, variable_name TEXT NOT NULL, solver_function TEXT NOT NULL, source_dof_table_name TEXT NOT NULL);"
	
	stringstream sql_command;
	sql_command << "SELECT * FROM " << table_name << " WHERE id=" << GetID() << ";";

	rc = sqlite3_prepare(psketcher_model.GetDatabase(), sql_command.str().c_str(), -1, &statement, 0);
	if( rc!=SQLITE_OK ){
		stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(psketcher_model.GetDatabase());
		throw pSketcherException(error_description.str());
	}

	rc = sqlite3_step(statement);

	if(rc == SQLITE_ROW) {
		// row exist, store the values to initialize this object
		
		stringstream variable_name;
		variable_name << sqlite3_column_text(statement,1);
		SetName(variable_name.str());
		solver_function_name << sqlite3_column_text(statement,2);
		source_dof_table_name << sqlite3_column_text(statement,3);
	} else {
		// the requested row does not exist in the database
		sqlite3_finalize(statement);	

		return false; // object not present in database, return false
	}

	rc = sqlite3_step(statement);
	if( rc!=SQLITE_DONE ){
		// sql statement didn't finish properly, some error must to have occured
		stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(psketcher_model.GetDatabase());
		throw pSketcherException(error_description.str());
	}
	
	rc = sqlite3_finalize(statement);
	if( rc!=SQLITE_OK ){
		stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(psketcher_model.GetDatabase());
		throw pSketcherException(error_description.str());
	}

	// next read the source dof table that lists the DOF's that this DependentDOF depends on
	sql_command.str("");
	sql_command << "SELECT * FROM " << source_dof_table_name.str() << ";";

	rc = sqlite3_prepare(psketcher_model.GetDatabase(), sql_command.str().c_str(), -1, &statement, 0);
	if( rc!=SQLITE_OK ){
		stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(psketcher_model.GetDatabase());
		throw pSketcherException(error_description.str());
	}

	rc = sqlite3_step(statement);
	
	int source_dof_id;
	DOFPointer source_dof;
	while(rc == SQLITE_ROW) {
		source_dof_id = sqlite3_column_int(statement,1);

		// get the dof (it will be automatically created if it doesn't already exist)
		source_dof = psketcher_model.FetchDOF(source_dof_id);

		solver_function_dof_list.push_back(source_dof);

		rc = sqlite3_step(statement);
	}

	// we now have enought information to define the solver function for this dependent dof
	SetSolverFunction(SolverFunctionsFactory(solver_function_name.str(),solver_function_dof_list));

	if( rc!=SQLITE_DONE ){
		// sql statement didn't finish properly, some error must to have occured
		stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(psketcher_model.GetDatabase());
		throw pSketcherException(error_description.str());
	}

	rc = sqlite3_finalize(statement);
	if( rc!=SQLITE_OK ){
		stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(psketcher_model.GetDatabase());
		throw pSketcherException(error_description.str());
	}
	
	return true; // object exists in table and was defined successfully
}

double DependentDOF::GetValue()const
{
	return GetSolverFunction()->GetValue();
}

// method for adding this object to the SQLite3 database
void DependentDOF::AddToDatabase(sqlite3 *database)
{
	// set the database for this object, in the future this database will be updated whenever this object is updated
	database_ = database;

	DatabaseAddDelete(true);
}

void DependentDOF::RemoveFromDatabase()
{
	DatabaseAddDelete(false);
}

void DependentDOF::DatabaseAddDelete(bool add_to_database) // utility method called by AddToDatabase and DeleteFromDatabase since they both do similar things
{
	// First, create the sql statements to undo and redo this operation
	string sql_do, sql_undo;

	stringstream temp_stream;
	temp_stream.precision(__DBL_DIG__);
	temp_stream << "BEGIN; "
                << "INSERT INTO " << SQL_dependent_dof_database_table_name << " VALUES(" 
                << GetID() << ",'" << GetName() << "','" 
				<< GetSolverFunction()->GetName() << "', 'source_dof_table_" << GetID() <<"'); "
                << "INSERT INTO dof_list VALUES("
                << GetID() << ",'" << SQL_dependent_dof_database_table_name << "'); "
				<< "CREATE TABLE " << "source_dof_table_" << GetID() << " (id INTEGER PRIMARY KEY, dof_id INTEGER NOT NULL, FOREIGN KEY(dof_id) REFERENCES dof_list(id) ON UPDATE CASCADE DEFERRABLE INITIALLY DEFERRED);";

	// add each source dof to the source_dof table that was just created for this dependent dof	
	for(unsigned int current_dof = 0; current_dof < GetSolverFunction()->GetDOFList().size(); current_dof++)
	{
			temp_stream << "INSERT INTO " << "source_dof_table_" << GetID() << " VALUES(" << current_dof << "," << GetSolverFunction()->GetDOFList()[current_dof]->GetID() << "); ";
	}

	temp_stream << "COMMIT; ";

	if(add_to_database)
		sql_do = temp_stream.str();
	else
		sql_undo = temp_stream.str();

	temp_stream.str(""); // clears the string stream

	temp_stream << "BEGIN; "
				<< "DELETE FROM dof_list WHERE id=" << GetID() 
				<< "; DELETE FROM " << SQL_dependent_dof_database_table_name << " WHERE id=" << GetID() 
				<< "; DROP TABLE " << "source_dof_table_" << GetID()
				<< "; COMMIT;";

	if(add_to_database)
		sql_undo = temp_stream.str();
	else
		sql_do = temp_stream.str();

	// add this object to the appropriate tables by executing the SQL command sql_insert 
	char *zErrMsg = 0;
	int rc = sqlite3_exec(database_, sql_do.c_str(), 0, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		if(add_to_database)
		{
			//std::cerr << "SQL error: " << zErrMsg << endl;
			sqlite3_free(zErrMsg);
			
			// the table "dependent_dof_list" may not exist, attempt to create
			rc = sqlite3_exec(database_, ("ROLLBACK;"+SQL_dependent_dof_database_schema).c_str(), 0, 0, &zErrMsg);  // need to add ROLLBACK since previous transaction failed
			if( rc!=SQLITE_OK ){
				std::string error_description = "SQL error: " + std::string(zErrMsg);
				sqlite3_free(zErrMsg);
				throw pSketcherException(error_description);
			}
	
			// now that the table has been created, attempt the insert one last time
			rc = sqlite3_exec(database_, sql_do.c_str(), 0, 0, &zErrMsg);
			if( rc!=SQLITE_OK ){
				std::string error_description = "SQL error: " + std::string(zErrMsg);
				sqlite3_free(zErrMsg);
				throw pSketcherException(error_description);
			}
		} else {
			std::string error_description = "SQL error: " + std::string(zErrMsg);
			sqlite3_free(zErrMsg);
			throw pSketcherException(error_description);
		} 
	}

	// finally, update the undo_redo_list in the database with the database changes that have just been made
	// need to use sqlite3_mprintf to make sure the single quotes in the sql statements get escaped where needed
	char *sql_undo_redo = sqlite3_mprintf("INSERT INTO undo_redo_list(undo,redo) VALUES('%q','%q')",sql_undo.c_str(),sql_do.c_str());

	rc = sqlite3_exec(database_, sql_undo_redo, 0, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		std::string error_description = "SQL error: " + std::string(zErrMsg);
		sqlite3_free(zErrMsg);
		throw pSketcherException(error_description);
	}

	sqlite3_free(sql_undo_redo);
}


