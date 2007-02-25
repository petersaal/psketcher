#ifndef ModelWorkspaceH
#define ModelWorksapceH

#include "Primitives.h"

class ModelWorksapce
{
public:

	// DOF Management
	DOF *GetNewDOF(double value = 0.0);
	DOF *GetNewDOF(const char *name,double value = 0.0);
	void DeleteDOF(DOF *OldDOF);
	
	// Constraint equation management
	void AddConstraintEquation(ConstraintEquationBasePointer new_constraint_equation);
	void DeleteConstraintEquation(ConstraintEquationBasePointer constraint_to_delete);
	
	// Primitive management
	void AddPrimitive(PrimitiveBasePointer new_primitive);
	void DeletePrimitive(PrimitiveBasePointer primitive_to_delete);

private:
	std::vector<DOFPointer> dof_list_;
	std::vector<ConstraintEquationBasePointer> constraint_equation_list_;
	std::vector<PrimitiveBasePointer> primitive_list_;
};

#endif //ModelWorksapceH
