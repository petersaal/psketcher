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

#include "DistancePoint.h"
#include "IndependentDOF.h"

using namespace std;
using namespace GiNaC;

// Create a constraint that defines the distance between two points
DistancePoint::DistancePoint(const PointPointer point1, const PointPointer point2, double distance)
{
	AddPrimitive(point1);
	AddPrimitive(point2);

	// Create a DOF for the distance parameter
	DOFPointer new_dof(new IndependentDOF(distance,false));
	
	AddDOF(new_dof);

	// create the expression that defines the constraint and add it the the constraint list
	boost::shared_ptr<ex> new_constraint(new ex);
	
	*new_constraint = sqrt( pow(point1->GetXDOF()->GetVariable() - point2->GetXDOF()->GetVariable(),2) +
													pow(point1->GetYDOF()->GetVariable() - point2->GetYDOF()->GetVariable(),2) +
													pow(point1->GetZDOF()->GetVariable() - point2->GetZDOF()->GetVariable(),2))
										- new_dof->GetVariable();

	constraints_.push_back(new_constraint);
	weight_list_.push_back(1.0);
}
