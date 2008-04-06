#include "Line.h"

Line :: Line(const PointPointer point1, const PointPointer point2)
{
	x1_ = point1->GetXDOF();
	y1_ = point1->GetYDOF();
	z1_ = point1->GetZDOF();

	x2_ = point2->GetXDOF();
	y2_ = point2->GetYDOF();
	z2_ = point2->GetZDOF();

	dof_list_.push_back(x1_);
	dof_list_.push_back(y1_);
	dof_list_.push_back(z1_);

	dof_list_.push_back(x2_);
	dof_list_.push_back(y2_);
	dof_list_.push_back(z2_);
}

void Line::ApplySelectionMask(SelectionMask mask)
{
	if(mask == All || mask == Edges || mask == Lines)
		SetSelectable(true);
	else
		SetSelectable(false);
}

