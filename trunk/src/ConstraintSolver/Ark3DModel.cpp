#include "Ark3DModel.h"

void Ark3DModel::AddConstraintEquation(const ConstraintEquationBasePointer &new_constraint_equation)
{
	// Add constraint equation to constraint equation vector container
	constraint_equation_list_.push_back(new_constraint_equation);
	
	// delete duplicate constraint equations
	// @todo warn user if the constraint equation added is a duplicate
  sort( constraint_equation_list_.begin(), constraint_equation_list_.end());
  constraint_equation_list_.erase( unique( constraint_equation_list_.begin(), constraint_equation_list_.end()), constraint_equation_list_.end());

	// Add DOF's to DOF vector containter
	dof_list_.insert(dof_list_.end(),new_constraint_equation->GetDOFList().begin(),new_constraint_equation->GetDOFList().end());

	// delete duplicate DOF's
  sort( dof_list_.begin(), dof_list_.end());
  dof_list_.erase( unique( dof_list_.begin(), dof_list_.end()), dof_list_.end());
	
}

void Ark3DModel::AddConstraintEquations(const std::vector<ConstraintEquationBasePointer> &new_constraint_equations)
{
	for(unsigned int current_equation = 0; current_equation < new_constraint_equations.size(); current_equation++)
		AddConstraintEquation(new_constraint_equations[current_equation]);
}

void Ark3DModel::AddPrimitive(const PrimitiveBasePointer &new_primitive)
{
	// Add primitive to the primitive vector container
	primitive_list_.push_back(new_primitive);

	// delete duplicate primitives
	// @todo warn user if the primitive added is a duplicate
  sort( primitive_list_.begin(), primitive_list_.end());
  primitive_list_.erase( unique( primitive_list_.begin(), primitive_list_.end()), primitive_list_.end());	


	// Add DOF's to DOF vector containter
	dof_list_.insert(dof_list_.end(),new_primitive->GetDOFList().begin(),new_primitive->GetDOFList().end());

	// delete duplicate DOF's
  sort( dof_list_.begin(), dof_list_.end());
  dof_list_.erase( unique( dof_list_.begin(), dof_list_.end()), dof_list_.end());
}

void Ark3DModel::AddPrimitives(const std::vector<PrimitiveBasePointer> &new_primitives)
{
	for(unsigned int current_primitive = 0; current_primitive < new_primitives.size(); current_primitive++)
		AddPrimitive(new_primitives[current_primitive]);
}

// This method solves the system of constraint equations for this model
void SolveConstraints()
{

}
