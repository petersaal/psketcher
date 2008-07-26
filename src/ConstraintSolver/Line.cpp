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

