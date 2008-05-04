#include "QtDistancePoint2D.h"

QtDistancePoint2D::QtDistancePoint2D(Handle(AIS_InteractiveContext) ais_context,const Point2DPointer point1, const Point2DPointer point2,double distance):
QtPrimitiveBase(ais_context),
DistancePoint2D(point1,point2,distance)
{
	GenerateAISObject();

	// Display the newly create ais_object
	Display();
}


void QtDistancePoint2D::UpdateDisplay()
{
	// first, erase the pervious AIS_ParallelRelation from the display because we'll have to recreate it
	Erase();
	
	GenerateAISObject();

	Display();

	QtPrimitiveBase::UpdateDisplay();
}

void QtDistancePoint2D::GenerateAISObject()
{
	// create the points needed to create the line segment
	double x1, y1, z1, x2, y2, z2;	
	point1_->Get3DLocation(x1, y1, z1);
	point2_->Get3DLocation(x2, y2, z2);

	oc_point1_.SetX(x1);
	oc_point1_.SetY(y1);
	oc_point1_.SetZ(z1);

	oc_point2_.SetX(x2);
	oc_point2_.SetY(y2);
	oc_point2_.SetZ(z2);


	// create the line segments (the AIS_ParallelRelation object needs to know where the lines are in order to display the parallel constraint)
	oc_shape1_ = BRepBuilderAPI_MakeVertex(oc_point1_);
	oc_shape2_ = BRepBuilderAPI_MakeVertex(oc_point2_);

	// define the plane that is needed by the AIS_parallelRelation object
	double coef_a, coef_b, coef_c, coef_d;
	point1_->GetSketchPlane()->GetABCD(coef_a,coef_b,coef_c,coef_d);
	oc_plane_ = new Geom_Plane(coef_a, coef_b, coef_c, coef_d);

	// create the interactive ais_object, this is what will actually be displayed
	ais_object_list_.push_back(new AIS_LengthDimension(oc_shape1_,oc_shape2_,oc_plane_,distance_->GetValue(),distance_->GetValue()));
}
