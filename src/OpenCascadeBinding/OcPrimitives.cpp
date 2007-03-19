#include "OcPrimitives.h"

OcLine::OcLine (const LinePointer line, Handle(AIS_InteractiveContext) ais_context) :
OcPrimitiveBase(ais_context)
{
	line_ = line;

	// create the ais interactive object that will represent the line
	oc_point1_ = new Geom_CartesianPoint(line_->GetX1()->GetValue(),
																			 line_->GetY1()->GetValue(),
																			 line_->GetZ1()->GetValue());

	oc_point2_ = new Geom_CartesianPoint(line_->GetX2()->GetValue(),
																			 line_->GetY2()->GetValue(),
																			 line_->GetZ2()->GetValue());

	ais_object_ = new AIS_Line(oc_point1_, oc_point2_);

	// Display the newly create ais_object
	Display();
}


OcPoint::OcPoint (const PointPointer point, Handle(AIS_InteractiveContext) ais_context) :
OcPrimitiveBase(ais_context)
{
	point_ = point;
	
	// create the ais interactive object that will represent the point
	oc_point_ = new Geom_CartesianPoint(point_->GetXDOF()->GetValue(),
																			point_->GetYDOF()->GetValue(),
																			point_->GetZDOF()->GetValue());

	ais_object_ = new AIS_Point(oc_point_);

	// Display the newly create ais_object
	Display();
}

void OcPrimitiveBase::Display()
{
	ais_context_->Display(ais_object_, Standard_False);
}
