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

#include <sstream>

#include "ConstraintEquationBase.h"
#include "Ark3DModel.h"

#include "ConstraintEquations.h"

using namespace std;
using namespace GiNaC;

void ConstraintEquationBase::ApplySelectionMask(SelectionMask mask)
{
	if(mask == All || mask == Constraints)
		SetSelectable(true);
	else
		SetSelectable(false);
}

// Utility method to add the constraints_ list to the database
void ConstraintEquationBase::DatabaseAddDeleteConstraintList(bool add_to_database, const std::string &constraint_list_table_name)
{
	string sql_do, sql_undo;	
	stringstream temp_stream;

	temp_stream << "BEGIN;"
				<< "CREATE TABLE " << constraint_list_table_name << " (expression TEXT NOT NULL, weight FLOAT NOT NULL);";

	// add the insert statements for each constraint in constraints_	
	for(unsigned int current_constraint = 0; current_constraint < constraints_.size(); current_constraint++)
	{
		temp_stream << "INSERT INTO " << constraint_list_table_name << " VALUES('" << *(constraints_[current_constraint]) << "'," << weight_list_[current_constraint] << "); ";
	}

	temp_stream << "COMMIT;";

	if(add_to_database)
		sql_do = temp_stream.str();
	else
		sql_undo = temp_stream.str();

	temp_stream.str("");

	// define the undo sql statement
	temp_stream << "BEGIN; "
 				<< "DROP TABLE " << constraint_list_table_name
				<< "; COMMIT;";

	if(add_to_database)
		sql_undo = temp_stream.str();
	else
		sql_do = temp_stream.str();

	// go ahead and create the sql tables
	char *zErrMsg = 0;
	int rc = sqlite3_exec(database_, sql_do.c_str(), 0, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		std::string error_description = "SQL error: " + std::string(zErrMsg);
		sqlite3_free(zErrMsg);
		throw Ark3DException(error_description);
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
}

// utility method to synchronize the contraints_ and weight_list_ vectors to the database
// Important: this method assumes that the method PrimitiveBase::SyncListsToDatabase(...) has been called prior to calling this method (needs the dof_list_ vector up to date)
void ConstraintEquationBase::SyncConstraintListToDatabase(const std::string &constraint_list_table_name, Ark3DModel &ark3d_model)
{
	int rc;
	sqlite3_stmt *statement;

	// synchronize the constraint_ and weight_list_ parallel vectors to the database
	
	// clear the contents of the constraint vectors, they will be recreated from the database
	constraints_.clear();
	weight_list_.clear();

	// need to populate the list of variables that all of the constraints in the list will depend on (populate from PrimitiveBase::dof_list_)
	std::vector<DOFPointer> dof_list = GetDOFList();
	GiNaC::lst variable_list;
	for(unsigned int current_dof = 0; current_dof < dof_list.size(); current_dof++)
	{
		variable_list.append(dof_list[current_dof]->GetVariable());
	}

	// create the SQL statement needed to pull the table called constraint_list_table_name
	stringstream sql_command;
	sql_command << "SELECT * FROM " << constraint_list_table_name << ";";

	rc = sqlite3_prepare(ark3d_model.GetDatabase(), sql_command.str().c_str(), -1, &statement, 0);
	if( rc!=SQLITE_OK ){
		stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(ark3d_model.GetDatabase());
		throw Ark3DException(error_description.str());
	}

	rc = sqlite3_step(statement);
	
	int current_dof_id;
	DOFPointer current_dof;
	stringstream expression;
	double weight;
	boost::shared_ptr<GiNaC::ex> current_constraint;
	while(rc == SQLITE_ROW) {
		expression.str("");
		expression << sqlite3_column_text(statement,0); // column 0 holds the text form the the constraint equation
		weight = sqlite3_column_double(statement,1);    // column 1 holds the weight of the above constraint equation

		current_constraint.reset(new GiNaC::ex(expression.str(),variable_list));
		constraints_.push_back(current_constraint);
		weight_list_.push_back(weight);
	
		rc = sqlite3_step(statement);
	}

	if( rc!=SQLITE_DONE ){
		// sql statement didn't finish properly, some error must to have occured
		stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(ark3d_model.GetDatabase());
		throw Ark3DException(error_description.str());
	}

	rc = sqlite3_finalize(statement);
	if( rc!=SQLITE_OK ){
		stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(ark3d_model.GetDatabase());
		throw Ark3DException(error_description.str());
	}
}