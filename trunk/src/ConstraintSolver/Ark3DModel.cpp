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

#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>

// Begining of includes related to libdime (used for dxf import and export)
#include "../dime/Basic.h"  // force the inclusion of my own version of this libdime header (the default version creates a conflicting decleration of uint32 as compared to GiNaC)
#include <dime/Model.h>
#include <dime/Output.h>
#include <dime/entities/Entity.h>
#include <dime/sections/EntitiesSection.h>
// End of includes related to libdime

#include "Ark3DModel.h"

using namespace std;

const std::string SQL_ark3d_database_schema = 
"BEGIN;"
	"CREATE TABLE dof_list (id INTEGER PRIMARY KEY, table_name TEXT NOT NULL);"
	"CREATE TABLE primitive_list (id INTEGER PRIMARY KEY, table_name TEXT NOT NULL);"
	"CREATE TABLE constraint_equation_list (id INTEGER PRIMARY KEY, table_name TEXT NOT NULL);"
	"CREATE TABLE undo_redo_list (id INTEGER PRIMARY KEY, undo TEXT, redo TEXT);"
	"CREATE TABLE undo_stable_points (id INTEGER PRIMARY KEY, stable_point INTEGER NOT NULL UNIQUE, bool_current_stable_point INTEGER CHECK (bool_current_stable_point >= 0 AND bool_current_stable_point <= 1), description TEXT);"
	"CREATE TRIGGER clear_current_stable_point BEFORE UPDATE ON undo_stable_points "
	"WHEN (NEW.bool_current_stable_point == 1 AND OLD.bool_current_stable_point == 0) "
	"BEGIN "
		"UPDATE undo_stable_points SET bool_current_stable_point=0 WHERE bool_current_stable_point=1;"
	"END;"
	"CREATE TRIGGER truncate_undo_list BEFORE INSERT ON undo_redo_list "
	"WHEN EXISTS (SELECT stable_point FROM undo_stable_points WHERE bool_current_stable_point=1) "
	"BEGIN "
		"DELETE FROM undo_redo_list WHERE id > (SELECT stable_point FROM  undo_stable_points WHERE bool_current_stable_point=1);"
		"DELETE FROM undo_stable_points WHERE stable_point > (SELECT stable_point FROM  undo_stable_points WHERE bool_current_stable_point=1);"
		"UPDATE undo_stable_points SET bool_current_stable_point=0 WHERE bool_current_stable_point=1;"
	"END;"
"COMMIT;";

const std::string ark3d_current_database_file = "ark3d_working_db.current";
const std::string ark3d_previous_database_file = "ark3d_working_db.previous";


// construct empty model
Ark3DModel::Ark3DModel(PrimitiveBasePointer (*current_primitive_factory)(unsigned, Ark3DModel &), ConstraintEquationBasePointer (*current_constraint_factory)(unsigned, Ark3DModel &)):
CurrentPrimitiveFactory(current_primitive_factory),
CurrentConstraintFactory(current_constraint_factory),
current_selection_mask_(All),
database_(0),
current_file_name_("")
{
	// initialize an empty database
	InitializeDatabase();
}

// construct existing model from file
Ark3DModel::Ark3DModel(const std::string &file_name,PrimitiveBasePointer (*current_primitive_factory)(unsigned, Ark3DModel &), ConstraintEquationBasePointer (*current_constraint_factory)(unsigned, Ark3DModel &)):
CurrentPrimitiveFactory(current_primitive_factory),
CurrentConstraintFactory(current_constraint_factory),
current_selection_mask_(All),
database_(0),
current_file_name_(file_name)
{
	// delete the previous database file if it already exists
	if(boost::filesystem::exists(ark3d_previous_database_file))
		boost::filesystem::remove(ark3d_previous_database_file);

	// move the current database file to the previous database file if it exists
	if(boost::filesystem::exists(ark3d_current_database_file))
		boost::filesystem::rename(ark3d_current_database_file,ark3d_previous_database_file);

	// replace the working database file with the file to be opened
	boost::filesystem::copy_file(current_file_name_,ark3d_current_database_file);

	// open the working database file
	int rc = sqlite3_open(ark3d_current_database_file.c_str(), &database_);
	if( rc ){
		// an error occurred when trying to open the database
		std::string error_description = "Can't open database: " + std::string(sqlite3_errmsg(database_));
		sqlite3_close(database_);
		throw Ark3DException(error_description);
	}

	// synchronize memory to the newly opened database
	SyncToDatabase();
}

Ark3DModel::~Ark3DModel() 
{
	// close the database
	int rc = sqlite3_close(database_);
	if(rc)
	{
		// error occured when attempting to close the database
		std::cerr << "Error closing SQL Database: " << sqlite3_errmsg(database_) << std::endl;
	}
	database_ = 0;

	// let all of the primitives and constraints do some cleanup if needed before they are deleted
	map<unsigned,PrimitiveBasePointer>::iterator iter1 = primitive_list_.begin();
	while(iter1 != primitive_list_.end())
	{
		iter1->second->Erase();
		iter1++;
	}
	
	map<unsigned,ConstraintEquationBasePointer>::iterator iter2 = constraint_equation_list_.begin();
	while(iter2 != constraint_equation_list_.end())
	{
		iter2->second->Erase();
		iter2++;
	}	
	
	// clear out the lists
	dof_list_.clear(); 
	constraint_equation_list_.clear(); 
	primitive_list_.clear();
}

void Ark3DModel::InitializeDatabase()
{
	// delete the previous database file if it already exists
	if(boost::filesystem::exists(ark3d_previous_database_file))
		boost::filesystem::remove(ark3d_previous_database_file);

	// move the current database file to the previous database file if it exists
	if(boost::filesystem::exists(ark3d_current_database_file))
		boost::filesystem::rename(ark3d_current_database_file,ark3d_previous_database_file);

	int rc = sqlite3_open(ark3d_current_database_file.c_str(), &database_);
	if( rc ){
		// an error occurred when trying to open the database
		std::string error_description = "Can't open database: " + std::string(sqlite3_errmsg(database_));
		sqlite3_close(database_);
		throw Ark3DException(error_description);
	}

	// initialize the database schema
	char *zErrMsg = 0;
	rc = sqlite3_exec(database_, SQL_ark3d_database_schema.c_str(), 0, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		std::string error_description = "SQL error: " + std::string(zErrMsg);
		sqlite3_free(zErrMsg);
		throw Ark3DException(error_description);
	}
	
}

bool Ark3DModel::Save(const std::string &file_name, bool save_copy)
{
	if(file_name != "" && !save_copy)
		current_file_name_ = file_name;

	bool success = true;

	// first close the working database so that it is in a stable state
	int rc = sqlite3_close(database_);
	if(rc)
	{
		// error occured when attempting to close the database
		std::stringstream error_description;
		error_description << "Error closing SQL Database: " << sqlite3_errmsg(database_) << std::endl;	
		throw Ark3DException(error_description.str());
	}
	database_ = 0;

	// now copy the working database to the location of current_file_name_
	try{
		if(!save_copy)
		{
			if(boost::filesystem::exists(current_file_name_))
				boost::filesystem::remove(current_file_name_); // delete file if it already exists
			boost::filesystem::copy_file(ark3d_current_database_file,current_file_name_);
		} else {
			if(boost::filesystem::exists(file_name))
				boost::filesystem::remove(file_name); // delete file if it already exists
			boost::filesystem::copy_file(ark3d_current_database_file,file_name);
		}
	}
	catch (boost::filesystem::basic_filesystem_error<string> e) {
		success = false;
	}

	// finally, reopen the working database so that it can continue to be used
	rc = sqlite3_open(ark3d_current_database_file.c_str(), &database_);
	if( rc ){
		// an error occurred when trying to open the database
		std::string error_description = "Can't open database: " + std::string(sqlite3_errmsg(database_));
		sqlite3_close(database_);
		throw Ark3DException(error_description);
	}

	return success;
}

void Ark3DModel::AddConstraintEquation(const ConstraintEquationBasePointer &new_constraint_equation, bool update_database)
{
	// Add constraint equation to constraint equation vector container
	pair<map<unsigned,ConstraintEquationBasePointer>::iterator,bool> constraint_ret;
	constraint_ret = constraint_equation_list_.insert(pair<unsigned,ConstraintEquationBasePointer>(new_constraint_equation->GetID(),new_constraint_equation));
	if(constraint_ret.second && update_database) // constraint_ret.second is true if this constraint is not already in the map
		new_constraint_equation->AddToDatabase(database_);		

	// Add the primitives that this constraint depends on to the primitive map container
	vector<PrimitiveBasePointer>::const_iterator primitive_it;
	vector<PrimitiveBasePointer>::const_iterator primitive_end = new_constraint_equation->GetPrimitiveList().end();
	for ( primitive_it=new_constraint_equation->GetPrimitiveList().begin() ; primitive_it != primitive_end; primitive_it++ )
	{
		pair<map<unsigned,PrimitiveBasePointer>::iterator,bool> primitive_ret = primitive_list_.insert(pair<unsigned,PrimitiveBasePointer>((*primitive_it)->GetID(),(*primitive_it)));
		if(primitive_ret.second && update_database) // ret.second is true if this PrimitivePointer is not already in the map
			(*primitive_it)->AddToDatabase(database_); // this primitive is new to this model and needs to be added to the database
	}

	// Add DOF's to DOF map containter
	vector<DOFPointer>::const_iterator dof_it;
	vector<DOFPointer>::const_iterator dof_end = new_constraint_equation->GetDOFList().end();
	pair<map<unsigned,DOFPointer>::iterator,bool> ret;
	for ( dof_it=new_constraint_equation->GetDOFList().begin() ; dof_it != dof_end; dof_it++ )
	{
		ret = dof_list_.insert(pair<unsigned,DOFPointer>((*dof_it)->GetID(),(*dof_it)));
		if(ret.second && update_database) // ret.second is true if this DOFPointer is not already in the map
			(*dof_it)->AddToDatabase(database_); // this DOF is new to this model and needs to be added to the database
	}
	
	ApplySelectionMask(current_selection_mask_);
}

/*
void Ark3DModel::AddConstraintEquations(const std::vector<ConstraintEquationBasePointer> &new_constraint_equations)
{
	for(unsigned int current_equation = 0; current_equation < new_constraint_equations.size(); current_equation++)
		AddConstraintEquation(new_constraint_equations[current_equation]);
}
*/

void Ark3DModel::AddPrimitive(const PrimitiveBasePointer &new_primitive, bool update_database)
{
	// Add primitive to the primitive vector container
	pair<map<unsigned,PrimitiveBasePointer>::iterator,bool> primitive_ret;
	primitive_ret = primitive_list_.insert(pair<unsigned,PrimitiveBasePointer>(new_primitive->GetID(),new_primitive));
	if(primitive_ret.second && update_database) // primitive_ret.second is true if this primitive is not already in the map
		new_primitive->AddToDatabase(database_);

	// Add the primitives that this primitive depends on to the primitive map container
	vector<PrimitiveBasePointer>::const_iterator primitive_it;
	vector<PrimitiveBasePointer>::const_iterator primitive_end = new_primitive->GetPrimitiveList().end();
	for ( primitive_it=new_primitive->GetPrimitiveList().begin() ; primitive_it != primitive_end; primitive_it++ )
	{
		primitive_ret = primitive_list_.insert(pair<unsigned,PrimitiveBasePointer>((*primitive_it)->GetID(),(*primitive_it)));
		if(primitive_ret.second && update_database) // ret.second is true if this PrimitivePointer is not already in the map
			(*primitive_it)->AddToDatabase(database_); // this primitive is new to this model and needs to be added to the database
	}

	// Add DOF's that this primitive depends on to DOF map containter
	vector<DOFPointer>::const_iterator dof_it;
	vector<DOFPointer>::const_iterator dof_end = new_primitive->GetDOFList().end();
	pair<map<unsigned,DOFPointer>::iterator,bool> ret;
	for ( dof_it=new_primitive->GetDOFList().begin() ; dof_it != dof_end; dof_it++ )
	{
		ret = dof_list_.insert(pair<unsigned,DOFPointer>((*dof_it)->GetID(),(*dof_it)));
		if(ret.second && update_database) // ret.second is true if this DOFPointer is not already in the map
			(*dof_it)->AddToDatabase(database_); // this DOF is new to this model and needs to be added to the database
	}

	ApplySelectionMask(current_selection_mask_);
}

/*
void Ark3DModel::AddPrimitives(const std::vector<PrimitiveBasePointer> &new_primitives)
{
	for(unsigned int current_primitive = 0; current_primitive < new_primitives.size(); current_primitive++)
		AddPrimitive(new_primitives[current_primitive]);
}
*/

// This method solves the system of constraint equations for this model
void Ark3DModel::SolveConstraints()
{
	// only procedd if at least one constraint equation exists
	if(constraint_equation_list_.size() > 0)
	{
		unsigned int num_free_values;
		unsigned int num_fixed_values; 
		
		// create constraint equation and constraint equation weight vectors
		std::vector<GiNaC::ex> constraints;
		std::vector<double> weights;
	
		for(map<unsigned,ConstraintEquationBasePointer>::iterator constraint_it=constraint_equation_list_.begin() ; constraint_it != constraint_equation_list_.end(); constraint_it++ )
		{
				std::vector< boost::shared_ptr<GiNaC::ex> > current_constraint_list = constraint_it->second->GetConstraintList();
				
				std::vector<double> current_weight_list = constraint_it->second->GetWeightList();
	
				// loop over each expression in the constraint equation class
				for(unsigned int current_sub_equation = 0; current_sub_equation < current_constraint_list.size(); current_sub_equation++)
				{
					constraints.push_back(*(current_constraint_list[current_sub_equation]));
					weights.push_back(current_weight_list[current_sub_equation]);
				}
		}
	
		// For each dependent DOF, substitute its defining expression into all of the constraint equations that reference it
		for (map<unsigned,DOFPointer>::iterator dof_it=dof_list_.begin() ; dof_it != dof_list_.end(); dof_it++ )
		{
			if((*dof_it).second->IsDependent())
			{
				// loop over each constraint equation and substitute the dependent DOF
				for(unsigned int current_equation = 0; current_equation < constraints.size(); current_equation++)
				{
					constraints[current_equation] = constraints[current_equation].subs((*dof_it).second->GetVariable() == (*dof_it).second->GetExpression(),GiNaC::subs_options::no_pattern);
				}
			}
		}

		// create the free parameters, free_values, fixed_parameters, and fixed_values lists
		std::vector<GiNaC::symbol> free_parameters;
		std::vector<DOFPointer> free_dof_list;   // This vector will be used to update the DOF's after solving the constraint equations
		std::vector<double> free_values;
		std::vector<GiNaC::symbol> fixed_parameters;
		std::vector<double> fixed_values;
		
		for (map<unsigned,DOFPointer>::iterator dof_it=dof_list_.begin() ; dof_it != dof_list_.end(); dof_it++ )
		{
			if((*dof_it).second->IsFree())
			{	// free parameter
				free_parameters.push_back((*dof_it).second->GetVariable());
				free_dof_list.push_back((*dof_it).second);
				free_values.push_back((*dof_it).second->GetValue());
			} else if( ! (*dof_it).second->IsDependent()) 
			{	// fixed, independent parameter
				fixed_parameters.push_back((*dof_it).second->GetVariable());
				fixed_values.push_back((*dof_it).second->GetValue());
			}
		}
	
		// only continue if there are actually some free values to solve for
		if(free_values.size() > 0)
		{
			// populate the initial condition matrix using free_values
			mmcMatrix initial_free_values(free_values.size(),1);
			for(unsigned int current_free_value = 0; current_free_value < free_values.size(); current_free_value++)
				initial_free_values(current_free_value,0) = free_values[current_free_value];
		
		
			ConstraintSolver my_constraint_solver(constraints, weights, free_parameters,
																				fixed_parameters, fixed_values);
		
			mmcMatrix computed_free_values = my_constraint_solver.MinimizeMeritFunction(initial_free_values, 1000, 1e-10, 1e-15, 100, 1, &std::cerr);
		
			// Update the free DOF's with the solution
			for(unsigned int current_dof = 0; current_dof < free_dof_list.size(); current_dof++)
				free_dof_list[current_dof]->SetValue(computed_free_values(current_dof,0));
		}
	}
}


void Ark3DModel::UpdateDisplay()
{
	// Update display for all of the constraint equations
	for(map<unsigned,ConstraintEquationBasePointer>::iterator constraint_it=constraint_equation_list_.begin() ; constraint_it != constraint_equation_list_.end(); constraint_it++ )
		constraint_it->second->UpdateDisplay();

	// Update display for all of the primitives
	for(map<unsigned,PrimitiveBasePointer>::iterator primitive_it=primitive_list_.begin() ; primitive_it != primitive_list_.end(); primitive_it++ )
		primitive_it->second->UpdateDisplay();
}

void Ark3DModel::ApplySelectionMask(SelectionMask mask)
{
	current_selection_mask_ = mask;

	// Apply mask to all of the constraint equations
	for(map<unsigned,ConstraintEquationBasePointer>::iterator constraint_it=constraint_equation_list_.begin() ; constraint_it != constraint_equation_list_.end(); constraint_it++ )
		constraint_it->second->ApplySelectionMask(mask);

	// Apply mask to  all of the primitives
	for(map<unsigned,PrimitiveBasePointer>::iterator primitive_it=primitive_list_.begin() ; primitive_it != primitive_list_.end(); primitive_it++ )
		primitive_it->second->ApplySelectionMask(mask);

}

std::vector<PrimitiveBasePointer> Ark3DModel::GetSelectedPrimitives()
{
	std::vector<PrimitiveBasePointer> selected_primitives;

	for(map<unsigned,PrimitiveBasePointer>::iterator primitive_it=primitive_list_.begin() ; primitive_it != primitive_list_.end(); primitive_it++ )
	{
		if(primitive_it->second->IsSelected())
			selected_primitives.push_back(primitive_it->second);
	}

	return selected_primitives;
}

std::vector<ConstraintEquationBasePointer> Ark3DModel::GetConstraintEquations()
{
	std::vector<ConstraintEquationBasePointer> selected_constraint_equations;

	for(map<unsigned,ConstraintEquationBasePointer>::iterator constraint_it=constraint_equation_list_.begin() ; constraint_it != constraint_equation_list_.end(); constraint_it++ )
	{
		if(constraint_it->second->IsSelected())
			selected_constraint_equations.push_back(constraint_it->second);
	}

	return selected_constraint_equations;
}

void Ark3DModel::DeletePrimitive(PrimitiveBasePointer primitive_to_delete)
{
	primitive_to_delete->FlagForDeletion();
	FlagDependentsForDeletion(primitive_to_delete);
	DeleteFlagged();
}

void Ark3DModel::DeletePrimitiveNoDependancyCheck(PrimitiveBasePointer primitive_to_delete)
{
    primitive_to_delete->FlagForDeletion();
    DeleteFlagged();
}

// Flag any primitives or constraint equations for deletion that depend on this primitive
void Ark3DModel::FlagDependentsForDeletion(PrimitiveBasePointer primitive_to_delete)
{
	bool status_changed;

	// loop through all of the primitives
	for(map<unsigned,PrimitiveBasePointer>::iterator primitive_it=primitive_list_.begin() ; primitive_it != primitive_list_.end(); primitive_it++ )
	{
		status_changed = primitive_it->second->FlagForDeletionIfDependent(primitive_to_delete);
		if(status_changed)
			// recurse if this primitive is now flagged for deletion
			FlagDependentsForDeletion(primitive_it->second);
	}

	// loop through all of the constraints
	for(map<unsigned,ConstraintEquationBasePointer>::iterator constraint_it=constraint_equation_list_.begin() ; constraint_it != constraint_equation_list_.end(); constraint_it++ )
	{
		status_changed = constraint_it->second->FlagForDeletionIfDependent(primitive_to_delete);
		if(status_changed)
			// recurse if this constraint equation is now flagged for deletion
			FlagDependentsForDeletion(constraint_it->second);
	}
}

// delete all of the primitives that have been flagged for deletion
void Ark3DModel::DeleteFlagged(bool remove_from_db)
{
	map<unsigned,PrimitiveBasePointer>::iterator iter1 = primitive_list_.begin();

	while(iter1 != primitive_list_.end())
	{
		if(iter1->second->IsFlaggedForDeletion())
		{
			iter1->second->Erase();
			if(remove_from_db)
				iter1->second->RemoveFromDatabase();
			primitive_list_.erase(iter1++);
		} else {
			iter1++;
		}
	}
	
	map<unsigned,ConstraintEquationBasePointer>::iterator iter2 = constraint_equation_list_.begin();

	while(iter2 != constraint_equation_list_.end())
	{
		if(iter2->second->IsFlaggedForDeletion())
		{
			iter2->second->Erase();
			if(remove_from_db)
				iter2->second->RemoveFromDatabase();
			constraint_equation_list_.erase(iter2++);
		} else {
			iter2++;
		}
	}

	// there may now be some DOF's that are not needed, go ahead and delete them
	DeleteUnusedDOFs(false /* don't attempt to remove from the database */);
}

void Ark3DModel::DeleteSelected()
{
	// loop through all of the primitives
	for(map<unsigned,PrimitiveBasePointer>::iterator primitive_it=primitive_list_.begin() ; primitive_it != primitive_list_.end(); primitive_it++ )
	{
		if(primitive_it->second->IsSelected())
		{
			primitive_it->second->FlagForDeletion();
			FlagDependentsForDeletion(primitive_it->second);
		}
	}

	// loop through all of the constraints
	for(map<unsigned,ConstraintEquationBasePointer>::iterator constraint_it=constraint_equation_list_.begin() ; constraint_it != constraint_equation_list_.end(); constraint_it++ )
	{
		if(constraint_it->second->IsSelected())
		{
			constraint_it->second->FlagForDeletion();
			FlagDependentsForDeletion(constraint_it->second);
		}
	}

	DeleteFlagged();
}

DOFPointer Ark3DModel::FetchDOF(unsigned id)
{
	map<unsigned,DOFPointer>::iterator dof_it = dof_list_.find(id);
	if(dof_it != dof_list_.end())
	{
		// dof exists, synchronize it to the database and return it
		(dof_it->second)->SyncToDatabase(*this);
		return(dof_it->second);

	} else {
		// dof object does not exist, need to create it from the database
		DOFPointer new_dof = DOFFactory(id);

		// add this DOF to the DOF map container so that it is not reconstructed upon a subsiquent call to FetchDOF
		dof_list_.insert(pair<unsigned,DOFPointer>(id,new_dof));

		return new_dof;
	}
}

DOFPointer Ark3DModel::DOFFactory(unsigned id)
{
	// grab the table name from the database so we now exactly which class needs to be created
	int rc;
	sqlite3_stmt *statement;
	stringstream table_name_stream;
	string table_name;

	stringstream sql_command;
	sql_command << "SELECT * FROM dof_list WHERE id=" << id << ";";

	rc = sqlite3_prepare(database_, sql_command.str().c_str(), -1, &statement, 0);
	if( rc!=SQLITE_OK ){
		stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(database_);
		throw Ark3DException(error_description.str());
	}

	rc = sqlite3_step(statement);

	if(rc == SQLITE_ROW) {
		// row exist, store the values to initialize this object
		table_name_stream << sqlite3_column_text(statement,1);
		table_name = table_name_stream.str();
	} else {
		// the requested row does not exist in the database
		sqlite3_finalize(statement);

		stringstream error_description;
		error_description << "SQLite rowid " << id << " in table dof_list does not exist";
		throw Ark3DException(error_description.str());
	}

	rc = sqlite3_step(statement);
	if( rc!=SQLITE_DONE ){
		// sql statement didn't finish properly, some error must to have occured
		stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(database_);
		throw Ark3DException(error_description.str());
	}
	
	rc = sqlite3_finalize(statement);
	if( rc!=SQLITE_OK ){
		stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(database_);
		throw Ark3DException(error_description.str());
	}

	// now generate the object based on the table name
	DOFPointer result;

	if(table_name == "independent_dof_list")
	{
		result.reset(new IndependentDOF(id,*this));
	}
	else if(table_name == "dependent_dof_list"){
		result.reset(new DependentDOF(id,*this));
	}
	else {
		throw Ark3DException("Ark3D::DOFFactory: Unable to determine type based on database table name " + table_name);
	}

	return result;
}

PrimitiveBasePointer Ark3DModel::PrimitiveFactory(unsigned id, Ark3DModel &ark3d_model)
{
	// grab the table name from the database so we now exactly which class needs to be created
	int rc;
	sqlite3_stmt *statement;
	stringstream table_name_stream;
	string table_name;

	stringstream sql_command;
	sql_command << "SELECT * FROM primitive_list WHERE id=" << id << ";";

	rc = sqlite3_prepare(ark3d_model.GetDatabase(), sql_command.str().c_str(), -1, &statement, 0);
	if( rc!=SQLITE_OK ){
		stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(ark3d_model.GetDatabase());
		throw Ark3DException(error_description.str());
	}

	rc = sqlite3_step(statement);

	if(rc == SQLITE_ROW) {
		// row exist, store the values to initialize this object
		table_name_stream << sqlite3_column_text(statement,1);
		table_name = table_name_stream.str();
	} else {
		// the requested row does not exist in the database
		sqlite3_finalize(statement);

		stringstream error_description;
		error_description << "SQLite rowid " << id << " in table " << table_name << " does not exist";
		throw Ark3DException(error_description.str());
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

	// now generate the object based on the table name
	PrimitiveBasePointer result;

	if(table_name == "arc2d_list")
	{
		result.reset(new Arc2D(id,ark3d_model));
	}
	else if(table_name == "line2d_list"){
		result.reset(new Line2D(id,ark3d_model));
	}
	else if(table_name == "point_list"){
		result.reset(new Point(id,ark3d_model));
	}
	else if(table_name == "point2d_list"){
		result.reset(new Point2D(id,ark3d_model));
	}
	else if(table_name == "sketch_plane_list"){
		result.reset(new SketchPlane(id,ark3d_model));
	}
	else if(table_name == "vector_list"){
		result.reset(new Vector(id,ark3d_model));
	}
	else {
		throw Ark3DException("Ark3D::PrimitiveFactory: Unable to determine type based on database table name " + table_name);	
	}

	return result;
}

ConstraintEquationBasePointer Ark3DModel::ConstraintFactory(unsigned id, Ark3DModel &ark3d_model)
{

	// grab the table name from the database so we now exactly which class needs to be created
	int rc;
	sqlite3_stmt *statement;
	stringstream table_name_stream;
	string table_name;

	stringstream sql_command;
	sql_command << "SELECT * FROM constraint_equation_list WHERE id=" << id << ";";

	rc = sqlite3_prepare(ark3d_model.GetDatabase(), sql_command.str().c_str(), -1, &statement, 0);
	if( rc!=SQLITE_OK ){
		stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(ark3d_model.GetDatabase());
		throw Ark3DException(error_description.str());
	}

	rc = sqlite3_step(statement);

	if(rc == SQLITE_ROW) {
		// row exist, store the values to initialize this object
		table_name_stream << sqlite3_column_text(statement,1);
		table_name = table_name_stream.str();
	} else {
		// the requested row does not exist in the database
		sqlite3_finalize(statement);

		stringstream error_description;
		error_description << "SQLite rowid " << id << " in table " << table_name << " does not exist";
		throw Ark3DException(error_description.str());
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

	// now generate the object based on the table name
	ConstraintEquationBasePointer result;

	if(table_name == "angle_line2d_list"){
		result.reset(new AngleLine2D(id,ark3d_model));
	}
	else if(table_name == "distance_point2d_list"){
		result.reset(new DistancePoint2D(id,ark3d_model));
	}
	else if(table_name == "distance_pointline2d_list"){
		result.reset(new DistancePointLine2D(id,ark3d_model));
	}
	else if(table_name == "parallel_line2d_list"){
		result.reset(new ParallelLine2D(id,ark3d_model));
	}
	else if(table_name == "tangent_edge2d_list"){
		result.reset(new TangentEdge2D(id,ark3d_model));
	}
    else if(table_name == "distance_pointline2d_list"){
        result.reset(new DistancePointLine2D(id,ark3d_model));
    }
	else {
		throw Ark3DException("Ark3D::ConstraintFactory: Unable to determine type based on database table name " + table_name);
	}

	return result;
}

// delete all unneeded DOF's in the dof_list_ container
void Ark3DModel::DeleteUnusedDOFs(bool remove_from_db)
{
	// first flag all of the DOF's for deletion
	for (map<unsigned,DOFPointer>::iterator dof_it=dof_list_.begin() ; dof_it != dof_list_.end(); dof_it++ )
		(*dof_it).second->FlagForDeletion();

	// loop through all of the primitives and constraints and unflag any DOF's that they depend on
	// loop over the primitives
	for(map<unsigned,PrimitiveBasePointer>::iterator primitive_it=primitive_list_.begin() ; primitive_it != primitive_list_.end(); primitive_it++ )
	{
		vector<DOFPointer>::const_iterator dof_it;
		vector<DOFPointer>::const_iterator dof_end = primitive_it->second->GetDOFList().end();
		for (dof_it=primitive_it->second->GetDOFList().begin() ; dof_it != dof_end; dof_it++ )
		{
			map<unsigned,DOFPointer>::iterator find_dof_it = dof_list_.find((*dof_it)->GetID());
			if(find_dof_it != dof_list_.end())
			{
				// dof exists, unflag it for deletion
				find_dof_it->second->UnflagForDeletion();
			} else {
				// dof does not exist, throw an exception
				throw Ark3DException("Expected DOF does not exist in dof_list_ container.");
			}
		}
	}

	// loop over the constraints
	for(map<unsigned,ConstraintEquationBasePointer>::iterator constraint_it=constraint_equation_list_.begin() ; constraint_it != constraint_equation_list_.end(); constraint_it++ )
	{
		vector<DOFPointer>::const_iterator dof_it;
		vector<DOFPointer>::const_iterator dof_end = constraint_it->second->GetDOFList().end();
		for (dof_it=constraint_it->second->GetDOFList().begin() ; dof_it != dof_end; dof_it++ )
		{
			map<unsigned,DOFPointer>::iterator find_dof_it = dof_list_.find((*dof_it)->GetID());
			if(find_dof_it != dof_list_.end())
			{
				// dof exists, unflag it for deletion
				find_dof_it->second->UnflagForDeletion();
			} else {
				// dof does not exist, throw an exception
				throw Ark3DException("Expected DOF does not exist in dof_list_ container.");
			}
		}
	}

	// Finally, delete any DOF's that are still flagged for deletion
	map<unsigned,DOFPointer>::iterator dof_it = dof_list_.begin();
	while(dof_it != dof_list_.end())
	{
		if(dof_it->second->IsFlaggedForDeletion())
		{
			if(remove_from_db)
				dof_it->second->RemoveFromDatabase();
			dof_list_.erase(dof_it++);
		} else {
			dof_it++;
		}
	}
}

// synchronize the primitive, constraint, and DOF lists to the database (used to implement file open and undo/redo)
void Ark3DModel::SyncToDatabase()
{
	// set the next_id_number_ variables for the PrimitiveBase and DOF classes (this is a static member)
	SetMaxIDNumbers();

	// Step 1: Flag all primitives and constraint equations for deletion
	for (map<unsigned,PrimitiveBasePointer>::iterator primitive_it=primitive_list_.begin() ; primitive_it != primitive_list_.end(); primitive_it++ )
		(*primitive_it).second->FlagForDeletion();

	for (map<unsigned,ConstraintEquationBasePointer>::iterator constraint_it=constraint_equation_list_.begin() ; constraint_it != constraint_equation_list_.end(); constraint_it++ )
		(*constraint_it).second->FlagForDeletion();

	// Step 2: Fetch all primitives and constraints that are defined in the database

	// synchronize the primitive_list_ container to the database

	char *zErrMsg = 0;
	int rc;
	sqlite3_stmt *statement;

	stringstream sql_command;
	sql_command << "SELECT * FROM primitive_list;";

	rc = sqlite3_prepare(GetDatabase(), sql_command.str().c_str(), -1, &statement, 0);
	if( rc!=SQLITE_OK ){
		stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(GetDatabase());
		throw Ark3DException(error_description.str());
	}

	rc = sqlite3_step(statement);
	
	int current_primitive_id;
	PrimitiveBasePointer current_primitive;
	while(rc == SQLITE_ROW) {
		current_primitive_id = sqlite3_column_int(statement,0);

		// get the primitive (it will be automatically created from the database if it doesn't already exist)
		current_primitive = FetchPrimitive<PrimitiveBase>(current_primitive_id);
		
		if(current_primitive->IsFlaggedForDeletion())
		{
			// this primitive already existed in memory, all we need to do is sync it to the database
			current_primitive->SyncToDatabase(*this);
			current_primitive->UnflagForDeletion(); // don't need to delete this primitive since it exists in the database
		} else {
			// this primitive was not in memory, need to add it to the model
			// don't update the database since we are in the process of syncing to the database
			AddPrimitive(current_primitive, false /*bool update_database */);
		}

		rc = sqlite3_step(statement);
	}

	if( rc!=SQLITE_DONE ){
		// sql statement didn't finish properly, some error must to have occured
		stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(GetDatabase());
		throw Ark3DException(error_description.str());
	}

	rc = sqlite3_finalize(statement);
	if( rc!=SQLITE_OK ){
		stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(GetDatabase());
		throw Ark3DException(error_description.str());
	}


	// synchronize the constraint_equation_list_ container to the database
	sql_command.str(""); // clear the contents of the string stream
	sql_command << "SELECT * FROM constraint_equation_list;";

	rc = sqlite3_prepare(GetDatabase(), sql_command.str().c_str(), -1, &statement, 0);
	if( rc!=SQLITE_OK ){
		stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(GetDatabase());
		throw Ark3DException(error_description.str());
	}

	rc = sqlite3_step(statement);
	
	int current_constraint_id;
	ConstraintEquationBasePointer current_constraint;
	while(rc == SQLITE_ROW) {
		current_constraint_id = sqlite3_column_int(statement,0);

		// get the constraint (it will be automatically created from the database if it doesn't already exist)
		current_constraint = FetchConstraint<ConstraintEquationBase>(current_constraint_id);

		if(current_constraint->IsFlaggedForDeletion())
		{
			// this primitive already existed in memory, all we need to do is sync it to the database
			current_constraint->SyncToDatabase(*this);
			current_constraint->UnflagForDeletion(); // don't need to delete this primitive since it exists in the database
		} else {
			// this primitive was not in memory, need to add it to the model
			// don't update the database since we are in the process of reading the model from the database
			AddConstraintEquation(current_constraint, false /* bool update_database */);
		}

		rc = sqlite3_step(statement);
	}

	if( rc!=SQLITE_DONE ){
		// sql statement didn't finish properly, some error must to have occured
		stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(GetDatabase());
		throw Ark3DException(error_description.str());
	}

	rc = sqlite3_finalize(statement);
	if( rc!=SQLITE_OK ){
		stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(GetDatabase());
		throw Ark3DException(error_description.str());
	}


	// Step 3: Delete all primitives and constraints that are flagged for deletion
	DeleteFlagged(false /* delete_from_db */);
}

void Ark3DModel::SetMaxIDNumbers()
{
	// need to set the next_id_number_ members of the PrimitiveBase and DOF classes
	char *zErrMsg = 0;
	int rc;
	sqlite3_stmt *statement;

	int max_primitive, max_constraint, next_primitive;
	
	std::string sql_command_primitive = "SELECT max(id) AS id FROM primitive_list;";
	std::string sql_command_constraint = "SELECT max(id) AS id FROM constraint_equation_list;";
	std::string sql_command_dof = "SELECT max(id) AS id FROM dof_list;";

	rc = sqlite3_prepare(GetDatabase(), sql_command_primitive.c_str(), -1, &statement, 0);
	if( rc!=SQLITE_OK ){
		std::stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(GetDatabase());
		throw Ark3DException(error_description.str());
	}

	rc = sqlite3_step(statement);

	if(rc == SQLITE_ROW) {
		// set the next_primitive value based on the max id number in the database
		max_primitive = sqlite3_column_int(statement,0);
	} else {
		// the requested row does not exist in the database so there are no existing primitive entities
		// set max_primitve to 0
		max_primitive=0;
	}

	rc = sqlite3_finalize(statement);

	// make sure the finalize statement didn't generate an error
	if( rc!=SQLITE_OK ){
		std::stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(GetDatabase());
		throw Ark3DException(error_description.str());
	}

	rc = sqlite3_prepare(GetDatabase(), sql_command_constraint.c_str(), -1, &statement, 0);
	if( rc!=SQLITE_OK ){
		std::stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(GetDatabase());
		throw Ark3DException(error_description.str());
	}

	rc = sqlite3_step(statement);

	if(rc == SQLITE_ROW) {
		// set the max_constraint value from the database
		max_constraint = sqlite3_column_int(statement,0);
	} else {
		// the requested row does not exist in the database so there are no existing constraint entities
		// set max_constraint to 0
		max_constraint=0;
	}

	rc = sqlite3_finalize(statement);

	// make sure the finalize statement didn't generate an error
	if( rc!=SQLITE_OK ){
		std::stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(GetDatabase());
		throw Ark3DException(error_description.str());
	}

	next_primitive = max_constraint > max_primitive ? max_constraint+1 : max_primitive+1;
	
	PrimitiveBase::SetNextID(next_primitive);
	cerr << "next primitive = " << next_primitive << endl;	

	rc = sqlite3_prepare(GetDatabase(), sql_command_dof.c_str(), -1, &statement, 0);
	if( rc!=SQLITE_OK ){
		std::stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(GetDatabase());
		throw Ark3DException(error_description.str());
	}

	rc = sqlite3_step(statement);

	if(rc == SQLITE_ROW) {
		// set the next_id_number_ value based on the max id number in the database
		DOF::SetNextID(sqlite3_column_int(statement,0)+1);
		
		cerr << "next dof = " << sqlite3_column_int(statement,0)+1 << endl;
	} else {
		// the requested row does not exist in the database so there are no existing primitive entities
		// set next_id_number_ to 1
		DOF::SetNextID(1);

		cerr << "next dof = " << 1 << endl;
	}

	rc = sqlite3_finalize(statement);

	// make sure the finalize statement didn't generate an error
	if( rc!=SQLITE_OK ){
		std::stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(GetDatabase());
		throw Ark3DException(error_description.str());
	}
}


void Ark3DModel::MarkStablePoint(const std::string &description)
{
	// Add a new stable point, use the maximum id from the undo_redo_list table as the stable id
	// Do add a stable point if the undo_redo_list table is empty or if this stable point has has already been defined

	// first get the max max undo_redo id and the max stable point
	int max_undo_redo_id, max_stable_point;
	bool undo_redo_list_empty = false;
	bool stable_point_already_exists = false;
	bool stable_point_list_empty = false;

	char *zErrMsg = 0;
	int rc;
	sqlite3_stmt *statement;
	
	std::string sql_command_undo_redo_list = "SELECT max(id) AS id FROM undo_redo_list;";
	std::string sql_command_undo_stable_points = "SELECT max(stable_point) AS stable_point FROM undo_stable_points;";

	rc = sqlite3_prepare(GetDatabase(), sql_command_undo_redo_list.c_str(), -1, &statement, 0);
	if( rc!=SQLITE_OK ){
		std::stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(GetDatabase());
		throw Ark3DException(error_description.str());
	}

	rc = sqlite3_step(statement);

	if(rc == SQLITE_ROW) {
		// undo_redo_list is not empty, record the max id
		max_undo_redo_id = sqlite3_column_int(statement,0);		
	} else {
		// the undo_redo_list is empty
		undo_redo_list_empty = true;
	}

	rc = sqlite3_finalize(statement);

	// make sure the finalize statement didn't generate an error
	if( rc!=SQLITE_OK ){
		std::stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(GetDatabase());
		throw Ark3DException(error_description.str());
	}


	rc = sqlite3_prepare(GetDatabase(), sql_command_undo_stable_points.c_str(), -1, &statement, 0);
	if( rc!=SQLITE_OK ){
		std::stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(GetDatabase());
		throw Ark3DException(error_description.str());
	}

	rc = sqlite3_step(statement);

	if(rc == SQLITE_ROW) {
		// the stable point list is not empty, record the max stable point
		max_stable_point = sqlite3_column_int(statement,0);
		
		// check to see if this stable point has already been defined
		if(!undo_redo_list_empty && max_undo_redo_id == max_stable_point)
			stable_point_already_exists = true;
	
	} else {
		// the undo_redo_list is empty
		stable_point_list_empty = true;
	}

	rc = sqlite3_finalize(statement);

	// make sure the finalize statement didn't generate an error
	if( rc!=SQLITE_OK ){
		std::stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(GetDatabase());
		throw Ark3DException(error_description.str());
	}
 
	// now add the row for this new stable point if it doesn't already exist and the unde/redo list is not empty
	if(!undo_redo_list_empty && !stable_point_already_exists)
	{
		// define the update statement
		char *sql_statement = sqlite3_mprintf("INSERT INTO undo_stable_points(stable_point,bool_current_stable_point,description) VALUES(%d,0,'%q');",max_undo_redo_id,description.c_str());
		
		// do the database update
		char *zErrMsg = 0;
		int rc = sqlite3_exec(database_, sql_statement, 0, 0, &zErrMsg);
		if( rc!=SQLITE_OK ){
			std::string error_description = "SQL error: " + std::string(zErrMsg);
			sqlite3_free(zErrMsg);
			throw Ark3DException(error_description);
		}

		sqlite3_free(sql_statement);
	}
}

bool Ark3DModel::IsUndoAvailable(string &description)
{
	int current_stable_point, new_stable_point, current_row_id;
	return IsUndoAvailable(current_stable_point, new_stable_point, current_row_id, description);
}

bool Ark3DModel::IsRedoAvailable(string &description)
{
	int current_stable_point, new_stable_point, current_row_id;
	return IsRedoAvailable(current_stable_point, new_stable_point, current_row_id, description);
}

bool Ark3DModel::IsUndoAvailable(int &current_stable_point, int &new_stable_point, int &current_row_id /* current row id of table undo_stable_points */, string &description)
{
	stringstream temp_stream;

	char *zErrMsg = 0;
	int rc;
	sqlite3_stmt *statement;
	
	string sql_command = "SELECT * FROM undo_stable_points WHERE bool_current_stable_point=1;";

	rc = sqlite3_prepare(database_, sql_command.c_str(), -1, &statement, 0);
	if( rc!=SQLITE_OK ){
		stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(database_);
		throw Ark3DException(error_description.str());
	}

	rc = sqlite3_step(statement);

	if(rc == SQLITE_ROW) {
		// row exists
		
		current_row_id = sqlite3_column_int(statement,0);
		current_stable_point = sqlite3_column_int(statement,1);
		temp_stream.str("");
		temp_stream << sqlite3_column_text(statement,3);
		description = temp_stream.str();

		rc = sqlite3_finalize(statement);
		if( rc!=SQLITE_OK ){
			stringstream error_description;
			error_description << "SQL error: " << sqlite3_errmsg(database_);
			throw Ark3DException(error_description.str());
		}

	} else {
		// the requested row does not exist in the database
		// we are at the end of the undo/redo list, retrieve that last row in undo_stable_points to determine if and undo is available
		
		// must first finalize the previous sql statement
		rc = sqlite3_finalize(statement);
		if( rc!=SQLITE_OK ){
			stringstream error_description;
			error_description << "SQL error: " << sqlite3_errmsg(database_);
			throw Ark3DException(error_description.str());
		}
		
		// Check to insure that the last stable point int the table undo_stable_points matches the last id in the undo_redo_table
		// an undo cannot be performed if the model is not currently at a stable point
		sql_command = "SELECT * FROM undo_stable_points WHERE stable_point=(SELECT max(stable_point) AS stable_point FROM undo_stable_points);";
		rc = sqlite3_prepare(database_, sql_command.c_str(), -1, &statement, 0);
		if( rc!=SQLITE_OK ){
			stringstream error_description;
			error_description << "SQL error: " << sqlite3_errmsg(database_);
			throw Ark3DException(error_description.str());
		}
	
		rc = sqlite3_step(statement);
	
		if(rc == SQLITE_ROW) {
			// row exists
			
			current_row_id = sqlite3_column_int(statement,0);
			current_stable_point = sqlite3_column_int(statement,1);
			temp_stream.str("");
			temp_stream << sqlite3_column_text(statement,3);
			description = temp_stream.str();

			rc = sqlite3_finalize(statement);
			if( rc!=SQLITE_OK ){
				stringstream error_description;
				error_description << "SQL error: " << sqlite3_errmsg(database_);
				throw Ark3DException(error_description.str());
			}

			// make sure that current_row_id matches the largest id in the undo_redo_table
			sql_command = "SELECT max(id) AS id FROM undo_redo_list;";
			rc = sqlite3_prepare(database_, sql_command.c_str(), -1, &statement, 0);
			if( rc!=SQLITE_OK ){
				stringstream error_description;
				error_description << "SQL error: " << sqlite3_errmsg(database_);
				throw Ark3DException(error_description.str());
			}
		
			rc = sqlite3_step(statement);
		
			if(rc == SQLITE_ROW) {
				// row exists
				
				int max_undo_redo_table_id = sqlite3_column_int(statement,0);
	
				rc = sqlite3_finalize(statement);
				if( rc!=SQLITE_OK ){
					stringstream error_description;
					error_description << "SQL error: " << sqlite3_errmsg(database_);
					throw Ark3DException(error_description.str());
				}
	
				if(max_undo_redo_table_id != current_stable_point)
				{
					// The model is not currently at a stable point, cannot perform undo operation
					return false;
				}

			} else {
				// row doesn't exist (the undo_redo_table is emtpy), this is an error condition since there is a stable point defined
				rc = sqlite3_finalize(statement);
				throw Ark3DException("Ark3DModel Error: The undo_redo_table is empty while a stable point is defined.);");	
				return false;
			}


		} else {
			// undo_stable_points table is empty so undo is not available

			// must first finalize the previous sql statement
			rc = sqlite3_finalize(statement);
			if( rc!=SQLITE_OK ){
				stringstream error_description;
				error_description << "SQL error: " << sqlite3_errmsg(database_);
				throw Ark3DException(error_description.str());
			}
			return false;
		}
	}

	if(current_row_id == 1)
	{
		// we are already at the first stable point, no more undos are available
		return false;
	} else {
		// get teh new stalbe point
		temp_stream.str("");
		temp_stream << "SELECT stable_point FROM undo_stable_points WHERE id=" << current_row_id-1 << ";";

		rc = sqlite3_prepare(database_, temp_stream.str().c_str(), -1, &statement, 0);
		if( rc!=SQLITE_OK ){
			stringstream error_description;
			error_description << "SQL error: " << sqlite3_errmsg(database_);
			throw Ark3DException(error_description.str());
		}
	
		rc = sqlite3_step(statement);
	
		if(rc == SQLITE_ROW) {
			// row exists
			
			new_stable_point = sqlite3_column_int(statement,0);
	
			rc = sqlite3_finalize(statement);
			if( rc!=SQLITE_OK ){
				stringstream error_description;
				error_description << "SQL error: " << sqlite3_errmsg(database_);
				throw Ark3DException(error_description.str());
			}
	
			// undo exists and has been fully defined
			return true;
	
		} else {
			// value doesn't exist, this is an error condition 
			rc = sqlite3_finalize(statement);
			throw Ark3DException("Ark3DModel Error: Inconsistant undo_stable_points table.");
			return false;
		}
	}
}

bool Ark3DModel::IsRedoAvailable(int &current_stable_point, int &new_stable_point, int &current_row_id /* current row id of table undo_stable_points */, string &description)
{
	stringstream temp_stream;

	char *zErrMsg = 0;
	int rc;
	sqlite3_stmt *statement;
	
	string sql_command = "SELECT * FROM undo_stable_points WHERE bool_current_stable_point=1;";

	rc = sqlite3_prepare(database_, sql_command.c_str(), -1, &statement, 0);
	if( rc!=SQLITE_OK ){
		stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(database_);
		throw Ark3DException(error_description.str());
	}

	rc = sqlite3_step(statement);

	if(rc == SQLITE_ROW) {
		// row exists
		
		current_row_id = sqlite3_column_int(statement,0);
		current_stable_point = sqlite3_column_int(statement,1);

		rc = sqlite3_finalize(statement);
		if( rc!=SQLITE_OK ){
			stringstream error_description;
			error_description << "SQL error: " << sqlite3_errmsg(database_);
			throw Ark3DException(error_description.str());
		}

	} else {
		// the requested row does not exist in the database
		// we are at the end of the undo/redo list, there is no redo available
		
		// must first finalize the previous sql statement
		rc = sqlite3_finalize(statement);
		if( rc!=SQLITE_OK ){
			stringstream error_description;
			error_description << "SQL error: " << sqlite3_errmsg(database_);
			throw Ark3DException(error_description.str());
		}
		
		return false;
	}

	// get the new stalbe point
	temp_stream.str("");
	temp_stream << "SELECT stable_point FROM undo_stable_points WHERE id=" << current_row_id+1 << ";";

	rc = sqlite3_prepare(database_, temp_stream.str().c_str(), -1, &statement, 0);
	if( rc!=SQLITE_OK ){
		stringstream error_description;
		error_description << "SQL error: " << sqlite3_errmsg(database_);
		throw Ark3DException(error_description.str());
	}

	rc = sqlite3_step(statement);

	if(rc == SQLITE_ROW) {
		// row exists
		
		new_stable_point = sqlite3_column_int(statement,0);
		temp_stream << sqlite3_column_text(statement,3);
		description = temp_stream.str();

		rc = sqlite3_finalize(statement);
		if( rc!=SQLITE_OK ){
			stringstream error_description;
			error_description << "SQL error: " << sqlite3_errmsg(database_);
			throw Ark3DException(error_description.str());
		}

		// undo exists and has been fully defined
		return true;

	} else {
		// no new stable points available, model is up to date 
		rc = sqlite3_finalize(statement);
		if( rc!=SQLITE_OK ){
			stringstream error_description;
			error_description << "SQL error: " << sqlite3_errmsg(database_);
			throw Ark3DException(error_description.str());
		}

		return false;
	}
}

bool Ark3DModel::Undo()
{
	// first use the IsUndoAvailable(..) method to get some necessary information
	int current_stable_point, new_stable_point, current_row_id;
	string description;
	bool undo_available = IsUndoAvailable(current_stable_point, new_stable_point, current_row_id, description);

	if(undo_available)
	{

		cerr << "new_stable_point = " << new_stable_point << ", current_stable_point = " << current_stable_point << endl;
	
		// retrieve the appropriate SQL commands to undo from the database and execute each one
		char *zErrMsg = 0;
		int rc;
		sqlite3_stmt *statement;
		
		stringstream sql_command;
		sql_command << "SELECT undo FROM undo_redo_list WHERE (id > " << new_stable_point << " AND id <= " << current_stable_point << ") ORDER BY id DESC;";
	
		rc = sqlite3_prepare(database_, sql_command.str().c_str(), -1, &statement, 0);
		if( rc!=SQLITE_OK ){
			stringstream error_description;
			error_description << "SQL error: " << sqlite3_errmsg(database_);
			throw Ark3DException(error_description.str());
		}
	
		rc = sqlite3_step(statement);
			
		stringstream current_undo_command;
		vector<string> undo_command_list;	


		while(rc == SQLITE_ROW) {
			//retrieve the undo command	
			current_undo_command.str("");
			current_undo_command << sqlite3_column_text(statement,0);
			undo_command_list.push_back(current_undo_command.str());

			// go to the next row in the results
			rc = sqlite3_step(statement);
		}
	
		if( rc!=SQLITE_DONE ){
			// sql statement didn't finish properly, some error must to have occured
			stringstream error_description;
			error_description << "SQL error: " << sqlite3_errmsg(database_);
			throw Ark3DException(error_description.str());
		}
	
		rc = sqlite3_finalize(statement);
		if( rc!=SQLITE_OK ){
			stringstream error_description;
			error_description << "SQL error: " << sqlite3_errmsg(database_);
			throw Ark3DException(error_description.str());
		}
	
		// loop through all of the undo commands and excecute each one
		vector<string>::iterator it_undo = undo_command_list.begin();
		while(it_undo != undo_command_list.end())
		{
			//cout << "Current Undo Command: " << it_undo->c_str() << endl;

			// execute the redo command
			rc = sqlite3_exec(database_, it_undo->c_str(), 0, 0, &zErrMsg);
			if( rc!=SQLITE_OK ){
				std::string error_description = "Ark3DModel error: SQL error in replaying redo commands. SQL error: " + std::string(zErrMsg);
				sqlite3_free(zErrMsg);
				throw Ark3DException(error_description);
			}
			
			it_undo++;
		}

		// update the current stable point to reflect the undo operation
		sql_command.str("");
		sql_command << "UPDATE undo_stable_points SET bool_current_stable_point=1 WHERE id=" << current_row_id-1 << ";";
		rc = sqlite3_exec(database_, sql_command.str().c_str(), 0, 0, &zErrMsg);
		if( rc!=SQLITE_OK ){
			std::string error_description = "SQL error: " + std::string(zErrMsg);
			sqlite3_free(zErrMsg);
			throw Ark3DException(error_description);
		}

		// the final step is to synchronize the model to the current database
		SyncToDatabase();

		return true;
	} else {
		// no undo operation is avialable
		return false;	
	}
}

bool Ark3DModel::Redo()
{
	// first use the IsRedoAvailable(..) method to get some necessary information
	int current_stable_point, new_stable_point, current_row_id;
	string description;
	bool redo_available = IsRedoAvailable(current_stable_point, new_stable_point, current_row_id, description);

	if(redo_available)
	{
		// retrieve the appropriate SQL commands to redo from the database and execute each one
		char *zErrMsg = 0;
		int rc;
		sqlite3_stmt *statement;
		
		stringstream sql_command;
		sql_command << "SELECT redo FROM undo_redo_list WHERE (id <= " << new_stable_point << " AND id > " << current_stable_point << ") ORDER BY id ASC;";
	
		rc = sqlite3_prepare(database_, sql_command.str().c_str(), -1, &statement, 0);
		if( rc!=SQLITE_OK ){
			stringstream error_description;
			error_description << "SQL error: " << sqlite3_errmsg(database_);
			throw Ark3DException(error_description.str());
		}
	
		rc = sqlite3_step(statement);
		
		stringstream current_redo_command;
		std::vector<string> redo_command_list;
		
		while(rc == SQLITE_ROW) {
			//store the redo command	
			current_redo_command.str("");
			current_redo_command << sqlite3_column_text(statement,0);
			redo_command_list.push_back(current_redo_command.str());

			// go to the next row in the results
			rc = sqlite3_step(statement);
		}
	
		if( rc!=SQLITE_DONE ){
			// sql statement didn't finish properly, some error must to have occured
			stringstream error_description;
			error_description << "SQL error: " << sqlite3_errmsg(database_);
			throw Ark3DException(error_description.str());
		}
	
		rc = sqlite3_finalize(statement);
		if( rc!=SQLITE_OK ){
			stringstream error_description;
			error_description << "SQL error: " << sqlite3_errmsg(database_);
			throw Ark3DException(error_description.str());
		}

		// loop through all of the redo commands and excecute each one
		vector<string>::iterator it_redo = redo_command_list.begin();
		while(it_redo != redo_command_list.end())
		{
			//cout << "Current Redo Command: " << it_redo->c_str() << endl;

			// execute the redo command
			rc = sqlite3_exec(database_, it_redo->c_str(), 0, 0, &zErrMsg);
			if( rc!=SQLITE_OK ){
				std::string error_description = "Ark3DModel error: SQL error in replaying redo commands. SQL error: " + std::string(zErrMsg);
				sqlite3_free(zErrMsg);
				throw Ark3DException(error_description);
			}
			
			it_redo++;
		}

		// update the current stable point to reflect the redo operation
		sql_command.str("");
		sql_command << "UPDATE undo_stable_points SET bool_current_stable_point=1 WHERE id=" << current_row_id+1 << ";";
		rc = sqlite3_exec(database_, sql_command.str().c_str(), 0, 0, &zErrMsg);
		if( rc!=SQLITE_OK ){
			std::string error_description = "SQL error: " + std::string(zErrMsg);
			sqlite3_free(zErrMsg);
			throw Ark3DException(error_description);
		}

		// the final step is to synchronize the model to the current database
		SyncToDatabase();

		return true;
	} else {
		// no undo operation is avialable
		return false;	
	}
}

bool Ark3DModel::ExportDXF(const std::string &file_name)
{
	bool success = true;
	dimeOutput dime_output;

	// delete any file that already exists with the same name
	try{
		if(boost::filesystem::exists(file_name))
			boost::filesystem::remove(file_name); 
	}
	catch (boost::filesystem::basic_filesystem_error<string> e) {
		success = false;
	}

	if(success)
	{	
		// open the dxf file for writing
		success = dime_output.setFilename(file_name.c_str()); // file is opened for writing
	}

	if(success)
	{
		// Create a dime model to add all of the primitives to
		dimeModel dime_model;

		// add the entities section.
		dimeEntitiesSection * entities = new dimeEntitiesSection;
		dime_model.insertSection(entities);
		
		// loop through all of the primitives and give each one a chance to generate a dimeEntity
		dimeEntity *current_dime_entity;
		map<unsigned,PrimitiveBasePointer>::iterator iter1 = primitive_list_.begin();
		while(iter1 != primitive_list_.end())
		{
			current_dime_entity = iter1->second->GenerateDimeEntity();
			if(current_dime_entity != 0 )
				dime_model.addEntity(current_dime_entity);
			
			iter1++;
		}
	
		// write the actual dxf file
		dime_model.write(&dime_output);
	}

	return success;
}