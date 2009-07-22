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

#ifndef ConstraintEquationBaseH
#define ConstraintEquationBaseH

#include "PrimitiveBase.h"
#include "SolverFunctions.h"

// constraint equation class
class ConstraintEquationBase : virtual public PrimitiveBase
{
	public:
		virtual ~ConstraintEquationBase() {;}
	
		// Accessor methods
		SolverFunctionsBasePointer GetSolverFunction() {return solver_function_;}
		double GetWeight() const {return weight_;}

		virtual void ApplySelectionMask(SelectionMask mask);

	protected:
		// The solver function that defines this constraint
        SolverFunctionsBasePointer solver_function_;

		// this vector stores the weight for this constraint
		double weight_;
};
typedef boost::shared_ptr<ConstraintEquationBase> ConstraintEquationBasePointer;


#endif //ConstraintEquationBaseH
