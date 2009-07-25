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

#include "TangentEdge2D.h"

#include "pSketcherModel.h"

const std::string SQL_tangent_edge2d_database_schema = "CREATE TABLE tangent_edge2d_list (id INTEGER PRIMARY KEY, dof_table_name TEXT NOT NULL, primitive_table_name TEXT NOT NULL, edge1 INTEGER NOT NULL, edge2 INTEGER NOT NULL, point_num_1 INTEGER NOT NULL, point_num_2 INTEGER NOT NULL, s_1_dof INTEGER NOT NULL, t_1_dof INTEGER NOT NULL, s_2_dof INTEGER NOT NULL, t_2_dof INTEGER NOT NULL, weight FLOAT NOT NULL);";

using namespace std;

TangentEdge2D::TangentEdge2D(Edge2DBasePointer edge1, EdgePointNumber point_num_1, Edge2DBasePointer edge2, EdgePointNumber point_num_2):
edge1_(edge1),
edge2_(edge2),
point_num_1_(point_num_1),
point_num_2_(point_num_2)
{
	AddPrimitive(edge1);
	AddPrimitive(edge2);

	if(point_num_1 == Point1)
	{	// use point1 of edge1
		edge1->GetTangent1(s_1_,t_1_);
	} else {
		// use point2 of edge1
		edge1->GetTangent2(s_1_,t_1_);
	}

	if(point_num_2 == Point1)
	{
		// use point1 of edge2
		edge2->GetTangent1(s_2_,t_2_);
	} else {
		// use point2 of edge2
		edge2->GetTangent2(s_2_,t_2_);
	}

	// add every DOF that the above expression depends on to the vector dof_list_
	AddDOF(s_1_);
    AddDOF(t_1_);
    AddDOF(s_2_);
    AddDOF(t_2_);

    solver_function_.reset(new tangent_edge_2d(s_1_,t_1_,s_2_,t_2_));
    weight_ = 1.0;
}

// Construct from database
TangentEdge2D::TangentEdge2D(unsigned id, pSketcherModel &psketcher_model)
{
	SetID(id);  bool exists = SyncToDatabase(psketcher_model);
	
	if(!exists) // this object does not exist in the table
	{
		stringstream error_description;
		error_description << "SQLite rowid " << id << " in table tangent_edge2d_list does not exist";
		throw pSketcherException(error_description.str());
	}
}

void TangentEdge2D::AddToDatabase(sqlite3 *database)
{
	database_ = database;
	DatabaseAddRemove(true);
}

void TangentEdge2D::RemoveFromDatabase()
{
	DatabaseAddRemove(false);
}

void TangentEdge2D::DatabaseAddRemove(bool add_to_database) // Utility method used by AddToDatabase and RemoveFromDatabase
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
                << "INSERT INTO tangent_edge2d_list VALUES("
                << GetID() << ",'" << dof_list_table_name.str() << "','"
				<< primitive_list_table_name.str()
				<< "'," << edge1_->GetID() << "," << edge2_->GetID()
				<< "," << point_num_1_ << "," << point_num_2_
                << "," << s_1_->GetID() << "," << t_1_->GetID()
                << "," << s_2_->GetID() << "," << t_2_->GetID() 
                << "," << weight_ << "); "
                << "INSERT INTO constraint_equation_list VALUES("
                << GetID() << ",'tangent_edge2d_list'); "
                << "COMMIT; ";

	if(add_to_database)
		sql_do = temp_stream.str();
	else
		sql_undo = temp_stream.str();

	temp_stream.str(""); // clears the string stream

	temp_stream << "BEGIN; "
				<< "DELETE FROM constraint_equation_list WHERE id=" << GetID() 
				<< "; DELETE FROM tangent_edge2d_list WHERE id=" << GetID() 
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
			rc = sqlite3_exec(database_, ("ROLLBACK;"+SQL_tangent_edge2d_database_schema).c_str(), 0, 0, &zErrMsg);  // need to add ROLLBACK since previous transaction failed
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

	// Now use the method provided by PrimitiveBase to create the tables listing the DOF's and the other Primitives that this primitive depends on.
	DatabaseAddDeleteLists(add_to_database,dof_list_table_name.str(),primitive_list_table_name.str());
}

bool TangentEdge2D::SyncToDatabase(pSketcherModel &psketcher_model)
{
	database_ = psketcher_model.GetDatabase();

	string table_name = "tangent_edge2d_list";

	char *zErrMsg = 0;
	int rc;
	sqlite3_stmt *statement;
	
	stringstream sql_command;
	sql_command << "SELECT * FROM " << table_name << " WHERE id=" << GetID() << ";";

	rc = sqlite3_prepare(psketcher_model.GetDatabase(), sql_command.str().c_str(), -1, &statement, 0);
	if( rc!=SQLITE_OK ){
		stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(psketcher_model.GetDatabase());
		throw pSketcherException(error_description.str());
	}

	rc = sqlite3_step(statement);

	stringstream dof_table_name, primitive_table_name, constraint_table_name;

	if(rc == SQLITE_ROW) {
		// row exists, store the values to initialize this object
		
		dof_table_name << sqlite3_column_text(statement,1);
		primitive_table_name << sqlite3_column_text(statement,2);
		edge1_ = psketcher_model.FetchPrimitive<Edge2DBase>(sqlite3_column_int(statement,3));
		edge2_ = psketcher_model.FetchPrimitive<Edge2DBase>(sqlite3_column_int(statement,4));
		point_num_1_ = static_cast<EdgePointNumber>(sqlite3_column_int(statement,5));
		point_num_2_ = static_cast<EdgePointNumber>(sqlite3_column_int(statement,6));
        s_1_ = psketcher_model.FetchDOF(sqlite3_column_int(statement,7));
        t_1_ = psketcher_model.FetchDOF(sqlite3_column_int(statement,8));
        s_2_ = psketcher_model.FetchDOF(sqlite3_column_int(statement,9));
        t_2_ = psketcher_model.FetchDOF(sqlite3_column_int(statement,10));
        weight_ = sqlite3_column_double(statement,11);

        // define the constraint equation
        solver_function_.reset(new tangent_edge_2d(s_1_,t_1_,s_2_,t_2_));

	} else {
		// the requested row does not exist in the database
		sqlite3_finalize(statement);	

		return false; // row does not exist in the database, exit method and return false
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

	// now sync the lists stored in the base classes
	SyncListsToDatabase(dof_table_name.str(),primitive_table_name.str(),psketcher_model); // PrimitiveBase

	return true; // row existed in the database
}
