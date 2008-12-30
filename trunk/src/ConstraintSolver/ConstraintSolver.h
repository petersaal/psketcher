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
	virtual void GetMeritValuePlusGradient(const mmcMatrix & x, double &value, mmcMatrix &gradient);

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

