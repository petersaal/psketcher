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

// utility function used by this class for setting up the database and performing file save operations
bool FileExists(std::string file_name);

class Ark3DModel
{
public:
	// Acessor Methods
	Ark3DModel();
	~Ark3DModel();
	
	// methods used to manage the sqlite3 database, this database is used to implement saving to file and undo/redo functionality
	void InitializeDatabase();

	// Constraint equation management
	void AddConstraintEquation(const ConstraintEquationBasePointer &new_constraint_equation);
	
	// Primitive management
	void AddPrimitive(const PrimitiveBasePointer &new_primitive);
	
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
	bool Save(std::string file_name);

private:
	void FlagDependentsForDeletion(PrimitiveBasePointer primitive_to_delete); // Flag any primitives or constraint equations for deletion that depend on this primitive
	void DeleteFlagged(); // delete all of the primitives that have been flagged for deletion
	virtual void PreparePrimitiveForDeletion(PrimitiveBasePointer primitive_to_delete) {;}

	std::map<unsigned,DOFPointer> dof_list_;
	std::map<unsigned,ConstraintEquationBasePointer> constraint_equation_list_;
	std::map<unsigned,PrimitiveBasePointer> primitive_list_;

	SelectionMask current_selection_mask_;

	// SQLite3 database that will be used to implement file save and undo/redo
	sqlite3 *database_;
};


#endif //Ark3DModelH
