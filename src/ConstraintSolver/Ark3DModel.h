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

#ifndef Ark3DModelH
#define Ark3DModelH

#include <string>
#include <map>

#include <sqlite3.h>

#include "Primitives.h"
#include "ConstraintSolver.h"

class Ark3DModel
{
public:
	// Constructors
	Ark3DModel(PrimitiveBasePointer (*current_primitive_factory)(unsigned, Ark3DModel &) = Ark3DModel::PrimitiveFactory, ConstraintEquationBasePointer (*current_constraint_factory)(unsigned, Ark3DModel &) = Ark3DModel::ConstraintFactory);

	Ark3DModel(const std::string &file_name, PrimitiveBasePointer (*current_primitive_factory)(unsigned, Ark3DModel &) = Ark3DModel::PrimitiveFactory, ConstraintEquationBasePointer (*current_constraint_factory)(unsigned, Ark3DModel &) = Ark3DModel::ConstraintFactory);  // construct from file

	~Ark3DModel();
	
	// methods used to manage the sqlite3 database, this database is used to implement saving to file and undo/redo functionality
	void InitializeDatabase();

	// Constraint equation management
	virtual void AddConstraintEquation(const ConstraintEquationBasePointer &new_constraint_equation, bool update_database = true);
	
	// Primitive management
	virtual void AddPrimitive(const PrimitiveBasePointer &new_primitive, bool update_database = true);
	
	// method for deleting primitives (either a primitive or a constraint equation)
	void DeletePrimitive(PrimitiveBasePointer primitive_to_delete);
	void DeleteSelected();

	void SolveConstraints();

	void UpdateDisplay();

	// apply a selection mask to all primitives and constraint equations
	void ApplySelectionMask(SelectionMask mask);
	void ClearSelected() {;}

	// return vector of selected primitives
	std::vector<PrimitiveBasePointer> GetSelectedPrimitives();

	// return vector of selected constraint equations
	std::vector<ConstraintEquationBasePointer> GetConstraintEquations();

	// Methods relating to saving and loading the model from a file
	// An SQLite3 database is used to store the model 
	void SyncToDatabase();  // synchronize the primitive, constraint, and DOF lists to the database (used to implement file open and undo/redo)
	void SetMaxIDNumbers(); // set the max id numbers for the PrimitiveBase and DOF classes via their static method setNextID(...)
	bool Save(const std::string &file_name = "", bool save_copy = false); // returns true on success
	const std::string & GetFileName() {return current_file_name_;}

	// these methods fetch objects from the map containers or create them using the factory methods below if they do not exist in their respective map
	DOFPointer FetchDOF(unsigned id);
	template <class data_t> boost::shared_ptr<data_t> FetchPrimitive(unsigned id);
	template <class data_t> boost::shared_ptr<data_t> FetchConstraint(unsigned id);
	
	sqlite3 *GetDatabase() {return database_;}

	// methods to implement undo/redo functionality
	bool Undo();
	bool Redo();
	bool IsUndoAvailable(std::string &description);
	bool IsUndoAvailable(int &current_stable_point, int &new_stable_point, int &current_row_id /* current row id of table undo_stable_points */, std::string &description);
	bool IsRedoAvailable(std::string &description);
	bool IsRedoAvailable(int &current_stable_point, int &new_stable_point, int &current_row_id /* current row id of table undo_stable_points */, std::string &description);
	void MarkStablePoint(const std::string &description);

	// methods for importing and exporting geometry
	bool ExportDXF(const std::string &file_name);

protected:
	// methods for generating objects directly from the database
	DOFPointer DOFFactory(unsigned id);
	static PrimitiveBasePointer PrimitiveFactory(unsigned id, Ark3DModel &ark3d_model);
	static ConstraintEquationBasePointer ConstraintFactory(unsigned id, Ark3DModel &ark3d_model);

	std::map<unsigned,DOFPointer> dof_list_;
	std::map<unsigned,ConstraintEquationBasePointer> constraint_equation_list_;
	std::map<unsigned,PrimitiveBasePointer> primitive_list_;

private:
	// function pointers so that child classes can implement their own factory methods for use within the constructor of Ark3DModel (virtual methods won't work in the constructor) 
	PrimitiveBasePointer (*CurrentPrimitiveFactory)(unsigned, Ark3DModel &);
	ConstraintEquationBasePointer (*CurrentConstraintFactory)(unsigned, Ark3DModel &);

	void FlagDependentsForDeletion(PrimitiveBasePointer primitive_to_delete); // Flag any primitives or constraint equations for deletion that depend on this primitive
	void DeleteFlagged(bool remove_from_db = true); // delete all of the primitives that have been flagged for deletion
	void DeleteUnusedDOFs(bool remove_from_db = true); // delete all unused DOF's in the dof_list_ container

	SelectionMask current_selection_mask_;

	// SQLite3 database that will be used to implement file save and undo/redo
	sqlite3 *database_;

	// current file name
	std::string current_file_name_;
};


// Must define the template member functions in the header file since it won't work to define them in Ark3DModel.cpp
template <class data_t> boost::shared_ptr<data_t> Ark3DModel::FetchPrimitive(unsigned id)
{
	PrimitiveBasePointer temp;
	boost::shared_ptr<data_t> result;

	std::map<unsigned,PrimitiveBasePointer>::iterator primitive_it = primitive_list_.find(id);
	if(primitive_it != primitive_list_.end())
	{
		// primitive already exists
		temp = primitive_it->second;

	} else {
		// primitive object does not exist, need to create it from the database
		temp = CurrentPrimitiveFactory(id,*this);
		AddPrimitive(temp, false);  // don't update DB since primitive already exists in DB
	}

	// attempt to cast the PrimitiveBasePointer to the desired datatype, generate an error if the cast fails
	if(dynamic_cast<data_t *>(temp.get()) != 0){
		result = boost::dynamic_pointer_cast<data_t>(temp);
	} else {
		throw Ark3DException("Requested data type does not match database data type.");
	}
	
	return result;
}

template <class data_t> boost::shared_ptr<data_t> Ark3DModel::FetchConstraint(unsigned id)
{
	ConstraintEquationBasePointer temp;
	boost::shared_ptr<data_t> result;

	std::map<unsigned,ConstraintEquationBasePointer>::iterator constraint_it = constraint_equation_list_.find(id);
	if(constraint_it != constraint_equation_list_.end())
	{
		// primitive already exists
		temp = constraint_it->second;

	} else {
		// primitive object does not exist, need to create it from the database
		temp = CurrentConstraintFactory(id,*this);
		AddConstraintEquation(temp, false);  // don't add to DB since the constraint equation already exists in the DB
	}

	// attempt to cast the PrimitiveBasePointer to the desired datatype, generate an error if the cast fails
	if(dynamic_cast<data_t *>(temp.get()) != 0){
		result = boost::dynamic_pointer_cast<data_t>(temp);
	} else {
		throw Ark3DException("Requested data type does not match database data type.");
	}
	
	return result;
}


#endif //Ark3DModelH
