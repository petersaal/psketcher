#include "Ark3DModel.h"

void Ark3DModel::AddConstraintEquation(const ConstraintEquationBasePointer &new_constraint_equation)
{
	// Add constraint equation to constraint equation vector container
	constraint_equation_list_.push_back(new_constraint_equation);
	
	// delete duplicate constraint equations

	// Add DOF's to DOF vector containter

	// delete duplication DOF's
	
}

void Ark3DModel::AddConstraintEquations(const std::vector<ConstraintEquationBasePointer> &new_constraint_equations)
{
	
}


void Ark3DModel::AddPrimitive(const PrimitiveBasePointer &new_primitive)
{

}

void Ark3DModel::AddPrimitives(const std::vector<PrimitiveBasePointer> &new_primitives)
{
	
}

