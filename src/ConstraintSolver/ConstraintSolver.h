#ifndef ConstraintSolverH
#define ConstraintSolverH

#include <vector>
#include "../mmcMatrixClass/mmcMatrix.h"
#include "../NumOptimization/bfgs.h"
#include <ginac/ginac.h>

using namespace GiNaC;

/* Now will define the merit function derived class used in the template matching */
class ConstraintSolver : public MeritFunction
{
public:
	ConstraintSolver(vector<ex> constraints_, vector<double> weights_, vector<symbol> free_parameters_,
                     vector<symbol> fixed_parameters_, vector<double> fixed_values_);
	virtual ~ConstraintSolver() {;}

	virtual double GetMeritValue(const mmcMatrix & x);
	virtual mmcMatrix GetMeritGradient(const mmcMatrix & x);

private:
	vector<symbol> free_parameters;
	vector<symbol> fixed_parameters;
	vector<double> fixed_values;
	vector<double> weights;
	vector<ex> constraints;
	vector<ex> grad_expressions;

	ex error_function;
};

#endif //ConstraintSolverH

