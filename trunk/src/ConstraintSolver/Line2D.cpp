#include "Line2D.h"

using namespace std;
using namespace GiNaC;

void Line2D::GetTangent1(GiNaC::ex & s_component, GiNaC::ex & t_component)
{
	ex length = sqrt(pow(GetS1()->GetVariable()-GetS2()->GetVariable(),2)+pow(GetT1()->GetVariable()-GetT2()->GetVariable(),2));
	
	s_component = (GetS1()->GetVariable() - GetS2()->GetVariable())/length;
	t_component = (GetT1()->GetVariable() - GetT2()->GetVariable())/length;
}

void Line2D::GetTangent2(GiNaC::ex & s_component, GiNaC::ex & t_component)
{
	ex length = sqrt(pow(GetS1()->GetVariable()-GetS2()->GetVariable(),2)+pow(GetT1()->GetVariable()-GetT2()->GetVariable(),2));
	
	s_component = (GetS2()->GetVariable() - GetS1()->GetVariable())/length;
	t_component = (GetT2()->GetVariable() - GetT1()->GetVariable())/length;
}

// returns global coordinates of line end points
void Line2D :: Get3DLocations(double & x1, double & y1, double & z1,
															double & x2, double & y2, double & z2)
{
	sketch_plane_->Get3DLocation(s1_->GetValue(), t1_->GetValue(), x1, y1, z1);
	sketch_plane_->Get3DLocation(s2_->GetValue(), t2_->GetValue(), x2, y2, z2);
}

void Line2D::ApplySelectionMask(SelectionMask mask)
{
	if(mask == All || mask == Edges || mask == Lines)
		SetSelectable(true);
	else
		SetSelectable(false);
}

Line2D :: Line2D(const Point2DPointer point1, const Point2DPointer point2, SketchPlanePointer sketch_plane):
Edge2DBase(sketch_plane)
{
	AddPrimitive(point1);
	AddPrimitive(point2);
	AddPrimitive(sketch_plane);

	s1_ = point1->GetSDOF();
	t1_ = point1->GetTDOF();

	s2_ = point2->GetSDOF();
	t2_ = point2->GetTDOF();

	AddDOF(s1_);
	AddDOF(t1_);

	AddDOF(s2_);
	AddDOF(t2_);
}

Point2DPointer Line2D::GetPoint1()
{
	// create the actual point object
	Point2DPointer result(new Point2D(GetS1(), GetT1(), GetSketchPlane()));
	return result;
}

Point2DPointer Line2D::GetPoint2()
{
	// create the actual point object
	Point2DPointer result(new Point2D(GetS2(), GetT2(), GetSketchPlane()));
	return result;
}
