#include "OccLine.h"

OccLine::OccLine (Handle(AIS_InteractiveContext) ais_context, const PointPointer point1, const PointPointer point2) :
OccPrimitiveBase(ais_context),
Line(point1,point2)
{
	// create the ais interactive object that will represent the line
	oc_point1_ = new Geom_CartesianPoint(GetX1()->GetValue(),
																			 GetY1()->GetValue(),
																			 GetZ1()->GetValue());

	oc_point2_ = new Geom_CartesianPoint(GetX2()->GetValue(),
																			 GetY2()->GetValue(),
																			 GetZ2()->GetValue());

	ais_object_list_.push_back(new AIS_Line(oc_point1_, oc_point2_));

	// Display the newly create ais_object
	Display();
}

void OccLine::UpdateDisplay()
{
	oc_point1_->SetCoord(GetX1()->GetValue(),
											 GetY1()->GetValue(),
											 GetZ1()->GetValue());

	oc_point2_->SetCoord(GetX2()->GetValue(),
											 GetY2()->GetValue(),
											 GetZ2()->GetValue());

	OccPrimitiveBase::UpdateDisplay();
}
