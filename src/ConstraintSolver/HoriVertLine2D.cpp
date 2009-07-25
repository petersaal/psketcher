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

#include "IndependentDOF.h"
#include "HoriVertLine2D.h"

#include "Ark3DModel.h"

const std::string SQL_horivert_line2d_database_schema = "CREATE TABLE horivert_line2d_list (id INTEGER PRIMARY KEY, dof_table_name TEXT NOT NULL, primitive_table_name TEXT NOT NULL, line INTEGER NOT NULL, bool_vertical_constraint INTEGER INTEGER CHECK (bool_vertical_constraint >= 0 AND bool_vertical_constraint <= 1), marker_position_dof INTEGER NOT NULL, weight FLOAT NOT NULL);";

using namespace std;

// Create a parallelism constrain between two lines
HoriVertLine2D::HoriVertLine2D(const Line2DPointer line, bool vertical_constraint):
line_(line),
vertical_constraint_(vertical_constraint),
marker_position_(new IndependentDOF(0.5,false))   // by default place marker at the middle of the constrained lines
{
	AddPrimitive(line_);

	AddDOF(line_->GetS1());
	AddDOF(line_->GetS2());
	AddDOF(line_->GetT1());
	AddDOF(line_->GetT2());

	AddDOF(marker_position_);
	
	if(vertical_constraint_)
		solver_function_.reset(new hori_vert_2d(line_->GetS1(), line_->GetS2()));
	else
		solver_function_.reset(new hori_vert_2d(line_->GetT1(), line_->GetT2()));

    // randomize the marker position to try to prevent multiple markers from overlapping each other
    double min_marker_pos = 0.10;
    double max_marker_pos = 0.90;
    marker_position_->SetValue(min_marker_pos + (double)rand() * ((max_marker_pos - min_marker_pos) / (double)RAND_MAX));

	weight_ = 1.0;
}

// Construct from database
HoriVertLine2D::HoriVertLine2D(unsigned id, Ark3DModel &ark3d_model)
{
	SetID(id);
	bool exists = SyncToDatabase(ark3d_model);
	
	if(!exists) // this object does not exist in the table
	{
		stringstream error_description;
		error_description << "SQLite rowid " << id << " in table horivert_line2d_list does not exist";
		throw Ark3DException(error_description.str());
	}
}

void HoriVertLine2D::AddToDatabase(sqlite3 *database)
{
	database_ = database;
	DatabaseAddRemove(true);
}

void HoriVertLine2D::RemoveFromDatabase()
{
	DatabaseAddRemove(false);
}

void HoriVertLine2D::DatabaseAddRemove(bool add_to_database) // Utility method used by AddToDatabase and RemoveFromDatabase
{
	string sql_do, sql_undo;

	stringstream dof_list_table_name;
	dof_list_table_name << "dof_table_" << GetID();
	stringstream primitive_list_table_name;
	primitive_list_table_name << "primitive_table_" << GetID();

	// First, create the sql statements to undo and redo this operation
	stringstream temp_stream;
	temp_stream.precision(__DBL_DIG__);
	temp_stream << "BEGIN; "
                << "INSERT INTO horivert_line2d_list VALUES(" 
                << GetID() << ",'" << dof_list_table_name.str() << "','" 
				<< primitive_list_table_name.str() 
				<< "'," << line_->GetID() << "," << vertical_constraint_
				<< "," << marker_position_->GetID() 
                << "," << weight_ << "); "
                << "INSERT INTO constraint_equation_list VALUES("
                << GetID() << ",'horivert_line2d_list'); "
                << "COMMIT; ";

	if(add_to_database)
		sql_do = temp_stream.str();
	else
		sql_undo = temp_stream.str();

	temp_stream.str(""); // clears the string stream

	temp_stream << "BEGIN; "
				<< "DELETE FROM constraint_equation_list WHERE id=" << GetID() 
				<< "; DELETE FROM horivert_line2d_list WHERE id=" << GetID() 
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
			rc = sqlite3_exec(database_, ("ROLLBACK;"+SQL_horivert_line2d_database_schema).c_str(), 0, 0, &zErrMsg);  // need to add ROLLBACK since previous transaction failed
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

	// Now use the method provided by PrimitiveBase to create the tables listing the DOF's and the other Primitives that this primitive depends on.
	DatabaseAddDeleteLists(add_to_database,dof_list_table_name.str(),primitive_list_table_name.str());
}


bool HoriVertLine2D::SyncToDatabase(Ark3DModel &ark3d_model)
{
	database_ = ark3d_model.GetDatabase();

	string table_name = "horivert_line2d_list";

	char *zErrMsg = 0;
	int rc;
	sqlite3_stmt *statement;
	
	stringstream sql_command;
	sql_command << "SELECT * FROM " << table_name << " WHERE id=" << GetID() << ";";

	rc = sqlite3_prepare(ark3d_model.GetDatabase(), sql_command.str().c_str(), -1, &statement, 0);
	if( rc!=SQLITE_OK ){
		stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(ark3d_model.GetDatabase());
		throw Ark3DException(error_description.str());
	}

	rc = sqlite3_step(statement);

	stringstream dof_table_name, primitive_table_name;

	if(rc == SQLITE_ROW) {
		// row exists, store the values to initialize this object
		
		dof_table_name << sqlite3_column_text(statement,1);
		primitive_table_name << sqlite3_column_text(statement,2);
		line_ = ark3d_model.FetchPrimitive<Line2D>(sqlite3_column_int(statement,3));
		vertical_constraint_ = sqlite3_column_int(statement,4);
		marker_position_ = ark3d_model.FetchDOF(sqlite3_column_int(statement,5));
        weight_ = sqlite3_column_double(statement,6);

        // define the constraint function
        if(vertical_constraint_)
            solver_function_.reset(new hori_vert_2d(line_->GetS1(), line_->GetS2()));
        else
            solver_function_.reset(new hori_vert_2d(line_->GetT1(), line_->GetT2()));

	} else {
		// the requested row does not exist in the database
		sqlite3_finalize(statement);	

		return false; // row does not exist in the database, exit method and return false
	}

	rc = sqlite3_step(statement);
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

	// now sync the lists store in the base classes
	SyncListsToDatabase(dof_table_name.str(),primitive_table_name.str(),ark3d_model); // PrimitiveBase

	return true; // row existed in the database
}

