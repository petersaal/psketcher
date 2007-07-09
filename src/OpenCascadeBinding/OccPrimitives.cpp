#include "OccPrimitives.h"

OccLine::OccLine (const LinePointer line, Handle(AIS_InteractiveContext) ais_context) :
OccPrimitiveBase(ais_context)
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

OccLine2D::OccLine2D (const Line2DPointer line, Handle(AIS_InteractiveContext) ais_context) :
OccPrimitiveBase(ais_context)
{
	double x1, y1, z1, x2, y2, z2;	

	line_ = line;

	line_->Get3DLocations(x1, y1, z1, x2, y2, z2);

	// create the ais interactive object that will represent the line
	oc_point1_ = new Geom_CartesianPoint(x1, y1, z1);

	oc_point2_ = new Geom_CartesianPoint(x2, y2, z2);

	ais_object_ = new AIS_Line(oc_point1_, oc_point2_);

	// Display the newly create ais_object
	Display();
}

OccPoint::OccPoint (const PointPointer point, Handle(AIS_InteractiveContext) ais_context) :
OccPrimitiveBase(ais_context)
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

OccPoint2D::OccPoint2D (const Point2DPointer point, Handle(AIS_InteractiveContext) ais_context) :
OccPrimitiveBase(ais_context)
{
	double x_position, y_position, z_position;

	point_ = point;

	point_->Get3DLocation(x_position, y_position, z_position);

	// create the ais interactive object that will represent the point
	oc_point_ = new Geom_CartesianPoint(x_position, y_position, z_position);

	ais_object_ = new AIS_Point(oc_point_);

	// Display the newly create ais_object
	Display();
}

void OccPrimitiveBase::Display()
{
	ais_context_->Display(ais_object_, Standard_False);
}
