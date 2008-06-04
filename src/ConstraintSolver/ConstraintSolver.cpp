#include <math.h>
#include "ConstraintSolver.h"

using namespace std;
using namespace GiNaC;

// Constructor
ConstraintSolver::ConstraintSolver(const std::vector<GiNaC::ex> &constraints_, const std::vector<double> & weights_, const std::vector<GiNaC::symbol> & free_parameters_, const std::vector<GiNaC::symbol> & fixed_parameters_, const std::vector<double> & fixed_values_):
MeritFunction(free_parameters_.size())
{
	if(constraints_.size() < 1)
		throw MeritFunctionException();
	
	if(constraints_.size() != weights_.size())
		throw MeritFunctionException();
		
	if(free_parameters_.size() < 1)
		throw MeritFunctionException();
		
	if(fixed_parameters_.size() != fixed_values_.size())
		throw MeritFunctionException(); 
	
	free_parameters = free_parameters_;
	fixed_parameters = fixed_parameters_;
	fixed_values = fixed_values_;
	weights = weights_;
	constraints = constraints_;
	
	// create error function
	error_function = 0;
	for(unsigned int current_ex = 0; current_ex < constraints.size(); current_ex++)
	{
		error_function += weights[current_ex] * constraints[current_ex] * constraints[current_ex];
	}

	// substitute fixed parameters
	for(unsigned int current_par = 0; current_par < fixed_parameters.size(); current_par++)
	{
		error_function = error_function.subs(fixed_parameters[current_par] == fixed_values[current_par],subs_options::no_pattern);
	}

	for(unsigned int current_par = 0; current_par < free_parameters.size(); current_par++)
	{
		grad_expressions.push_back(error_function.diff(free_parameters[current_par],1));
	}
	
}

double ConstraintSolver::GetMeritValue(const mmcMatrix & x)
{
	double result = 0;

	ex new_error = error_function;

	// substitute x into the error function
	for(unsigned int current_par = 0; current_par < free_parameters.size(); current_par++)
	{
		new_error = new_error.subs(free_parameters[current_par] == x(current_par,0), subs_options::no_pattern);
	}

	// evaluate to a numeric
	new_error = evalf(new_error);

	// check to make sure the expression is a numeric value
	if (is_a<numeric>(new_error)) {
		result = ex_to<numeric>(new_error).to_double();
	} else {
		throw MeritFunctionException();
  }

	return result;
}

mmcMatrix ConstraintSolver::GetMeritGradient(const mmcMatrix & x)
{
	mmcMatrix gradient(GetNumDims(),1,0.0);

	vector<ex> new_grad;

	// substitute the current x into the gradient expression
	for(unsigned int current_ex = 0; current_ex < grad_expressions.size(); current_ex++)
	{
		new_grad.push_back(grad_expressions[current_ex]);
		for(unsigned int current_par = 0; current_par < free_parameters.size(); current_par++)
		{
			new_grad[current_ex] = new_grad[current_ex].subs(free_parameters[current_par] == x(current_par,0), subs_options::no_pattern);
		}
	}
	
	// check to make sure the grad expressions evaluate to a numeric value
	for(unsigned int current_ex = 0; current_ex < new_grad.size(); current_ex++)
	{
		// evaluate to a numeric
		new_grad[current_ex] = evalf(new_grad[current_ex]);
		
		// make sure the expression is a numeric value
		if (is_a<numeric>(new_grad[current_ex])) {
			gradient(current_ex,0) = ex_to<numeric>(new_grad[current_ex]).to_double();
		} else {
			throw MeritFunctionException();
		}
	}

	return gradient;
}

