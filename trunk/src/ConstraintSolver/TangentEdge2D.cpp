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

const std::string SQL_tangent_edge2d_database_schema = "CREATE TABLE tangent_edge2d_list (id INTEGER PRIMARY KEY, dof_table_name TEXT NOT NULL, primitive_table_name TEXT NOT NULL, constraint_table_name TEXT NOT NULL, edge1 INTEGER NOT NULL, edge2 INTEGER NOT NULL, point_num_1 INTEGER NOT NULL, point_num_2 INTEGER NOT NULL);";

using namespace std;
using namespace GiNaC;

TangentEdge2D::TangentEdge2D(Edge2DBasePointer edge1, EdgePointNumber point_num_1, Edge2DBasePointer edge2, EdgePointNumber point_num_2):
edge1_(edge1),
edge2_(edge2),
point_num_1_(point_num_1),
point_num_2_(point_num_2)
{
	AddPrimitive(edge1);
	AddPrimitive(edge2);

	ex s_1, t_1, s_2, t_2;	// tangent vector for edge1 (s_1,t_1) and tangent vector for edge2 (s_2,t_2)

	if(point_num_1 == Point1)
	{	// use point1 of edge1
		edge1->GetTangent1(s_1,t_1);
	} else {
		// use point2 of edge1
		edge1->GetTangent2(s_1,t_1);
	}

	if(point_num_2 == Point1)
	{
		// use point1 of edge2
		edge2->GetTangent1(s_2,t_2);
	} else {
		// use point2 of edge2
		edge2->GetTangent2(s_2,t_2);
	}

	// create the expression object that will store the constraint
	boost::shared_ptr<ex> new_constraint(new ex);

	// Calculate the dot product between the two tangent vectors
	// this expression will be zero when the lines are parallel
	// Ideally, I would use abs() instead of pow() but abs is not differentiable. 
	*new_constraint = pow((s_1*s_2 + t_1*t_2),2)-1;

	// populate the lists
	constraints_.push_back(new_constraint);
	weight_list_.push_back(1.0);
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
	stringstream constraint_list_table_name;
	constraint_list_table_name << "constraint_table_" << GetID();

	//"CREATE TABLE tangent_edge2d_list (id INTEGER PRIMARY KEY, dof_table_name TEXT NOT NULL, primitive_table_name TEXT NOT NULL, constraint_table_name TEXT NOT NULL, edge1 INTEGER NOT NULL, edge2 INTEGER NOT NULL, point_num_1 INTEGER NOT NULL, point_num_2 INTEGER NOT NULL);"

	// First, create the sql statements to undo and redo this operation
	stringstream temp_stream;
	temp_stream.precision(__DBL_DIG__);
	temp_stream << "BEGIN; "
                << "INSERT INTO tangent_edge2d_list VALUES(" 
                << GetID() << ",'" << dof_list_table_name.str() << "','" 
				<< primitive_list_table_name.str() 
				<< "','" << constraint_list_table_name.str()
				<< "'," << edge1_->GetID() << "," << edge2_->GetID()
				<< "," << point_num_1_ << "," << point_num_2_ << "); "
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

	// Now use the methods provided by PrimitiveBase and ConstraintEquationBase to create the tables listing the DOF's, the other Primitives that this primitive depends on, and the constraint equations
	DatabaseAddDeleteLists(add_to_database,dof_list_table_name.str(),primitive_list_table_name.str());
	DatabaseAddDeleteConstraintList(add_to_database, constraint_list_table_name.str());
}
