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
#include "Ark3DModel.h"

using namespace std;

const std::string SQL_ark3d_database_schema = "BEGIN;"
												"CREATE TABLE dof_list (id INTEGER PRIMARY KEY, table_name TEXT NOT NULL);"
												"CREATE TABLE primitive_list (id INTEGER PRIMARY KEY, table_name TEXT NOT NULL);"
												"CREATE TABLE constraint_equation_list (id INTEGER PRIMARY KEY, table_name TEXT NOT NULL);"
												"CREATE TABLE undo_redo_list (id INTEGER PRIMARY KEY, undo TEXT, redo TEXT);"
												"CREATE TABLE undo_stable_points (stable_id INTEGER);"
											  "COMMIT;";

const std::string ark3d_current_database_file = "ark3d_working_db.current";
const std::string ark3d_previous_database_file = "ark3d_working_db.previous";


// construct empty model
Ark3DModel::Ark3DModel():
current_selection_mask_(All),
database_(0),
current_file_name_("")
{
	// initialize an empty database
	InitializeDatabase();
}

// construct existing model from file
Ark3DModel::Ark3DModel(const std::string &file_name):
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
		
			mmcMatrix computed_free_values = my_constraint_solver.MinimizeMeritFunction(initial_free_values, 1000, 1e-10, 1e-15, 100, 1, &std::cout);
		
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
void Ark3DModel::DeleteFlagged()
{
	map<unsigned,PrimitiveBasePointer>::iterator iter1 = primitive_list_.begin();

	while(iter1 != primitive_list_.end())
	{
		if(iter1->second->IsFlaggedForDeletion())
		{
			iter1->second->Erase();
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
			iter2->second->RemoveFromDatabase();
			constraint_equation_list_.erase(iter2++);
		} else {
			iter2++;
		}
	}

	// there may now be some DOF's that are not need, go ahead and delete them
	DeleteUnusedDOFs();
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
		// dof exists, return it
		return(dof_it->second);

	} else {
		// dof object does not exist, need to create it from the database
		return DOFFactory(id);
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

PrimitiveBasePointer Ark3DModel::PrimitiveFactory(unsigned id)
{
	// grab the table name from the database so we now exactly which class needs to be created
	int rc;
	sqlite3_stmt *statement;
	stringstream table_name_stream;
	string table_name;

	stringstream sql_command;
	sql_command << "SELECT * FROM primitive_list WHERE id=" << id << ";";

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
		error_description << "SQLite rowid " << id << " in table " << table_name << " does not exist";
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
	PrimitiveBasePointer result;

	if(table_name == "arc2d_list")
	{
		result.reset(new Arc2D(id,*this));
	}
	else if(table_name == "line2d_list"){
		result.reset(new Line2D(id,*this));
	}
	else if(table_name == "point_list"){
		result.reset(new Point(id,*this));
	}
	else if(table_name == "point2d_list"){
		result.reset(new Point2D(id,*this));
	}
	else if(table_name == "sketch_plane_list"){
		result.reset(new SketchPlane(id,*this));
	}
	else if(table_name == "vector_list"){
		result.reset(new Vector(id,*this));
	}
	else {
		throw Ark3DException("Ark3D::PrimitiveFactory: Unable to determine type based on database table name " + table_name);	
	}

	return result;
}

ConstraintEquationBasePointer Ark3DModel::ConstraintFactory(unsigned id)
{

	// grab the table name from the database so we now exactly which class needs to be created
	int rc;
	sqlite3_stmt *statement;
	stringstream table_name_stream;
	string table_name;

	stringstream sql_command;
	sql_command << "SELECT * FROM constraint_equation_list WHERE id=" << id << ";";

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
		error_description << "SQLite rowid " << id << " in table " << table_name << " does not exist";
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
	ConstraintEquationBasePointer result;

	if(table_name == "angle_line2d_list"){
		result.reset(new AngleLine2D(id,*this));
	}
	else if(table_name == "distance_point2d_list"){
		result.reset(new DistancePoint2D(id,*this));
	}
	else if(table_name == "parallel_line2d_list"){
		result.reset(new ParallelLine2D(id,*this));
	}
	else if(table_name == "tangent_edge2d_list"){
		result.reset(new TangentEdge2D(id,*this));
	}
	else {
		throw Ark3DException("Ark3D::ConstraintFactory: Unable to determine type based on database table name " + table_name);
	}

	return result;
}

// delete all unneeded DOF's in the dof_list_ container
void Ark3DModel::DeleteUnusedDOFs()
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
	DeleteFlagged();
}

