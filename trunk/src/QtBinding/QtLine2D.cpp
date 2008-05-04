#include "QtLine2D.h"


QtLine2D::QtLine2D (Handle(AIS_InteractiveContext) ais_context, const Point2DPointer point1, const Point2DPointer point2, SketchPlanePointer sketch_plane) :
QtPrimitiveBase(ais_context),
Line2D(point1, point2, sketch_plane)
{
	double x1, y1, z1, x2, y2, z2;	

	Get3DLocations(x1, y1, z1, x2, y2, z2);

	// create the ais interactive object that will represent the line
	oc_point1_ = new Geom_CartesianPoint(x1, y1, z1);

	oc_point2_ = new Geom_CartesianPoint(x2, y2, z2);

	ais_object_list_.push_back(new AIS_Line(oc_point1_, oc_point2_));

	// Display the newly create ais_object
	Display();
}


TopoDS_Shape QtLine2D::GetTopoDS_Shape()
{
	// create an instance of a TopoDS_Shape using the current geometry
	double x1, y1, z1, x2, y2, z2;	
	Get3DLocations(x1, y1, z1, x2, y2, z2);
	
	gp_Pnt oc_point1;
	gp_Pnt oc_point2;

	oc_point1.SetX(x1);
	oc_point1.SetY(y1);
	oc_point1.SetZ(z1);

	oc_point2.SetX(x2);
	oc_point2.SetY(y2);
	oc_point2.SetZ(z2);

	// create the line segment 
	TopoDS_Shape line_shape = BRepBuilderAPI_MakeEdge(oc_point1,oc_point2);

	return line_shape;
}

void QtLine2D::UpdateDisplay()
{
	double x1, y1, z1, x2, y2, z2;	

	Get3DLocations(x1, y1, z1, x2, y2, z2);

	oc_point1_->SetCoord(x1, y1, z1);

	oc_point2_->SetCoord(x2, y2, z2);

	QtPrimitiveBase::UpdateDisplay();
}
