#include "OccArc2D.h"

OccArc2D::OccArc2D (Handle(AIS_InteractiveContext) ais_context, double s_center, double t_center, double theta_1, double theta_2, double radius, 
					SketchPlanePointer sketch_plane, bool s_center_free, bool t_center_free, bool theta_1_free, bool theta_2_free, 
					bool radius_free):
OccPrimitiveBase(ais_context),
Arc2D(s_center,t_center,theta_1,theta_2,radius,sketch_plane, s_center_free, t_center_free, theta_1_free, theta_2_free,radius_free)
{
	GenerateAISObject();

	// Display the newly create ais_object
	Display();
}

OccArc2D::OccArc2D (Handle(AIS_InteractiveContext) ais_context,DOFPointer s_center, DOFPointer t_center, DOFPointer theta_1, DOFPointer theta_2, DOFPointer radius, SketchPlanePointer sketch_plane):
OccPrimitiveBase(ais_context),
Arc2D(s_center,t_center,theta_1,theta_2,radius,sketch_plane)
{
	GenerateAISObject();

	// Display the newly create ais_object
	Display();
}

void OccArc2D::GenerateAISObject()
{
	// get the axis that define the plane of the circle (i_vector (x-axis), j_vector (y-axis), and normal_vector (z-axis))
	mmcMatrix j_vector = sketch_plane_->GetUp()->GetmmcMatrix();  // t axis direction vector in sketch plane
	mmcMatrix normal_vector = sketch_plane_->GetNormal()->GetmmcMatrix();
	mmcMatrix i_vector = j_vector.CrossProduct(normal_vector); // s axis direction vector in sketch plane

	// get the center coordinates for the circle
	double x_center,y_center,z_center;
	Get3DLocations(x_center, y_center, z_center);

	gp_Dir Zaxis(normal_vector(0,0),normal_vector(1,0),normal_vector(2,0));
	gp_Dir XvAxis(i_vector(0,0),i_vector(1,0),i_vector(2,0));
	gp_Pnt Origin(x_center,y_center,z_center);
	gp_Ax2 Csys(Origin,Zaxis,XvAxis);
	Handle(Geom_Circle) Circ = new Geom_Circle(Csys,GetRadius()->GetValue());

	// create the arc AIS object (for OCC, Theta1 must be less than Theta2)
	if(GetTheta1()->GetValue() <= GetTheta2()->GetValue())
		ais_object_list_.push_back(new AIS_Circle(Circ,GetTheta1()->GetValue(),GetTheta2()->GetValue(),Standard_True));
	else
		ais_object_list_.push_back(new AIS_Circle(Circ,GetTheta2()->GetValue(),GetTheta1()->GetValue(),Standard_True));

	// create the radius dimension
	// Only display the radius if it is not a free parameter
	// If it is a free parameter, it is not really a constraint and should not be displayed as such
	if( ! radius_->IsFree())
	{
		// create TopoDS object to attach radius dimension to
		TopoDS_Edge oc_arc = BRepBuilderAPI_MakeEdge(Circ,GetTheta1()->GetValue(),GetTheta2()->GetValue());

		// create the radius dimension interactive object
		ais_object_list_.push_back(new AIS_RadiusDimension(oc_arc,radius_->GetValue(),radius_->GetValue()));
	}

	// create the end point AIS objects and the center point AIS object
	// endpoint 1
	double x_location, y_location, z_location;
	sketch_plane_->Get3DLocation(s_center_->GetValue()+radius_->GetValue()*cos(theta_1_->GetValue()),
								 t_center_->GetValue()+radius_->GetValue()*sin(theta_1_->GetValue()), x_location, y_location, z_location);
	Handle(Geom_CartesianPoint) oc_point_1;
	oc_point_1 = new Geom_CartesianPoint(x_location, y_location, z_location);
	ais_object_list_.push_back(new AIS_Point(oc_point_1));
	
	// endpoint 2
	sketch_plane_->Get3DLocation(s_center_->GetValue()+radius_->GetValue()*cos(theta_2_->GetValue()),
								 t_center_->GetValue()+radius_->GetValue()*sin(theta_2_->GetValue()), x_location, y_location, z_location);
	Handle(Geom_CartesianPoint) oc_point_2;
	oc_point_2 = new Geom_CartesianPoint(x_location, y_location, z_location);
	ais_object_list_.push_back(new AIS_Point(oc_point_2));

	// center point
	sketch_plane_->Get3DLocation(s_center_->GetValue(),t_center_->GetValue(), x_location, y_location, z_location);
	Handle(Geom_CartesianPoint) oc_point_center;
	oc_point_center = new Geom_CartesianPoint(x_location, y_location, z_location);
	ais_object_list_.push_back(new AIS_Point(oc_point_center));
}

TopoDS_Shape OccArc2D::GetTopoDS_Shape()
{
	// get the axis that define the plane of the circle (i_vector (x-axis), j_vector (y-axis), and normal_vector (z-axis))
	mmcMatrix j_vector = sketch_plane_->GetUp()->GetmmcMatrix();  // t axis direction vector in sketch plane
	mmcMatrix normal_vector = sketch_plane_->GetNormal()->GetmmcMatrix();
	mmcMatrix i_vector = j_vector.CrossProduct(normal_vector); // s axis direction vector in sketch plane

	// get the center coordinates for the circle
	double x_center,y_center,z_center;
	Get3DLocations(x_center, y_center, z_center);

	gp_Dir Zaxis(normal_vector(0,0),normal_vector(1,0),normal_vector(2,0));
	gp_Dir XvAxis(i_vector(0,0),i_vector(1,0),i_vector(2,0));
	gp_Pnt Origin(x_center,y_center,z_center);
	gp_Ax2 Csys(Origin,Zaxis,XvAxis);
	Handle(Geom_Circle) Circ = new Geom_Circle(Csys,GetRadius()->GetValue());

	// create the arc TopoDS_Shape (for OCC, Theta1 must be less than Theta2)
	TopoDS_Shape arc_shape;
	if(GetTheta1()->GetValue() <= GetTheta2()->GetValue())
		arc_shape = BRepBuilderAPI_MakeEdge(Circ,GetTheta1()->GetValue(),GetTheta2()->GetValue());
	else
		arc_shape = BRepBuilderAPI_MakeEdge(Circ,GetTheta2()->GetValue(),GetTheta1()->GetValue());

	return arc_shape;
}

void OccArc2D::UpdateDisplay()
{
	/*
	std::cout << "s center = " << GetSCenter()->GetValue() << std::endl;
	std::cout << "t center = " << GetTCenter()->GetValue() << std::endl;
	std::cout << "theta 1 = " << GetTheta1()->GetValue() << std::endl;
	std::cout << "theta 2 = " << GetTheta2()->GetValue() << std::endl;
	std::cout << "radius = " << GetRadius()->GetValue() << std::endl;
	*/

	// first, erase the pervious AIS_ParallelRelation from the display because we'll have to recreate it
	Erase();
	
	GenerateAISObject();

	Display();

	OccPrimitiveBase::UpdateDisplay();
}
