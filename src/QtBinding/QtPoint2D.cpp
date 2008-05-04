#include "OccPoint2D.h"

OccPoint2D::OccPoint2D (Handle(AIS_InteractiveContext) ais_context,double s, double t, SketchPlanePointer sketch_plane, bool s_free, bool t_free) :
OccPrimitiveBase(ais_context),
Point2D(s,t,sketch_plane,s_free,t_free)
{
	double x_position, y_position, z_position;

	Get3DLocation(x_position, y_position, z_position);

	// create the ais interactive object that will represent the point
	oc_point_ = new Geom_CartesianPoint(x_position, y_position, z_position);

	ais_object_list_.push_back(new AIS_Point(oc_point_));

	// Display the newly create ais_object
	Display();
}

OccPoint2D::OccPoint2D (Handle(AIS_InteractiveContext) ais_context, DOFPointer s, DOFPointer t, SketchPlanePointer sketch_plane) :
OccPrimitiveBase(ais_context),
Point2D(s,t,sketch_plane)
{
	double x_position, y_position, z_position;

	Get3DLocation(x_position, y_position, z_position);

	// create the ais interactive object that will represent the point
	oc_point_ = new Geom_CartesianPoint(x_position, y_position, z_position);

	ais_object_list_.push_back(new AIS_Point(oc_point_));

	// Display the newly create ais_object
	Display();
}

void OccPoint2D::UpdateDisplay()
{
	double x_position, y_position, z_position;

	Get3DLocation(x_position, y_position, z_position);

	oc_point_->SetCoord(x_position, y_position, z_position);

	OccPrimitiveBase::UpdateDisplay();
}
