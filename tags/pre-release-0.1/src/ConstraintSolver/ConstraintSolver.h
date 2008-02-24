#ifndef ConstraintSolverH
#define ConstraintSolverH

#include <vector>
#include "../mmcMatrix/mmcMatrix.h"
#include "../NumOptimization/bfgs.h"
#include <ginac/ginac.h>

/* Now will define the merit function derived class used in the template matching */
class ConstraintSolver : public MeritFunction
{
public:
	ConstraintSolver(const std::vector<GiNaC::ex> &constraints_, const std::vector<double> & weights_, const std::vector<GiNaC::symbol> & free_parameters_, const std::vector<GiNaC::symbol> & fixed_parameters_, const std::vector<double> & fixed_values_);
	virtual ~ConstraintSolver() {;}

	virtual double GetMeritValue(const mmcMatrix & x);
	virtual mmcMatrix GetMeritGradient(const mmcMatrix & x);

private:
	std::vector<GiNaC::symbol> free_parameters;
	std::vector<GiNaC::symbol> fixed_parameters;
	std::vector<double> fixed_values;
	std::vector<double> weights;
	std::vector<GiNaC::ex> constraints;
	std::vector<GiNaC::ex> grad_expressions;

	GiNaC::ex error_function;
};

#endif //ConstraintSolverH

