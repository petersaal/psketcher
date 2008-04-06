#include "OccPoint.h"


OccPoint::OccPoint (Handle(AIS_InteractiveContext) ais_context, double x, double y, double z, bool x_free, bool y_free, bool z_free) :
OccPrimitiveBase(ais_context),
Point(x,y,z,x_free,y_free,z_free)
{
	// create the ais interactive object that will represent the point
	oc_point_ = new Geom_CartesianPoint(GetXDOF()->GetValue(),
																			GetYDOF()->GetValue(),
																			GetZDOF()->GetValue());

	ais_object_list_.push_back(new AIS_Point(oc_point_));

	// Display the newly create ais_object
	Display();
}

void OccPoint::UpdateDisplay()
{
	oc_point_->SetCoord(GetXDOF()->GetValue(),
											GetYDOF()->GetValue(),
											GetZDOF()->GetValue());

	OccPrimitiveBase::UpdateDisplay();
}


