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

#include "Edge2DBase.h"

using namespace std;

Edge2DBase::Edge2DBase(SketchPlanePointer sketch_plane):
Primitive2DBase(sketch_plane)
{
}

Edge2DBase::Edge2DBase()
{
}

void Edge2DBase::ApplySelectionMask(SelectionMask mask)
{
	if(mask == All || mask == Edges)
		SetSelectable(true);
	else
		SetSelectable(false);
}

bool Edge2DBase::IsPointCoincident(EdgePointNumber my_point_number, boost::shared_ptr<Edge2DBase> other_edge, EdgePointNumber other_point_number)
{
	bool result;

	Point2DPointer my_point;
	Point2DPointer other_point;
	
	// assign the Point pointers
	if(my_point_number == Point1)
		my_point = GetPoint1();
	else
		my_point = GetPoint2();

	if(other_point_number == Point1)
		other_point = other_edge->GetPoint1();
	else
		other_point = other_edge->GetPoint2();

	// perform the comparison
	if(my_point->GetSDOF() == other_point->GetSDOF() && my_point->GetTDOF() == other_point->GetTDOF())
		result = true;
	else
		result = false;

	return result;
}