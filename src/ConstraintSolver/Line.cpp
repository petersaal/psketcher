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

#include "Line.h"

Line :: Line(const PointPointer point1, const PointPointer point2)
{
	AddPrimitive(point1);
	AddPrimitive(point2);

	x1_ = point1->GetXDOF();
	y1_ = point1->GetYDOF();
	z1_ = point1->GetZDOF();

	x2_ = point2->GetXDOF();
	y2_ = point2->GetYDOF();
	z2_ = point2->GetZDOF();

	AddDOF(x1_);
	AddDOF(y1_);
	AddDOF(z1_);

	AddDOF(x2_);
	AddDOF(y2_);
	AddDOF(z2_);
}

void Line::ApplySelectionMask(SelectionMask mask)
{
	if(mask == All || mask == Edges || mask == Lines)
		SetSelectable(true);
	else
		SetSelectable(false);
}

