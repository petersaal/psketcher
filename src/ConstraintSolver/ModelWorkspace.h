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
    void AddConstraintEquation(ConstraintEquationPointer new_constraint_equation);
    void DeleteConstraintEquation(ConstraintEquationPointer constraint_to_delete);

    // Primitive management
    void AddPrimitiveBase(PrimitiveBasePointer new_primitive);
    void DeletePrimitive(PrimitiveBasePointer primitive_to_delete);

private:
	vector<DOFPointer> dof_list_;
	vector<ConstraintEquationPointer> constraint_equation_list_;
	vector<PrimitiveBasePointer> primitive_list_;

};

#endif //ModelWorksapceH
