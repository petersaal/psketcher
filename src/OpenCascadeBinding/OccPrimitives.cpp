#include "OccPrimitives.h"

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

	ais_object_ = new AIS_Line(oc_point1_, oc_point2_);

	// Display the newly create ais_object
	Display();
}

OccLine2D::OccLine2D (Handle(AIS_InteractiveContext) ais_context, const Point2DPointer point1, const Point2DPointer point2, SketchPlanePointer sketch_plane) :
OccPrimitiveBase(ais_context),
Line2D(point1, point2, sketch_plane)
{
	double x1, y1, z1, x2, y2, z2;	

	Get3DLocations(x1, y1, z1, x2, y2, z2);

	// create the ais interactive object that will represent the line
	oc_point1_ = new Geom_CartesianPoint(x1, y1, z1);

	oc_point2_ = new Geom_CartesianPoint(x2, y2, z2);

	ais_object_ = new AIS_Line(oc_point1_, oc_point2_);

	// Display the newly create ais_object
	Display();
}

OccPoint::OccPoint (Handle(AIS_InteractiveContext) ais_context, double x, double y, double z, bool x_free, bool y_free, bool z_free) :
OccPrimitiveBase(ais_context),
Point(x,y,z,x_free,y_free,z_free)
{
	// create the ais interactive object that will represent the point
	oc_point_ = new Geom_CartesianPoint(GetXDOF()->GetValue(),
																			GetYDOF()->GetValue(),
																			GetZDOF()->GetValue());

	ais_object_ = new AIS_Point(oc_point_);

	// Display the newly create ais_object
	Display();
}

OccPoint2D::OccPoint2D (Handle(AIS_InteractiveContext) ais_context,double s, double t, SketchPlanePointer sketch_plane, bool s_free, bool t_free) :
OccPrimitiveBase(ais_context),
Point2D(s,t,sketch_plane,s_free,t_free)
{
	double x_position, y_position, z_position;

	Get3DLocation(x_position, y_position, z_position);

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
