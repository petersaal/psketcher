#include <iostream>
#include "Ark3DModel.h"

Ark3DModel::Ark3DModel():
current_selection_mask_(All),
database_(0)
{
	InitializeDatabase();
}

void Ark3DModel::InitializeDatabase()
{
	int rc = sqlite3_open("test_model", &database_);
	if( rc ){
		// an error occurred when trying to open the database
		//fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(database_);
		throw Ark3DException();
	}
}

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
	
		// For each dependent DOF, substitute its defining expression into all of the constraint equations that reference it
		for(unsigned int current_dof = 0; current_dof < dof_list_.size(); current_dof++)
		{
			if(dof_list_[current_dof]->IsDependent())
			{
				// loop over each constraint equation and substitute the dependent DOF
				for(unsigned int current_equation = 0; current_equation < constraints.size(); current_equation++)
				{
					constraints[current_equation] = constraints[current_equation].subs(dof_list_[current_dof]->GetVariable() == dof_list_[current_dof]->GetExpression(),GiNaC::subs_options::no_pattern);
				}
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
			{	// free parameter
				free_parameters.push_back(dof_list_[current_dof]->GetVariable());
				free_dof_list.push_back(dof_list_[current_dof]);
				free_values.push_back(dof_list_[current_dof]->GetValue());
			} else if( ! dof_list_[current_dof]->IsDependent()) 
			{	// fixed, independent parameter
				fixed_parameters.push_back(dof_list_[current_dof]->GetVariable());
				fixed_values.push_back(dof_list_[current_dof]->GetValue());
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
	for(unsigned int current_equation = 0; current_equation < constraint_equation_list_.size(); current_equation++)
		constraint_equation_list_[current_equation]->UpdateDisplay();

	// Update display for all of the primitives
	for(unsigned int current_primitive = 0; current_primitive < primitive_list_.size(); current_primitive++)
		primitive_list_[current_primitive]->UpdateDisplay();
}

void Ark3DModel::ApplySelectionMask(SelectionMask mask)
{
	current_selection_mask_ = mask;

	// Apply mask to all of the constraint equations
	for(unsigned int current_equation = 0; current_equation < constraint_equation_list_.size(); current_equation++)
		constraint_equation_list_[current_equation]->ApplySelectionMask(mask);

	// Apply mask to  all of the primitives
	for(unsigned int current_primitive = 0; current_primitive < primitive_list_.size(); current_primitive++)
		primitive_list_[current_primitive]->ApplySelectionMask(mask);

}

std::vector<PrimitiveBasePointer> Ark3DModel::GetSelectedPrimitives()
{
	std::vector<PrimitiveBasePointer> selected_primitives;

	for(unsigned int current_primitive = 0; current_primitive < primitive_list_.size(); current_primitive++)
	{
		if(primitive_list_[current_primitive]->IsSelected())
			selected_primitives.push_back(primitive_list_[current_primitive]);
	}

	return selected_primitives;
}

std::vector<ConstraintEquationBasePointer> Ark3DModel::GetConstraintEquations()
{
	std::vector<ConstraintEquationBasePointer> selected_constraint_equations;

	for(unsigned int current_equation = 0; current_equation < constraint_equation_list_.size(); current_equation++)
	{
		if(constraint_equation_list_[current_equation]->IsSelected())
			selected_constraint_equations.push_back(constraint_equation_list_[current_equation]);
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
	for(unsigned int current_primitive = 0; current_primitive < primitive_list_.size(); current_primitive++)
	{
		status_changed = primitive_list_[current_primitive]->FlagForDeletionIfDependent(primitive_to_delete);
		if(status_changed)
			// recurse if this primitive is now flagged for deletion
			FlagDependentsForDeletion(primitive_list_[current_primitive]);
	}

	// loop through all of the constraints
	for(unsigned int current_equation = 0; current_equation < constraint_equation_list_.size(); current_equation++)
	{
		status_changed = constraint_equation_list_[current_equation]->FlagForDeletionIfDependent(primitive_to_delete);
		if(status_changed)
			// recurse if this constraint equation is now flagged for deletion
			FlagDependentsForDeletion(constraint_equation_list_[current_equation]);
	}
}

// delete all of the primitives that have been flagged for deletion
void Ark3DModel::DeleteFlagged()
{
	std::vector<PrimitiveBasePointer>::iterator iter1 = primitive_list_.begin();

	while(iter1 != primitive_list_.end())
	{
		if((*iter1)->IsFlaggedForDeletion())
		{
			PreparePrimitiveForDeletion(*iter1);
			iter1 = primitive_list_.erase(iter1);
		} else {
			iter1++;
		}
	}
	
	std::vector<ConstraintEquationBasePointer>::iterator iter2 = constraint_equation_list_.begin();

	while(iter2 != constraint_equation_list_.end())
	{
		if((*iter2)->IsFlaggedForDeletion())
		{
			PreparePrimitiveForDeletion(*iter2);
			iter2 = constraint_equation_list_.erase(iter2);
		} else {
			iter2++;
		}
	}
}

void Ark3DModel::DeleteSelected()
{
	// loop through all of the primitives
	for(unsigned int current_primitive = 0; current_primitive < primitive_list_.size(); current_primitive++)
	{
		if(primitive_list_[current_primitive]->IsSelected())
		{
			primitive_list_[current_primitive]->FlagForDeletion();
			FlagDependentsForDeletion(primitive_list_[current_primitive]);
		}
	}

	// loop through all of the constraints
	for(unsigned int current_equation = 0; current_equation < constraint_equation_list_.size(); current_equation++)
	{
		if(constraint_equation_list_[current_equation]->IsSelected())
		{
			constraint_equation_list_[current_equation]->FlagForDeletion();
			FlagDependentsForDeletion(constraint_equation_list_[current_equation]);
		}
	}

	DeleteFlagged();
}


