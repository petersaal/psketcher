#include "DependentDOF.h"

#include "PrimitiveBase.h"

using namespace std;
using namespace GiNaC;


DependentDOF :: DependentDOF (ex expression, std::vector<DOFPointer> source_dof_list):
DOF(false /*free*/,true /*dependent*/)
{
	// @fixme Need to make sure that all DOF's in expression are included in the DOF list
	expression_ = expression;
	source_dof_list_ = source_dof_list;
}

DependentDOF :: DependentDOF ( const char *name, ex expression, std::vector<DOFPointer> source_dof_list):
DOF(name,false /*free*/,true /*dependent*/)
{
	// @fixme Need to make sure that all DOF's in expression are included in the DOF list
	expression_ = expression;
	source_dof_list_ = source_dof_list;
}

double DependentDOF::GetValue()const
{
	// For each independent DOF in the list, substitute its value into the expression that defines this dependent DOF
	GiNaC::ex current_expression = GetExpression();
	double result;

	for(unsigned int current_dof = 0; current_dof < source_dof_list_.size(); current_dof++)
	{
		if(!source_dof_list_[current_dof]->IsDependent())
		{
			current_expression = current_expression.subs(source_dof_list_[current_dof]->GetVariable() ==
														 source_dof_list_[current_dof]->GetValue(),subs_options::no_pattern);
		}
	}

	// Now evaluate the expression to a numeric value
	// check to make sure the expression evaluates to a numeric value
	if (is_a<numeric>(current_expression)) {
		result = ex_to<numeric>(current_expression).to_double();
	} else {
		throw Ark3DException();
  	}

	return result;
}

GiNaC::ex DependentDOF::GetExpression()const
{
	// For each dependent DOF in the list, substitute its expression into the overall expression that defines this dependent DOF
	GiNaC::ex result = expression_;

	for(unsigned int current_dof = 0; current_dof < source_dof_list_.size(); current_dof++)
	{
		if(source_dof_list_[current_dof]->IsDependent())
		{
			result = result.subs(source_dof_list_[current_dof]->GetVariable() == source_dof_list_[current_dof]->GetExpression(),subs_options::no_pattern);
		}
	}

	return result;
}

// method for adding this object to the SQLite3 database
void DependentDOF::AddToDatabase(sqlite3 *database)
{
	
}
