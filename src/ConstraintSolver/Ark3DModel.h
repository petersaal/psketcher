#ifndef Ark3DModelH
#define Ark3DModelH

#include "Primitives.h"
#include "ConstraintSolver.h"

class Ark3DModel
{
public:
	// Acessor Methods
	~Ark3DModel() {dof_list_.clear(); constraint_equation_list_.clear(); primitive_list_.clear();}
	
	// Constraint equation management
	void AddConstraintEquation(const ConstraintEquationBasePointer &new_constraint_equation);
	//void AddConstraintEquations(const std::vector<ConstraintEquationBasePointer> &new_constraint_equations);
	//void DeleteConstraintEquation(ConstraintEquationBasePointer constraint_to_delete);
	
	// Primitive management
	void AddPrimitive(const PrimitiveBasePointer &new_primitive);
	//void AddPrimitives(const std::vector<PrimitiveBasePointer> &new_primitives);
	//void DeletePrimitive(PrimitiveBasePointer primitive_to_delete);

	void SolveConstraints();

	void UpdateDisplay();

	// apply a selection mask to all primitives and constraint equations
	void ApplySelectionMask(SelectionMask mask);
	void ClearSelected() {;}

	// return vector of selected primitives
	std::vector<PrimitiveBasePointer> GetSelectedPrimitives();

	// return vector of selected constraint equations
	std::vector<ConstraintEquationBasePointer> GetConstraintEquations();

private:
	std::vector<DOFPointer> dof_list_;
	std::vector<ConstraintEquationBasePointer> constraint_equation_list_;
	std::vector<PrimitiveBasePointer> primitive_list_;
};


#endif //Ark3DModelH
