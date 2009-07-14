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

#include "AngleLine2D.h"
#include "IndependentDOF.h"

#include "Ark3DModel.h"

const std::string SQL_angle_line2d_database_schema = "CREATE TABLE angle_line2d_list (id INTEGER PRIMARY KEY, dof_table_name TEXT NOT NULL, primitive_table_name TEXT NOT NULL, constraint_table_name TEXT NOT NULL, line1 INTEGER NOT NULL, line2 INTEGER NOT NULL, angle_dof INTEGER NOT NULL, interior_angle_bool INTEGER NOT NULL, text_angle_dof INTEGER NOT NULL, text_radius_dof INTEGER NOT NULL, text_s_dof INTEGER NOT NULL, text_t_dof INTEGER NOT NULL);";

using namespace std;
using namespace GiNaC;

// Create an angle constraint between two lines
AngleLine2D::AngleLine2D(const Line2DPointer line1, const Line2DPointer line2, double angle /* radians */, bool interior_angle):
line1_(line1),
line2_(line2),
interior_angle_(interior_angle),
text_angle_(new IndependentDOF(0.0,false)),
text_radius_(new IndependentDOF(0.0,false)),
text_s_(new IndependentDOF(0.0,false)),
text_t_(new IndependentDOF(0.0,false))
{
	// store the primitives that this primitive depends on
	AddPrimitive(line1);
	AddPrimitive(line2);

	AddDOF(line1_->GetS1());
	AddDOF(line1_->GetS2());
	AddDOF(line1_->GetT1());
	AddDOF(line1_->GetT2());

	AddDOF(line2_->GetS1());
	AddDOF(line2_->GetS2());
	AddDOF(line2_->GetT1());
	AddDOF(line2_->GetT2());

	AddDOF(text_angle_);
	AddDOF(text_radius_);
	AddDOF(text_s_);
	AddDOF(text_t_);

	SetDefaultTextLocation();

	// Create a DOF for the angle parameter
	DOFPointer new_dof(new IndependentDOF(angle,false));
	angle_ = new_dof;	

	AddDOF(angle_);

	// create the expression that defines the parallel constraint and add it the the constraint list
	boost::shared_ptr<ex> new_constraint(new ex);
	
    /*
	ex line1_ds = line1->GetS2()->GetVariable() - line1->GetS1()->GetVariable();
	ex line1_dt = line1->GetT2()->GetVariable() - line1->GetT1()->GetVariable();
	ex line1_length = sqrt(pow(line1_ds,2)+pow(line1_dt,2));

	ex line2_ds = line2->GetS2()->GetVariable() - line2->GetS1()->GetVariable();
	ex line2_dt = line2->GetT2()->GetVariable() - line2->GetT1()->GetVariable();
	ex line2_length = sqrt(pow(line2_ds,2)+pow(line2_dt,2));

	ex desired_angle = angle_->GetVariable();
    */

	// Calculate the dot product normalized by the vector lengths and subtract the cos of the desired angle
	// this expression will be zero when the lines are at the desired angle
	if(interior_angle_)
    {
		//*new_constraint = (1/(line1_length*line2_length))*(line1_ds*line2_ds + line1_dt*line2_dt)-cos(desired_angle);

        *new_constraint = angle_line_2d_interior(line1->GetS1()->GetVariable(),line1->GetT1()->GetVariable(),line1->GetS2()->GetVariable(),line1->GetT2()->GetVariable(),line2->GetS1()->GetVariable(),line2->GetT1()->GetVariable(),line2->GetS2()->GetVariable(),line2->GetT2()->GetVariable(),angle_->GetVariable());
	} else {
		//*new_constraint = (1/(line1_length*line2_length))*(line1_ds*line2_ds + line1_dt*line2_dt)-cos(mmcPI-desired_angle);

        *new_constraint = angle_line_2d_exterior(line1->GetS1()->GetVariable(),line1->GetT1()->GetVariable(),line1->GetS2()->GetVariable(),line1->GetT2()->GetVariable(),line2->GetS1()->GetVariable(),line2->GetT1()->GetVariable(),line2->GetS2()->GetVariable(),line2->GetT2()->GetVariable(),angle_->GetVariable());
    }

	constraints_.push_back(new_constraint);
	weight_list_.push_back(1.0);
}

// Construct from database
AngleLine2D::AngleLine2D(unsigned id, Ark3DModel &ark3d_model)
{
	SetID(id);  bool exists = SyncToDatabase(ark3d_model);
	
	if(!exists) // this object does not exist in the table
	{
		stringstream error_description;
		error_description << "SQLite rowid " << id << " in table angle_line2d_list does not exist";
		throw Ark3DException(error_description.str());
	}
}

void AngleLine2D::SetDefaultTextLocation()
{
	// first determine the intersection point of the two lines
	double x1 = line1_->GetPoint1()->GetSValue();
	double x2 = line1_->GetPoint2()->GetSValue();
	double x3 = line2_->GetPoint1()->GetSValue();
	double x4 = line2_->GetPoint2()->GetSValue();

	double y1 = line1_->GetPoint1()->GetTValue();
	double y2 = line1_->GetPoint2()->GetTValue();
	double y3 = line2_->GetPoint1()->GetTValue();
	double y4 = line2_->GetPoint2()->GetTValue();

	double denominator = (x1-x2)*(y3-y4)-(x3-x4)*(y1-y2);

	if(denominator == 0.0)
	{
		// so use the length of the lines to set a reasonable radius and set angle to 0.0
		text_radius_->SetValue(0.25*(sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)) + sqrt((x3-x4)*(x3-x4)+(y3-y4)*(y3-y4))));
		text_angle_->SetValue(0.0);

		// use the following parameters to locate the text instead
		text_s_->SetValue(0.25*(x1+x2+x3+x4));
		text_t_->SetValue(0.25*(y1+y2+y3+y4));
	} else {
		// lines do intersect
		// finish calculating the intersection point
		double temp1 = x1*y2-y1*x2;
		double temp2 = x3*y4-x4*y3;
		
		double x_center = (temp1*(x3-x4)-temp2*(x1-x2))/denominator;
		double y_center = (temp1*(y3-y4)-temp2*(y1-y2))/denominator;
		
		// calculate the average radius and the average angle of all of the line endpoints
		double ave_radius = 0.0;
		double ave_angle = 0.0;
		
		ave_radius += sqrt((x1-x_center)*(x1-x_center)+(y1-y_center)*(y1-y_center));
		ave_radius += sqrt((x2-x_center)*(x2-x_center)+(y2-y_center)*(y2-y_center));
		ave_radius += sqrt((x3-x_center)*(x3-x_center)+(y3-y_center)*(y3-y_center));
		ave_radius += sqrt((x4-x_center)*(x4-x_center)+(y4-y_center)*(y4-y_center));
		ave_radius = ave_radius/4.0;

		int angle_counter = 0;
		if((y1-y_center)*(y1-y_center)+(x1-x_center)*(x1-x_center) > 0)
		{
			ave_angle += atan2(y1-y_center,x1-x_center) + 2.0*mmcPI;
			angle_counter++;
		}

		if((y2-y_center)*(y2-y_center)+(x2-x_center)*(x2-x_center) > 0)
		{
			ave_angle += atan2(y2-y_center,x2-x_center) + 2.0*mmcPI;
			angle_counter++;
		}

		if((y3-y_center)*(y3-y_center)+(x3-x_center)*(x3-x_center) > 0)
		{
			ave_angle += atan2(y3-y_center,x3-x_center) + 2.0*mmcPI;
			angle_counter++;
		}

		if((y4-y_center)*(y4-y_center)+(x4-x_center)*(x4-x_center) > 0)
		{
			ave_angle += atan2(y4-y_center,x4-x_center) + 2.0*mmcPI;
			angle_counter++;
		}

		if(angle_counter > 0)
			ave_angle = ave_angle / (double)angle_counter;
		else
			ave_angle = 0.0;

		text_radius_->SetValue(ave_radius*0.75);
		text_angle_->SetValue(ave_angle);
	}
}

void AngleLine2D::SetSTTextLocation(double text_s, double text_t, bool update_db)
{
	// first determine the intersection point of the two lines
	double x1 = line1_->GetPoint1()->GetSValue();
	double x2 = line1_->GetPoint2()->GetSValue();
	double x3 = line2_->GetPoint1()->GetSValue();
	double x4 = line2_->GetPoint2()->GetSValue();

	double y1 = line1_->GetPoint1()->GetTValue();
	double y2 = line1_->GetPoint2()->GetTValue();
	double y3 = line2_->GetPoint1()->GetTValue();
	double y4 = line2_->GetPoint2()->GetTValue();

	double denominator = (x1-x2)*(y3-y4)-(x3-x4)*(y1-y2);

	if(denominator == 0.0)
	{
		// the lines are parallel

		// use the following parameters to locate the text instead of text_radius_ and text_angle_
		text_s_->SetValue(text_s, update_db);
		text_t_->SetValue(text_t, update_db);
	} else {
		// lines do intersect
		// finish calculating the intersection point
		double temp1 = x1*y2-y1*x2;
		double temp2 = x3*y4-x4*y3;
		
		double x_center = (temp1*(x3-x4)-temp2*(x1-x2))/denominator;
		double y_center = (temp1*(y3-y4)-temp2*(y1-y2))/denominator;

		text_radius_->SetValue(sqrt((x_center - text_s)*(x_center - text_s) + (y_center - text_t)*(y_center - text_t)), update_db);
		text_angle_->SetValue(atan2(text_t-y_center, text_s-x_center), update_db);
	}
}

double AngleLine2D::GetActualAngle() const
{	
	boost::shared_ptr<ex> new_constraint(new ex);
	
	double line1_ds = line1_->GetS2()->GetValue() - line1_->GetS1()->GetValue();
	double line1_dt = line1_->GetT2()->GetValue() - line1_->GetT1()->GetValue();
	double line1_length = sqrt(line1_ds*line1_ds+line1_dt*line1_dt);

	double line2_ds = line2_->GetS2()->GetValue() - line2_->GetS1()->GetValue();
	double line2_dt = line2_->GetT2()->GetValue() - line2_->GetT1()->GetValue();
	double line2_length = sqrt(line2_ds*line2_ds+line2_dt*line2_dt);

	double actual_angle;
	if(interior_angle_)
		actual_angle = acos((1/(line1_length*line2_length))*(line1_ds*line2_ds + line1_dt*line2_dt));
	else
		actual_angle = mmcPI - acos((1/(line1_length*line2_length))*(line1_ds*line2_ds + line1_dt*line2_dt));


	return actual_angle;
}

void AngleLine2D::AddToDatabase(sqlite3 *database)
{
	database_ = database;
	DatabaseAddRemove(true);
}

void AngleLine2D::RemoveFromDatabase()
{
	DatabaseAddRemove(false);
}

void AngleLine2D::DatabaseAddRemove(bool add_to_database) // Utility method used by AddToDatabase and RemoveFromDatabase
{
	string sql_do, sql_undo;

	stringstream dof_list_table_name;
	dof_list_table_name << "dof_table_" << GetID();
	stringstream primitive_list_table_name;
	primitive_list_table_name << "primitive_table_" << GetID();
	stringstream constraint_list_table_name;
	constraint_list_table_name << "constraint_table_" << GetID();

	//"CREATE TABLE angle_line2d_list (id INTEGER PRIMARY KEY, dof_table_name TEXT NOT NULL, primitive_table_name TEXT NOT NULL, constraint_table_name TEXT NOT NULL, line1 INTEGER NOT NULL, line2 INTEGER NOT NULL, angle_dof INTEGER NOT NULL, interior_angle_bool INTEGER NOT NULL, text_angle_dof INTEGER NOT NULL, text_radius_dof INTEGER NOT NULL, text_s_dof INTEGER NOT NULL, text_t_dof INTEGER NOT NULL);"

	// First, create the sql statements to undo and redo this operation
	stringstream temp_stream;
	temp_stream.precision(__DBL_DIG__);
	temp_stream << "BEGIN; "
                << "INSERT INTO angle_line2d_list VALUES(" 
                << GetID() << ",'" << dof_list_table_name.str() << "','" 
				<< primitive_list_table_name.str() 
				<< "','" << constraint_list_table_name.str()
				<< "'," << line1_->GetID() << "," << line2_->GetID()
				<< "," << angle_->GetID() << "," << interior_angle_
				<< "," << text_angle_->GetID() << "," << text_radius_->GetID()
				<< "," << text_s_->GetID() << "," << text_t_->GetID()
				<< "); "
                << "INSERT INTO constraint_equation_list VALUES("
                << GetID() << ",'angle_line2d_list'); "
                << "COMMIT; ";

	if(add_to_database)
		sql_do = temp_stream.str();
	else
		sql_undo = temp_stream.str();

	temp_stream.str(""); // clears the string stream

	temp_stream << "BEGIN; "
				<< "DELETE FROM constraint_equation_list WHERE id=" << GetID() 
				<< "; DELETE FROM angle_line2d_list WHERE id=" << GetID() 
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
			rc = sqlite3_exec(database_, ("ROLLBACK;"+SQL_angle_line2d_database_schema).c_str(), 0, 0, &zErrMsg);  // need to add ROLLBACK since previous transaction failed
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

bool AngleLine2D::SyncToDatabase(Ark3DModel &ark3d_model)
{
	database_ = ark3d_model.GetDatabase();

	string table_name = "angle_line2d_list";

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
		line1_ = ark3d_model.FetchPrimitive<Line2D>(sqlite3_column_int(statement,4));
		line2_ = ark3d_model.FetchPrimitive<Line2D>(sqlite3_column_int(statement,5));
		angle_ = ark3d_model.FetchDOF(sqlite3_column_int(statement,6));
		interior_angle_ = sqlite3_column_int(statement,7);
		text_angle_ = ark3d_model.FetchDOF(sqlite3_column_int(statement,8));
		text_radius_ = ark3d_model.FetchDOF(sqlite3_column_int(statement,9));
		text_s_ = ark3d_model.FetchDOF(sqlite3_column_int(statement,10));
		text_t_ = ark3d_model.FetchDOF(sqlite3_column_int(statement,11));

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
