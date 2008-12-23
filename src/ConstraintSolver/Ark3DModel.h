#ifndef Ark3DModelH
#define Ark3DModelH

#include <string>

#include "Primitives.h"
#include "ConstraintSolver.h"

class Ark3DModel
{
public:
	// Acessor Methods
	Ark3DModel() {current_selection_mask_ = All;}
	~Ark3DModel() {dof_list_.clear(); constraint_equation_list_.clear(); primitive_list_.clear();}
	
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

	std::vector<DOFPointer> dof_list_;
	std::vector<ConstraintEquationBasePointer> constraint_equation_list_;
	std::vector<PrimitiveBasePointer> primitive_list_;

	SelectionMask current_selection_mask_;
};


#endif //Ark3DModelH
