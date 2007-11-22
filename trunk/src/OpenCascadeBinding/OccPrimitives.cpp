
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

OccLine2D::OccLine2D (Handle(AIS_InteractiveContext) ais_context, const Point2DPointer point1, const Point2DPointer point2, SketchPlanePointer sketch_plane) :
OccPrimitiveBase(ais_context),
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


TopoDS_Shape OccLine2D::GetTopoDS_Shape()
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

void OccPoint2D::UpdateDisplay()
{
	double x_position, y_position, z_position;

	Get3DLocation(x_position, y_position, z_position);

	oc_point_->SetCoord(x_position, y_position, z_position);

	OccPrimitiveBase::UpdateDisplay();
}

void OccPrimitiveBase::Display()
{
	for(unsigned int current_index = 0; current_index < ais_object_list_.size(); current_index++)
	{
		ais_context_->Display(ais_object_list_[current_index], Standard_False);
	}
}

void OccPrimitiveBase::UpdateDisplay()
{
	for(unsigned int current_index = 0; current_index < ais_object_list_.size(); current_index++)
	{
		ais_context_->Redisplay(ais_object_list_[current_index]);
	}
}

bool OccPrimitiveBase::IsSelected()
{
	bool selected = false;

	for(unsigned int current_index = 0; current_index < ais_object_list_.size(); current_index++)
	{
		if(ais_context_->IsCurrent(ais_object_list_[current_index]))
			selected = true;
	}

	return selected;
}

void OccPrimitiveBase::SetSelectable(bool selectable)
{
	for(unsigned int current_index = 0; current_index < ais_object_list_.size(); current_index++)
	{
		if(!selectable)
		{
			ais_context_->Deactivate(ais_object_list_[current_index]);
			//ais_context_->Remove(ais_object_list_[current_index]);
			//ais_context_->Display(ais_object_list_[current_index],0, -1);
			//ais_context_->Redisplay(ais_object_list_[current_index]);
		} else {
			ais_context_->Activate(ais_object_list_[current_index]);
			//ais_context_->Deactivate(ais_object_list_[current_index]);
			//ais_context_->Remove(ais_object_list_[current_index]);
			//ais_context_->Display(ais_object_list_[current_index],0, 0);
			//ais_context_->Redisplay(ais_object_list_[current_index]);
		}
	}
}

// Erase current ais_object from the ais_context
void OccPrimitiveBase::Erase()
{
	for(unsigned int current_index = 0; current_index < ais_object_list_.size(); current_index++)
	{
		ais_context_->Erase(ais_object_list_[current_index]);
	}

	ais_object_list_.clear();
}

OccParallelLine2D::OccParallelLine2D(Handle(AIS_InteractiveContext) ais_context, const Line2DPointer line1, const Line2DPointer line2):
OccPrimitiveBase(ais_context),
ParallelLine2D(line1,line2)
{
	GenerateAISObject();

	// Display the newly create ais_object
	Display();
}


void OccParallelLine2D::UpdateDisplay()
{
	// first, erase the pervious AIS_ParallelRelation from the display because we'll have to recreate it
	Erase();
	
	GenerateAISObject();

	Display();

	OccPrimitiveBase::UpdateDisplay();
}

void OccParallelLine2D::GenerateAISObject()
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
	ais_object_list_.push_back(new AIS_ParallelRelation(oc_shape1_,oc_shape2_,oc_plane_));
}



OccDistancePoint2D::OccDistancePoint2D(Handle(AIS_InteractiveContext) ais_context,const Point2DPointer point1, const Point2DPointer point2,double distance):
OccPrimitiveBase(ais_context),
DistancePoint2D(point1,point2,distance)
{
	GenerateAISObject();

	// Display the newly create ais_object
	Display();
}


void OccDistancePoint2D::UpdateDisplay()
{
	// first, erase the pervious AIS_ParallelRelation from the display because we'll have to recreate it
	Erase();
	
	GenerateAISObject();

	Display();

	OccPrimitiveBase::UpdateDisplay();
}

void OccDistancePoint2D::GenerateAISObject()
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
	ais_object_list_.push_back(new AIS_LengthDimension(oc_shape1_,oc_shape2_,oc_plane_,distance_,distance_));
}


OccAngleLine2D::OccAngleLine2D(Handle(AIS_InteractiveContext) ais_context, const Line2DPointer line1, const Line2DPointer line2, double angle):
OccPrimitiveBase(ais_context),
AngleLine2D(line1,line2,angle)
{
	GenerateAISObject();

	// Display the newly create ais_object
	Display();
}


void OccAngleLine2D::UpdateDisplay()
{
	// first, erase the pervious AIS_ParallelRelation from the display because we'll have to recreate it
	Erase();
	
	GenerateAISObject();

	Display();

	OccPrimitiveBase::UpdateDisplay();
}

void OccAngleLine2D::GenerateAISObject()
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
	ais_object_list_.push_back(new AIS_AngleDimension(oc_shape1_,oc_shape2_,oc_plane_,-angle_*(180.0/mmcPI),angle_*(180.0/mmcPI)));
}


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

	// create the arc AIS object
	ais_object_list_.push_back(new AIS_Circle(Circ,GetTheta1()->GetValue(),GetTheta2()->GetValue(),Standard_True));

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

	// create the arc TopoDS_Shape
	TopoDS_Shape arc_shape = BRepBuilderAPI_MakeEdge(Circ,GetTheta1()->GetValue(),GetTheta2()->GetValue());
	return arc_shape;
}

void OccArc2D::UpdateDisplay()
{
	// first, erase the pervious AIS_ParallelRelation from the display because we'll have to recreate it
	Erase();
	
	GenerateAISObject();

	Display();

	OccPrimitiveBase::UpdateDisplay();
}


OccTangentEdge2D::OccTangentEdge2D (Handle(AIS_InteractiveContext) ais_context,
                       Edge2DBasePointer edge1, EdgePointNumber point_num_1, 
                       Edge2DBasePointer edge2, EdgePointNumber point_num_2):
OccPrimitiveBase(ais_context),
TangentEdge2D(edge1,point_num_1,edge2,point_num_2)
{
	GenerateAISObject();

	// Display the newly create ais_object
	Display();
}


void OccTangentEdge2D::UpdateDisplay()
{
	// first, erase the pervious AIS_ParallelRelation from the display because we'll have to recreate it
	Erase();
	
	GenerateAISObject();

	Display();

	OccPrimitiveBase::UpdateDisplay();
}

void OccTangentEdge2D::GenerateAISObject()
{
	// First, create opencascade versions of each of the two edges
	// the edges can be either a line or an arc
	// this will need to be updated as more edge types are introduced
	
	Edge2DBasePointer current_edge = edge1_;
	bool done = false;
	TopoDS_Shape current_shape;
	int loop_count = 0;
	while(!done)
	{
		if(dynamic_cast<Line2D*>(current_edge.get()) != 0){
			Line2D *current_line = dynamic_cast<Line2D*>(current_edge.get());
			OccLine2DPointer occ_line(new OccLine2D(ais_context_,current_line->GetPoint1(),current_line->GetPoint2(),current_line->GetSketchPlane()));
			current_shape = occ_line->GetTopoDS_Shape();
		}else if(dynamic_cast<Arc2D*>(current_edge.get()) != 0){
			Arc2D *current_arc = dynamic_cast<Arc2D*>(current_edge.get());

			OccArc2DPointer occ_arc(new OccArc2D(ais_context_,current_arc->GetSCenter(),current_arc->GetTCenter(),current_arc->GetTheta1(),current_arc->GetTheta2(),current_arc->GetRadius(),current_arc->GetSketchPlane()));

			current_shape = occ_arc->GetTopoDS_Shape();
		}else {
			throw PrimitiveException();
		}
			
		loop_count++;
	
		if(loop_count == 2)
		{
			done = true;
			oc_shape2_ = current_shape;
		} else {
			oc_shape1_ = current_shape;
			current_edge = edge2_;
		}
	}


	if(oc_shape1_.IsNull() || oc_shape2_.IsNull())
		throw PrimitiveException();

	// define the plane that is needed by the AIS_parallelRelation object
	double coef_a, coef_b, coef_c, coef_d;
	edge1_->GetSketchPlane()->GetABCD(coef_a,coef_b,coef_c,coef_d);
	oc_plane_ = new Geom_Plane(coef_a, coef_b, coef_c, coef_d);

	// create the interactive ais_object, this is what will actually be displayed
	ais_object_list_.push_back(new AIS_TangentRelation(oc_shape1_,oc_shape2_,oc_plane_));
}


