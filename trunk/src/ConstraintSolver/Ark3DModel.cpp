#include <iostream>
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
void Ark3DModel::SolveConstraints()
{
	unsigned int num_free_values;
	unsigned int num_fixed_values; 
	
	// create constraint equation and constraint equation weight vectors
	std::vector<GiNaC::ex> constraints;
	std::vector<double> weights;

	for(unsigned int current_equation = 0; current_equation < constraint_equation_list_.size(); current_equation++)
	{
			std::vector< boost::shared_ptr<GiNaC::ex> > current_constraint_list = constraint_equation_list_[current_equation]->GetConstraintList();
			
			std::vector<double> current_weight_list = constraint_equation_list_[current_equation]->GetWeightList();

			// loop over each expression in the constraint equation class
			for(unsigned int current_sub_equation = 0; current_sub_equation < current_constraint_list.size(); current_sub_equation++)
			{
				constraints.push_back(*(current_constraint_list[current_sub_equation]));
				weights.push_back(current_weight_list[current_sub_equation]);
			}
	}

	// create the free parameters, free_values, fixed_parameters, and fixed_values lists
	std::vector<GiNaC::symbol> free_parameters;
	std::vector<DOFPointer> free_dof_list;   // This vector will be used to update the DOF's after solving the constraint equations
	std::vector<double> free_values;
	std::vector<GiNaC::symbol> fixed_parameters;
	std::vector<double> fixed_values;
	
	for(unsigned int current_dof = 0; current_dof < dof_list_.size(); current_dof++)
	{
		if(dof_list_[current_dof]->IsFree())
		{
			free_parameters.push_back(dof_list_[current_dof]->GetVariable());
			free_dof_list.push_back(dof_list_[current_dof]);
			free_values.push_back(dof_list_[current_dof]->GetValue());
		} else {
			fixed_parameters.push_back(dof_list_[current_dof]->GetVariable());
			fixed_values.push_back(dof_list_[current_dof]->GetValue());
		}
	}

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


void Ark3DModel::UpdateDisplay()
{
	// Update display for all of the constraint equations
	for(unsigned int current_equation = 0; current_equation < constraint_equation_list_.size(); current_equation++)
		constraint_equation_list_[current_equation]->UpdateDisplay();

	// Update display for all of the primitives
	for(unsigned int current_primitive = 0; current_primitive < primitive_list_.size(); current_primitive++)
		primitive_list_[current_primitive]->UpdateDisplay();
}
