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

#include "DependentDOF.h"
#include "PrimitiveBase.h"
#include "Ark3DModel.h"

using namespace std;
using namespace GiNaC;


const string SQL_dependent_dof_database_schema = "CREATE TABLE dependent_dof_list (id INTEGER PRIMARY KEY, variable_name TEXT NOT NULL, expression TEXT NOT NULL, source_dof_table_name TEXT NOT NULL);";

DependentDOF :: DependentDOF (ex expression, std::vector<DOFPointer> source_dof_list):
DOF(false /*free*/,true /*dependent*/)
{
	// @fixme Need to make sure that all DOF's in expression are included in the DOF list
	expression_ = expression;
	source_dof_list_ = source_dof_list;
}

DependentDOF :: DependentDOF ( const char *name, ex expression, std::vector<DOFPointer> source_dof_list):
DOF(name,false /*free*/,true /*dependent*/)
{
	// @fixme Need to make sure that all DOF's in expression are included in the DOF list
	expression_ = expression;
	source_dof_list_ = source_dof_list;
}

// the following constructor creates the DOF from the database stored in ark3d_model
DependentDOF :: DependentDOF ( unsigned id, Ark3DModel &ark3d_model ):
DOF(id,true /* bool dependent */)
{
	bool exists = SyncToDatabase(id,ark3d_model);
	
	if(!exists) // this object does not exist in the table
	{
		stringstream error_description;
		error_description << "SQLite rowid " << id << " in table dependent_dof_list does not exist";
		throw Ark3DException(error_description.str());
	}
}

bool DependentDOF :: SyncToDatabase(unsigned id, Ark3DModel &ark3d_model)
{
	database_ = ark3d_model.GetDatabase();
	free_ =false;

	string table_name = "dependent_dof_list";

	char *zErrMsg = 0;
	int rc;
	sqlite3_stmt *statement;
	stringstream source_dof_table_name;
	stringstream expression;

	// "CREATE TABLE dependent_dof_list (id INTEGER PRIMARY KEY, variable_name TEXT NOT NULL, expression TEXT NOT NULL, source_dof_table_name TEXT NOT NULL);"
	
	stringstream sql_command;
	sql_command << "SELECT * FROM " << table_name << " WHERE id=" << id << ";";

	rc = sqlite3_prepare(ark3d_model.GetDatabase(), sql_command.str().c_str(), -1, &statement, 0);
	if( rc!=SQLITE_OK ){
		std::string error_description = "SQL error: " + std::string(zErrMsg);
		sqlite3_free(zErrMsg);
		throw Ark3DException(error_description);
	}

	rc = sqlite3_step(statement);

	if(rc == SQLITE_ROW) {
		// row exist, store the values to initialize this object
		id_number_ = sqlite3_column_int(statement,0);
		stringstream variable_name;
		variable_name << sqlite3_column_text(statement,1);
		variable_.set_name(variable_name.str());
		expression << sqlite3_column_text(statement,2);
		source_dof_table_name << sqlite3_column_text(statement,3);
	} else {
		// the requested row does not exist in the database
		sqlite3_finalize(statement);	

		return false; // object not present in database, return false
	}

	rc = sqlite3_step(statement);
	if( rc!=SQLITE_DONE ){
		// sql statement didn't finish properly, some error must to have occured
		std::string error_description = "SQL error: " + std::string(zErrMsg);
		sqlite3_free(zErrMsg);
		throw Ark3DException(error_description);
	}
	
	rc = sqlite3_finalize(statement);
	if( rc!=SQLITE_OK ){
		std::string error_description = "SQL error: " + std::string(zErrMsg);
		sqlite3_free(zErrMsg);
		throw Ark3DException(error_description);
	}

	// next read the source dof table that lists the DOF's that this DependentDOF depends on
	sql_command.str("");
	sql_command << "SELECT * FROM " << source_dof_table_name.str() << ";";

	rc = sqlite3_prepare(ark3d_model.GetDatabase(), sql_command.str().c_str(), -1, &statement, 0);
	if( rc!=SQLITE_OK ){
		std::string error_description = "SQL error: " + std::string(zErrMsg);
		sqlite3_free(zErrMsg);
		throw Ark3DException(error_description);
	}

	rc = sqlite3_step(statement);
	
	int source_dof_id;
	DOFPointer source_dof;
	GiNaC::lst variable_list;
	source_dof_list_.clear(); // clear the contents of this list, will be replaced by the contents of the database
	while(rc == SQLITE_ROW) {
		source_dof_id = sqlite3_column_int(statement,0);

		// get the dof (it will be automatically created if it doesn't already exist)
		source_dof = ark3d_model.FetchDOF(source_dof_id);

		source_dof_list_.push_back(source_dof);
		variable_list.append(source_dof->GetVariable());

		rc = sqlite3_step(statement);
	}

	// we now have enought information to define the expression_ member variable for this dependent dof
	expression_ = GiNaC::ex(expression.str(),variable_list);

	if( rc!=SQLITE_DONE ){
		// sql statement didn't finish properly, some error must to have occured
		std::string error_description = "SQL error: " + std::string(zErrMsg);
		sqlite3_free(zErrMsg);
		throw Ark3DException(error_description);
	}

	rc = sqlite3_finalize(statement);
	if( rc!=SQLITE_OK ){
		std::string error_description = "SQL error: " + std::string(zErrMsg);
		sqlite3_free(zErrMsg);
		throw Ark3DException(error_description);
	}
	
	return true; // object exists in table and was defined successfully
}

double DependentDOF::GetValue()const
{
	// For each independent DOF in the list, substitute its value into the expression that defines this dependent DOF
	GiNaC::ex current_expression = GetExpression();
	double result;

	for(unsigned int current_dof = 0; current_dof < source_dof_list_.size(); current_dof++)
	{
		if(!source_dof_list_[current_dof]->IsDependent())
		{
			current_expression = current_expression.subs(source_dof_list_[current_dof]->GetVariable() ==
														 source_dof_list_[current_dof]->GetValue(),subs_options::no_pattern);
		}
	}

	// Now evaluate the expression to a numeric value
	// check to make sure the expression evaluates to a numeric value
	if (is_a<numeric>(current_expression)) {
		result = ex_to<numeric>(current_expression).to_double();
	} else {
		throw Ark3DException();
  	}

	return result;
}

GiNaC::ex DependentDOF::GetExpression()const
{
	// For each dependent DOF in the list, substitute its expression into the overall expression that defines this dependent DOF
	GiNaC::ex result = expression_;

	for(unsigned int current_dof = 0; current_dof < source_dof_list_.size(); current_dof++)
	{
		if(source_dof_list_[current_dof]->IsDependent())
		{
			result = result.subs(source_dof_list_[current_dof]->GetVariable() == source_dof_list_[current_dof]->GetExpression(),subs_options::no_pattern);
		}
	}

	return result;
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
                << "INSERT INTO dependent_dof_list VALUES(" 
                << GetID() << ",'" << GetVariable().get_name() << "','" 
				<< expression_ << "', 'source_dof_table_" << GetID() <<"'); "
                << "INSERT INTO dof_list VALUES("
                << GetID() << ",'dependent_dof_list'); "
				<< "CREATE TABLE " << "source_dof_table_" << GetID() << " (id INTEGER PRIMARY KEY);";

	// add each source dof to the source_dof table that was just created for this dependent dof	
	for(unsigned int current_dof = 0; current_dof < source_dof_list_.size(); current_dof++)
	{
			temp_stream << "INSERT INTO " << "source_dof_table_" << GetID() << " VALUES(" << source_dof_list_[current_dof]->GetID() << "); ";
	}

	temp_stream << "COMMIT; ";

	if(add_to_database)
		sql_do = temp_stream.str();
	else
		sql_undo = temp_stream.str();

	temp_stream.str(""); // clears the string stream

	temp_stream << "BEGIN; "
				<< "DELETE FROM dof_list WHERE id=" << GetID() 
				<< "; DELETE FROM dependent_dof_list WHERE id=" << GetID() 
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


