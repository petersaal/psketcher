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

void OccLine2D::UpdateDisplay()
{
	double x1, y1, z1, x2, y2, z2;	

	Get3DLocations(x1, y1, z1, x2, y2, z2);

	oc_point1_->SetCoord(x1, y1, z1);

	oc_point2_->SetCoord(x2, y2, z2);

	OccPrimitiveBase::UpdateDisplay();
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

void OccPoint::UpdateDisplay()
{
	oc_point_->SetCoord(GetXDOF()->GetValue(),
											GetYDOF()->GetValue(),
											GetZDOF()->GetValue());

	OccPrimitiveBase::UpdateDisplay();
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

void OccPoint2D::UpdateDisplay()
{
	double x_position, y_position, z_position;

	Get3DLocation(x_position, y_position, z_position);

	oc_point_->SetCoord(x_position, y_position, z_position);

	OccPrimitiveBase::UpdateDisplay();
}

void OccPrimitiveBase::Display()
{
	ais_context_->Display(ais_object_, Standard_False);
}

// Erase current ais_object from the ais_context
void OccPrimitiveBase::Erase()
{
	ais_context_->Erase(ais_object_);
}

OccParallelConstraintLine2DLine2D::OccParallelConstraintLine2DLine2D(Handle(AIS_InteractiveContext) ais_context, const Line2DPointer line1, const Line2DPointer line2):
OccPrimitiveBase(ais_context),
ParallelConstraintLine2DLine2D(line1,line2)
{
	GenerateAISObject();

	// Display the newly create ais_object
	Display();
}


void OccParallelConstraintLine2DLine2D::UpdateDisplay()
{
	// first, erase the pervious AIS_ParallelRelation from the display because we'll have to recreate it
	Erase();
	
	GenerateAISObject();

	Display();

	OccPrimitiveBase::UpdateDisplay();
}

void OccParallelConstraintLine2DLine2D::GenerateAISObject()
{
	// create the points needed to create the line segment
	double x1, y1, z1, x2, y2, z2;	
	line1_->Get3DLocations(x1, y1, z1, x2, y2, z2);

	double x3, y3, z3, x4, y4, z4;	
	line2_->Get3DLocations(x3, y3, z3, x4, y4, z4);

	oc_point1_.SetX(x1);
	oc_point1_.SetY(y1);
	oc_point1_.SetZ(z1);

	oc_point2_.SetX(x2);
	oc_point2_.SetY(y2);
	oc_point2_.SetZ(z2);

	oc_point3_.SetX(x3);
	oc_point3_.SetY(y3);
	oc_point3_.SetZ(z3);

	oc_point4_.SetX(x4);
	oc_point4_.SetY(y4);
	oc_point4_.SetZ(z4);

	// create the line segments (the AIS_ParallelRelation object needs to know where the lines are in order to display the parallel constraint)
	oc_shape1_ = BRepBuilderAPI_MakeEdge(oc_point1_,oc_point2_);
	oc_shape2_ = BRepBuilderAPI_MakeEdge(oc_point3_,oc_point4_);

	// define the plane that is needed by the AIS_parallelRelation object
	double coef_a, coef_b, coef_c, coef_d;
	line1_->GetSketchPlane()->GetABCD(coef_a,coef_b,coef_c,coef_d);
	oc_plane_ = new Geom_Plane(coef_a, coef_b, coef_c, coef_d);

	// create the interactive ais_object, this is what will actually be displayed
	ais_object_ = new AIS_ParallelRelation(oc_shape1_,oc_shape2_,oc_plane_);
}



OccDistanceConstraintPoint2DPoint2D::OccDistanceConstraintPoint2DPoint2D(Handle(AIS_InteractiveContext) ais_context,const Point2DPointer point1, const Point2DPointer point2,double distance):
OccPrimitiveBase(ais_context),
DistanceConstraintPoint2DPoint2D(point1,point2,distance)
{
	GenerateAISObject();

	// Display the newly create ais_object
	Display();
}


void OccDistanceConstraintPoint2DPoint2D::UpdateDisplay()
{
	// first, erase the pervious AIS_ParallelRelation from the display because we'll have to recreate it
	Erase();
	
	GenerateAISObject();

	Display();

	OccPrimitiveBase::UpdateDisplay();
}

void OccDistanceConstraintPoint2DPoint2D::GenerateAISObject()
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
	ais_object_ = new AIS_LengthDimension(oc_shape1_,oc_shape2_,oc_plane_,distance_,distance_);
}
