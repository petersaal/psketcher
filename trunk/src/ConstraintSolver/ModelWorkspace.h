#ifndef ModelWorkspaceH
#define ModelWorksapceH

#include <vector>
#include"Primitives.cpp"

class ModelWorksapce
{
public:

    // DOF Management
    DOF *GetNewDOF(double value = 0.0);
    DOF *GetNewDOF(const char *name,double value = 0.0);
    void DeleteDOF(DOF *OldDOF);

    // Constraint equation management
    AddConstraintEquation
    DeleteConstraintEquation

    // Primitive management
    AddPrimitive
    DeletePrimitive

private:
	vector<DOF> dof_list_;
	vector<ConstraintEquation> constraint_equation_list_;
	vector<Primitive> primitive_list_;

};

#endif //ModelWorksapceH
