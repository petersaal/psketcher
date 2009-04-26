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
** Copyright (C) 2009 Michael Greminger. All rights reserved.
**
****************************************************************************/

#include <sstream>

const std::string SQL_distance_pointline2d_database_schema = "CREATE TABLE distance_pointline2d_list (id INTEGER PRIMARY KEY, dof_table_name TEXT NOT NULL, primitive_table_name TEXT NOT NULL, constraint_table_name TEXT NOT NULL, distance_dof INTEGER NOT NULL, point INTEGER NOT NULL, line INTEGER NOT NULL, text_offset_dof INTEGER NOT NULL, text_position_dof INTEGER NOT NULL);";

#include "DistancePointLine2D.h"
#include "IndependentDOF.h"

#include "Ark3DModel.h"

using namespace std;
using namespace GiNaC;

// Create a constraint that defines the distance between two points confined to a sketch plane
DistancePointLine2D::DistancePointLine2D(const Point2DPointer point, const Line2DPointer line, double distance):
point_(point),
line_(line)
{
	AddPrimitive(point);
	AddPrimitive(line);

	AddDOF(point->GetSDOF());
	AddDOF(point->GetTDOF());
	AddDOF(line->GetS1());
	AddDOF(line->GetT1());
	AddDOF(line->GetS2());
	AddDOF(line->GetT2());

	// Create a DOF for the distance parameter
	DOFPointer new_dof(new IndependentDOF(distance,false));
	distance_ = new_dof;

	AddDOF(distance_);

	// create DOF's for the text location
	text_offset_.reset(new IndependentDOF(0.0,false));
	text_position_.reset(new IndependentDOF(0.0,false));
	AddDOF(text_offset_);
	AddDOF(text_position_);	

	// create the expression that defines the constraint and add it the the constraint list
	boost::shared_ptr<ex> new_constraint(new ex);
	
	*new_constraint = abs((line_->GetS2()->GetVariable() - line_->GetS1()->GetVariable())*(line_->GetT1()->GetVariable() - point_->GetTDOF()->GetVariable())
						- (line_->GetT2()->GetVariable() - line_->GetT1()->GetVariable())*(line_->GetS1()->GetVariable() - point_->GetSDOF()->GetVariable()))
						/ sqrt( pow(line_->GetS2()->GetVariable() - line_->GetS1()->GetVariable(),2) +
								pow(line_->GetT2()->GetVariable() - line_->GetT1()->GetVariable(),2))
						- distance_->GetVariable();

	constraints_.push_back(new_constraint);
	weight_list_.push_back(1.0);

	// text location was not specified so provide a reasonable default
	SetDefaultTextLocation();
}

// Construct from database
DistancePointLine2D::DistancePointLine2D(unsigned id, Ark3DModel &ark3d_model)
{
	SetID(id);  bool exists = SyncToDatabase(ark3d_model);
	
	if(!exists) // this object does not exist in the table
	{
		stringstream error_description;
		error_description << "SQLite rowid " << id << " in table distance_pointline2d_list does not exist";
		throw Ark3DException(error_description.str());
	}
}

// Calculate the current actual distance between the point and the line
// This method is used to set the distance when graphically generating a distance constraint
double DistancePointLine2D::GetActualDistance() const
{
	// calculate the distance between point1_ and point2_
	double term1 = (line_->GetS2()->GetValue() - line_->GetS1()->GetValue());
	double term1_sq = term1*term1;
	double term2 = (line_->GetT2()->GetValue() - line_->GetT1()->GetValue());
	double term2_sq = term2*term2;
	
	double den = sqrt(term1_sq+term2_sq);

	double term3 = (line_->GetT1()->GetValue() - point_->GetTDOF()->GetValue());
	double term4 = (line_->GetS1()->GetValue() - point_->GetSDOF()->GetValue());

	double num = fabs(term1*term3 - term2*term4);

	return num/den;
}


void DistancePointLine2D::SetDefaultTextLocation()
{
	mmcMatrix point1 = line_->GetPoint1()->GetmmcMatrix();
	mmcMatrix point2 = line_->GetPoint2()->GetmmcMatrix();
	mmcMatrix point3 = point_->GetmmcMatrix();

	mmcMatrix tangent = (point2-point1);
	double tangent_magnitude = tangent.GetMagnitude();
	if (tangent_magnitude > 0.0)
	{
		tangent = tangent.GetScaled(1.0/tangent_magnitude);
	} else {
		// tangent vector has zero length, define an arbitrary tangent vector to avoid divide by zero
		tangent(0,0) = 1.0;
		tangent(1,0) = 0.0;	
	}

	mmcMatrix normal(2,1);
	normal(0,0) = -tangent(1,0);
	normal(1,0) = tangent(0,0);

	double dot_product = normal.DotProduct(point3-point1);
	
	if(dot_product >= 0.0)
		text_offset_->SetValue(0.5*GetActualDistance());
	else
		text_offset_->SetValue(-0.5*GetActualDistance());	

	if((point3 - point2).GetMagnitude() > (point3 - point1).GetMagnitude())
		text_position_->SetValue(-0.1*tangent_magnitude);
	else
		text_position_->SetValue(1.1*tangent_magnitude);
}

void DistancePointLine2D::SetSTTextLocation(double s, double t)
{
	mmcMatrix point1 = line_->GetPoint1()->GetmmcMatrix();
	mmcMatrix point2 = line_->GetPoint2()->GetmmcMatrix();

	mmcMatrix tangent = (point2-point1);
	double tangent_magnitude = tangent.GetMagnitude();
	if (tangent_magnitude > 0.0)
	{
		tangent = tangent.GetScaled(1.0/tangent_magnitude);
	} else {
		// tangent vector has zero length, define an arbitrary tangent vector to avoid divide by zero
		tangent(0,0) = 1.0;
		tangent(1,0) = 0.0;	
	}

	mmcMatrix normal(2,1);
	normal(0,0) = -tangent(1,0);
	normal(1,0) = tangent(0,0);

	mmcMatrix text_location(2,1);
	text_location(0,0) = s;
	text_location(1,0) = t;

	mmcMatrix inverse(2,2);
	inverse(0,0) = tangent(1,0);
	inverse(0,1) = -tangent(0,0);
	inverse(1,0) = -normal(1,0);
	inverse(1,1) = normal(0,0);
	inverse = (1.0/(normal(0,0)*tangent(1,0) - tangent(0,0)*normal(1,0)))*inverse;

	mmcMatrix solution = inverse*(text_location - point1);
	text_offset_->SetValue(solution(0,0));
	text_position_->SetValue(solution(1,0));
}

void DistancePointLine2D::AddToDatabase(sqlite3 *database)
{
	database_ = database;
	DatabaseAddRemove(true);
}

void DistancePointLine2D::RemoveFromDatabase()
{
	DatabaseAddRemove(false);
}

void DistancePointLine2D::DatabaseAddRemove(bool add_to_database) // Utility method used by AddToDatabase and RemoveFromDatabase
{
	string sql_do, sql_undo;

	stringstream dof_list_table_name;
	dof_list_table_name << "dof_table_" << GetID();
	stringstream primitive_list_table_name;
	primitive_list_table_name << "primitive_table_" << GetID();
	stringstream constraint_list_table_name;
	constraint_list_table_name << "constraint_table_" << GetID();

	//"CREATE TABLE distance_pointline2d_list (id INTEGER PRIMARY KEY, dof_table_name TEXT NOT NULL, primitive_table_name TEXT NOT NULL, constraint_table_name TEXT NOT NULL, distance_dof INTEGER NOT NULL, point INTEGER NOT NULL, line INTEGER NOT NULL, text_offset_dof INTEGER NOT NULL, text_position_dof INTEGER NOT NULL);"

	// First, create the sql statements to undo and redo this operation
	stringstream temp_stream;
	temp_stream.precision(__DBL_DIG__);
	temp_stream << "BEGIN; "
                << "INSERT INTO distance_pointline2d_list VALUES(" 
                << GetID() << ",'" << dof_list_table_name.str() << "','" 
				<< primitive_list_table_name.str() 
				<< "','" << constraint_list_table_name.str()
				<< "'," << distance_->GetID() << "," << point_->GetID()
				<< "," << line_->GetID() << "," << text_offset_->GetID() 
				<< "," << text_position_->GetID() << "); "
                << "INSERT INTO constraint_equation_list VALUES("
                << GetID() << ",'distance_pointline2d_list'); "
                << "COMMIT; ";

	if(add_to_database)
		sql_do = temp_stream.str();
	else
		sql_undo = temp_stream.str();

	temp_stream.str(""); // clears the string stream

	temp_stream << "BEGIN; "
				<< "DELETE FROM constraint_equation_list WHERE id=" << GetID() 
				<< "; DELETE FROM distance_pointline2d_list WHERE id=" << GetID() 
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
			rc = sqlite3_exec(database_, ("ROLLBACK;"+SQL_distance_pointline2d_database_schema).c_str(), 0, 0, &zErrMsg);  // need to add ROLLBACK since previous transaction failed
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


bool DistancePointLine2D::SyncToDatabase(Ark3DModel &ark3d_model)
{
	database_ = ark3d_model.GetDatabase();

	string table_name = "distance_pointline2d_list";

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

	stringstream dof_table_name, primitive_table_name, constraint_table_name;

	if(rc == SQLITE_ROW) {
		// row exists, store the values to initialize this object
		
		dof_table_name << sqlite3_column_text(statement,1);
		primitive_table_name << sqlite3_column_text(statement,2);
		constraint_table_name << sqlite3_column_text(statement,3);
		distance_ = ark3d_model.FetchDOF(sqlite3_column_int(statement,4));
		point_ = ark3d_model.FetchPrimitive<Point2D>(sqlite3_column_int(statement,5));
		line_ = ark3d_model.FetchPrimitive<Line2D>(sqlite3_column_int(statement,6));
		text_offset_ = ark3d_model.FetchDOF(sqlite3_column_int(statement,7));
		text_position_ = ark3d_model.FetchDOF(sqlite3_column_int(statement,8));
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
	SyncConstraintListToDatabase(constraint_table_name.str(),ark3d_model); // ConstraintEquationBase 

	return true; // row existed in the database
}
