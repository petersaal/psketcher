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
#include <sys/stat.h>
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

// utility function used by this class 
bool FileExists(std::string file_name) {
  struct stat file_info;
  bool result;
  int file_stat;

  // Attempt to get the file attributes
  file_stat = stat(file_name.c_str(),&file_info);
  if(file_stat == 0) {
    result = true;
  } else {
	// was unable to get that file info, file may not exist or there may be some other problem
    result = false;
  }

  return(result);
}



Ark3DModel::Ark3DModel():
current_selection_mask_(All),
database_(0)
{
	InitializeDatabase();
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

	// clear out the lists
	dof_list_.clear(); 
	constraint_equation_list_.clear(); 
	primitive_list_.clear();

}

void Ark3DModel::InitializeDatabase()
{
	// @fixme The following code for deleting and renaming files for the working database needs to be made more robust. Some cases will blow up. For example, if the the user does not have permision to move or delete the files or if there is a directory with the same name as one of the default database files.

	// delete the previous database file if it already exists
	if(FileExists(ark3d_previous_database_file))
		remove(ark3d_previous_database_file.c_str());

	// move the current database file to the previous database file if it exists
	if(FileExists(ark3d_current_database_file))
		rename(ark3d_current_database_file.c_str(),ark3d_previous_database_file.c_str());

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

void Ark3DModel::AddConstraintEquation(const ConstraintEquationBasePointer &new_constraint_equation)
{
	// Add constraint equation to constraint equation vector container
	pair<map<unsigned,ConstraintEquationBasePointer>::iterator,bool> constraint_ret;
	constraint_ret = constraint_equation_list_.insert(pair<unsigned,ConstraintEquationBasePointer>(new_constraint_equation->GetID(),new_constraint_equation));
	if(constraint_ret.second) // constraint_ret.second is true if this constraint is not already in the map
		new_constraint_equation->AddToDatabase(database_);		

	// Add the primitives that this constraint depends on to the primitive map container
	vector<PrimitiveBasePointer>::const_iterator primitive_it;
	vector<PrimitiveBasePointer>::const_iterator primitive_end = new_constraint_equation->GetPrimitiveList().end();
	for ( primitive_it=new_constraint_equation->GetPrimitiveList().begin() ; primitive_it != primitive_end; primitive_it++ )
	{
		pair<map<unsigned,PrimitiveBasePointer>::iterator,bool> primitive_ret = primitive_list_.insert(pair<unsigned,PrimitiveBasePointer>((*primitive_it)->GetID(),(*primitive_it)));
		if(primitive_ret.second) // ret.second is true if this PrimitivePointer is not already in the map
			(*primitive_it)->AddToDatabase(database_); // this primitive is new to this model and needs to be added to the database
	}

	// Add DOF's to DOF map containter
	vector<DOFPointer>::const_iterator dof_it;
	vector<DOFPointer>::const_iterator dof_end = new_constraint_equation->GetDOFList().end();
	pair<map<unsigned,DOFPointer>::iterator,bool> ret;
	for ( dof_it=new_constraint_equation->GetDOFList().begin() ; dof_it != dof_end; dof_it++ )
	{
		ret = dof_list_.insert(pair<unsigned,DOFPointer>((*dof_it)->GetID(),(*dof_it)));
		if(ret.second) // ret.second is true if this DOFPointer is not already in the map
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

void Ark3DModel::AddPrimitive(const PrimitiveBasePointer &new_primitive)
{
	// Add primitive to the primitive vector container
	pair<map<unsigned,PrimitiveBasePointer>::iterator,bool> primitive_ret;
	primitive_ret = primitive_list_.insert(pair<unsigned,PrimitiveBasePointer>(new_primitive->GetID(),new_primitive));
	if(primitive_ret.second) // primitive_ret.second is true if this primitive is not already in the map
		new_primitive->AddToDatabase(database_);

	// Add the primitives that this primitive depends on to the primitive map container
	vector<PrimitiveBasePointer>::const_iterator primitive_it;
	vector<PrimitiveBasePointer>::const_iterator primitive_end = new_primitive->GetPrimitiveList().end();
	for ( primitive_it=new_primitive->GetPrimitiveList().begin() ; primitive_it != primitive_end; primitive_it++ )
	{
		primitive_ret = primitive_list_.insert(pair<unsigned,PrimitiveBasePointer>((*primitive_it)->GetID(),(*primitive_it)));
		if(primitive_ret.second) // ret.second is true if this PrimitivePointer is not already in the map
			(*primitive_it)->AddToDatabase(database_); // this primitive is new to this model and needs to be added to the database
	}

	// Add DOF's that this primitive depends on to DOF map containter
	vector<DOFPointer>::const_iterator dof_it;
	vector<DOFPointer>::const_iterator dof_end = new_primitive->GetDOFList().end();
	pair<map<unsigned,DOFPointer>::iterator,bool> ret;
	for ( dof_it=new_primitive->GetDOFList().begin() ; dof_it != dof_end; dof_it++ )
	{
		ret = dof_list_.insert(pair<unsigned,DOFPointer>((*dof_it)->GetID(),(*dof_it)));
		if(ret.second) // ret.second is true if this DOFPointer is not already in the map
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
			PreparePrimitiveForDeletion(iter1->second);
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
			PreparePrimitiveForDeletion(iter2->second);
			iter2->second->RemoveFromDatabase();
			constraint_equation_list_.erase(iter2++);
		} else {
			iter2++;
		}
	}
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

DOFPointer Ark3DModel::FetchDOF(unsigned id, const string &table_name)
{
	map<unsigned,DOFPointer>::iterator dof_it = dof_list_.find(id);
	if(dof_it != dof_list_.end())
	{
		// dof exists, return it
		return(dof_it->second);

	} else {
		// dof object does not exist, need to create it from the database
		return DOFFactory(id,table_name);
	}
}

template <class data_t> boost::shared_ptr<data_t> Ark3DModel::FetchPrimitive(unsigned id, const string &table_name)
{
	PrimitiveBasePointer temp;
	boost::shared_ptr<data_t> result;

	map<unsigned,PrimitiveBasePointer>::iterator primitive_it = primitive_list_.find(id);
	if(primitive_it != primitive_list_.end())
	{
		// primitive already exists
		temp = primitive_it->second;

	} else {
		// primitive object does not exist, need to create it from the database
		temp = PrimitiveFactory(id,table_name);
		AddPrimitive(temp);
	}

	// attempt to cast the PrimitiveBasePointer to the desired datatype, generate an error if the cast fails
	if(dynamic_cast<data_t *>(temp.get()) != 0){
		result = boost::dynamic_pointer_cast<data_t>(temp);
	} else {
		throw Ark3DException("Requested data type does not match database data type.");
	}
	
	return result;
}


template <class data_t> boost::shared_ptr<data_t> Ark3DModel::FetchConstraint(unsigned id, const string &table_name)
{
	ConstraintEquationBasePointer temp;
	boost::shared_ptr<data_t> result;

	map<unsigned,ConstraintEquationBasePointer>::iterator constraint_it = constraint_equation_list_.find(id);
	if(constraint_it != constraint_equation_list_.end())
	{
		// primitive already exists
		temp = constraint_it->second;

	} else {
		// primitive object does not exist, need to create it from the database
		temp = ConstraintFactory(id,table_name);
		AddConstraintEquation(temp);
	}

	// attempt to cast the PrimitiveBasePointer to the desired datatype, generate an error if the cast fails
	if(dynamic_cast<data_t *>(temp.get()) != 0){
		result = boost::dynamic_pointer_cast<data_t>(temp);
	} else {
		throw Ark3DException("Requested data type does not match database data type.");
	}
	
	return result;
}

DOFPointer Ark3DModel::DOFFactory(unsigned id, const string &table_name)
{
		if(table_name == "independent_dof_list")
		{

		}
		else if(table_name == "dependent_dof_list"){
		
		}
		else {
			throw Ark3DException("Ark3D::DOFFactory: Unable to determine type based on database table name " + table_name);
		}
}

PrimitiveBasePointer Ark3DModel::PrimitiveFactory(unsigned id, const string &table_name)
{
		if(table_name == "arc2d_list")
		{

		}
		else if(table_name == "line2d_list"){

		}
		else if(table_name == "point_list"){

		}
		else if(table_name == "point2d_list"){

		}
		else if(table_name == "sketch_plane_list"){

		}
		else if(table_name == "vector_list"){

		}
		else {
			throw Ark3DException("Ark3D::PrimitiveFactory: Unable to determine type based on database table name " + table_name);	
		}
}

ConstraintEquationBasePointer Ark3DModel::ConstraintFactory(unsigned id, const string &table_name)
{
		if(table_name == "angle_line2d_list"){

		}
		else if(table_name == "distance_point2d_list"){

		}
		else if(table_name == "parallel_line2d_list"){

		}
		else if(table_name == "tangent_edge2d_list"){

		}
		else {
			throw Ark3DException("Ark3D::ConstraintFactory: Unable to determine type based on database table name " + table_name);
		}
}


