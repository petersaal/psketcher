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

#include <string>
#include <sstream>
#include "IndependentDOF.h"
#include "PrimitiveBase.h"

using namespace std;
using namespace GiNaC;

const std::string SQL_independent_dof_database_schema = "CREATE TABLE independent_dof_list (id INTEGER PRIMARY KEY, variable_name TEXT NOT NULL, bool_free INTEGER NOT NULL, value REAL NOT NULL);";

IndependentDOF ::IndependentDOF ( double value, bool free):
DOF(free,false /*dependent*/)
{
	value_=value;
}

IndependentDOF :: IndependentDOF ( const char *name, double value, bool free):
DOF(name,free,false /*dependent*/)
{
	value_ = value;
}

void IndependentDOF::SetValue ( double value ) 
{
	if(database_ != 0) // if this DOF is tied to a database then update the database
	{
		double old_value = value_;
		value_ = value;

		// define the update statement
		stringstream sql_stream;
		sql_stream.precision(__DBL_DIG__);
		sql_stream << "UPDATE independent_dof_list SET value=" 
					<< value_ << " WHERE id=" << GetID() << ";";

		string sql_update = sql_stream.str();
		
		// define the undo statement
		sql_stream.str("");
		sql_stream << "UPDATE independent_dof_list SET value=" 
					<< old_value << " WHERE id=" << GetID() << ";";
	
		string sql_undo = sql_stream.str();
		
		// do the database update
		char *zErrMsg = 0;
		int rc = sqlite3_exec(database_, sql_update.c_str(), 0, 0, &zErrMsg);
		if( rc!=SQLITE_OK ){
			std::string error_description = "SQL error: " + std::string(zErrMsg);
			sqlite3_free(zErrMsg);
			throw Ark3DException(error_description);
		}

		// store the undo/redo information in the database
		// need to use sqlite3_mprintf to make sure the single quotes in the sql statements get escaped where needed
		char *sql_undo_redo = sqlite3_mprintf("INSERT INTO undo_redo_list(undo,redo) VALUES('%q','%q');",sql_undo.c_str(),sql_update.c_str());
	
		rc = sqlite3_exec(database_, sql_undo_redo, 0, 0, &zErrMsg);
		if( rc!=SQLITE_OK ){
			std::string error_description = "SQL error: " + std::string(zErrMsg);
			sqlite3_free(zErrMsg);
			throw Ark3DException(error_description);
		}

	}else{
		// this is the case where there is not database
		value_=value; 
	} // if(database_ != 0)
}

void IndependentDOF::SetFree(bool free)
{
	if(free != IsFree())
	{
		if(database_ != 0) // if this DOF is tied to a database then update the database
		{
			bool old_value = IsFree();
			SetFree(free);
	
			// define the update statement
			stringstream sql_stream;
			sql_stream.precision(__DBL_DIG__);
			sql_stream << "UPDATE independent_dof_list SET bool_free=" 
						<< IsFree() << " WHERE id=" << GetID() << ";";
	
			string sql_update = sql_stream.str();
			
			// define the undo statement
			sql_stream.str("");
			sql_stream << "UPDATE independent_dof_list SET bool_free=" 
						<< old_value << " WHERE id=" << GetID() << ";";
		
			string sql_undo = sql_stream.str();
			
			// do the database update
			char *zErrMsg = 0;
			int rc = sqlite3_exec(database_, sql_update.c_str(), 0, 0, &zErrMsg);
			if( rc!=SQLITE_OK ){
				std::string error_description = "SQL error: " + std::string(zErrMsg);
				sqlite3_free(zErrMsg);
				throw Ark3DException(error_description);
			}
	
			// store the undo/redo information in the database
			// need to use sqlite3_mprintf to make sure the single quotes in the sql statements get escaped where needed
			char *sql_undo_redo = sqlite3_mprintf("INSERT INTO undo_redo_list(undo,redo) VALUES('%q','%q');",sql_undo.c_str(),sql_update.c_str());
		
			rc = sqlite3_exec(database_, sql_undo_redo, 0, 0, &zErrMsg);
			if( rc!=SQLITE_OK ){
				std::string error_description = "SQL error: " + std::string(zErrMsg);
				sqlite3_free(zErrMsg);
				throw Ark3DException(error_description);
			}
	
		}else{
			// this is the case where there is not a database
			SetFree(free); 
		} // if(database_ != 0)
	} // if(free != free_)
}

// method for adding this object to the SQLite3 database
void IndependentDOF::AddToDatabase(sqlite3 *database)
{
	// set the database for this object, in the future this database will be updated whenever this object is updated
	database_ = database;

	DatabaseAddDelete(true);
}

void IndependentDOF::RemoveFromDatabase()
{
	DatabaseAddDelete(false);
}

void IndependentDOF::DatabaseAddDelete(bool add_to_database) // utility method called by AddToDatabase and DeleteFromDatabase since they both do similar things
{	
	string sql_do, sql_undo;
	
	// First, create the sql statements to undo and redo this operation
	stringstream temp_stream;
	temp_stream.precision(__DBL_DIG__);
	temp_stream << "BEGIN; "
                << "INSERT INTO independent_dof_list VALUES(" 
                << GetID() << ",'" << GetVariable().get_name() << "'," 
				<< IsFree() << "," << value_ <<"); "
                << "INSERT INTO dof_list VALUES("
                << GetID() << ",'independent_dof_list'); "
                << "COMMIT; ";

	if(add_to_database)
		sql_do = temp_stream.str();
	else
		sql_undo = temp_stream.str();

	temp_stream.str(""); // clears the string stream

	temp_stream << "BEGIN; "
				<< "DELETE FROM dof_list WHERE id=" << GetID() 
				<< "; DELETE FROM independent_dof_list WHERE id=" << GetID() 
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
			
			// the table "independent_dof_list" may not exist, attempt to create
			rc = sqlite3_exec(database_, ("ROLLBACK;"+SQL_independent_dof_database_schema).c_str(), 0, 0, &zErrMsg);  // need to add ROLLBACK since previous transaction failed
			if( rc!=SQLITE_OK ){
				std::string error_description = "SQL error: " + std::string(zErrMsg);
				sqlite3_free(zErrMsg);
				throw Ark3DException(error_description);
			}
	
			// now that the table has been created, attempt the insert one last time
			rc = sqlite3_exec(database_, sql_do.c_str(), 0, 0, &zErrMsg);
			if( rc!=SQLITE_OK ){
				std::string error_description = "SQL error: " + std::string(zErrMsg);
				sqlite3_free(zErrMsg);
				throw Ark3DException(error_description);
			}
		} else {
			std::string error_description = "SQL error: " + std::string(zErrMsg);
			sqlite3_free(zErrMsg);
			throw Ark3DException(error_description);
		}
	}

	// finally, update the undo_redo_list in the database with the database changes that have just been made
	// need to use sqlite3_mprintf to make sure the single quotes in the sql statements get escaped where needed
	char *sql_undo_redo = sqlite3_mprintf("INSERT INTO undo_redo_list(undo,redo) VALUES('%q','%q')",sql_undo.c_str(),sql_do.c_str());

	rc = sqlite3_exec(database_, sql_undo_redo, 0, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		std::string error_description = "SQL error: " + std::string(zErrMsg);
		sqlite3_free(zErrMsg);
		throw Ark3DException(error_description);
	}

	sqlite3_free(sql_undo_redo);
}
