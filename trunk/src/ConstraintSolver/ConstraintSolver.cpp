/****************************************************************************
**
** This file is part of the pSketcher project.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Copyright (C) 2006-2008 Michael Greminger. All rights reserved.
**
****************************************************************************/

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

	ex new_error;

	exmap map;

	// substitute x into the error function
	for(unsigned int current_par = 0; current_par < free_parameters.size(); current_par++)
	{
		map[free_parameters[current_par]] = x(current_par,0);
	}

	new_error = error_function.subs(map, subs_options::no_pattern).evalf();

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

	exmap map;

	// substitute x into the error function
	for(unsigned int current_par = 0; current_par < free_parameters.size(); current_par++)
	{
		map[free_parameters[current_par]] = x(current_par,0);
	}	

	// substitute the current x into the gradient expression
	for(unsigned int current_ex = 0; current_ex < grad_expressions.size(); current_ex++)
	{
		new_grad.push_back(grad_expressions[current_ex].subs(map, subs_options::no_pattern).evalf());

		// make sure the expression is a numeric value
		if (is_a<numeric>(new_grad[current_ex])) {
			gradient(current_ex,0) = ex_to<numeric>(new_grad[current_ex]).to_double();
		} else {
			throw MeritFunctionException();
		}
	}
	

	return gradient;
}

void ConstraintSolver::GetMeritValuePlusGradient(const mmcMatrix & x, double &value, mmcMatrix &gradient)
{
	exmap map;

	// substitute x into the error function
	for(unsigned int current_par = 0; current_par < free_parameters.size(); current_par++)
	{
		map[free_parameters[current_par]] = x(current_par,0);
	}	

	// first calculate merit function value
	ex new_error = error_function.subs(map, subs_options::no_pattern).evalf();

	// check to make sure the expression is a numeric value
	if (is_a<numeric>(new_error)) {
		value = ex_to<numeric>(new_error).to_double();
	} else {
		throw MeritFunctionException();
 	}

	// now calculate merit function gradient
	vector<ex> new_grad;
	gradient.SetSize(GetNumDims(),1);

	for(unsigned int current_ex = 0; current_ex < grad_expressions.size(); current_ex++)
	{
		new_grad.push_back(grad_expressions[current_ex].subs(map, subs_options::no_pattern).evalf());

		// make sure the expression is a numeric value
		if (is_a<numeric>(new_grad[current_ex])) {
			gradient(current_ex,0) = ex_to<numeric>(new_grad[current_ex]).to_double();
		} else {
			throw MeritFunctionException();
		}
	}
}

