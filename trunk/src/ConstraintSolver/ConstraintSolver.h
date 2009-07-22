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

#include <map>
#include "SolverFunctions.h"
#include "../mmcMatrix/mmcMatrix.h"
#include "../NumOptimization/bfgs.h"

/* Now will define the merit function derived class used in the template matching */
class ConstraintSolver : public MeritFunction
{
public:
	ConstraintSolver(const std::vector<SolverFunctionsBasePointer> &constraints, const std::vector<double> & weights, const std::vector<DOFPointer> & free_parameters, const std::vector<DOFPointer> & fixed_parameters, const std::vector<double> & fixed_values);
	virtual ~ConstraintSolver() {;}

	virtual double GetMeritValue(const mmcMatrix & x);
	virtual mmcMatrix GetMeritGradient(const mmcMatrix & x);

private:
	std::vector<DOFPointer> free_parameters_;
	std::vector<DOFPointer> fixed_parameters_;
	mmcMatrix fixed_values_;
	std::vector<double> weights_;
	std::vector<SolverFunctionsBasePointer> constraints_;
};

#endif //ConstraintSolverH

