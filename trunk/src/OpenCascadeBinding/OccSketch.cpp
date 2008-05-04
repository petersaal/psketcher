#include "QtSketch.h"

QtSketch::QtSketch(Handle(AIS_InteractiveContext) ais_context, VectorPointer normal, VectorPointer up, PointPointer base, bool grid_snap):
Sketch(normal,up,base),
ais_context_(ais_context),
grid_snap_(grid_snap)
{
	//ais_context_->OpenLocalContext();

}

QtPoint2DPointer QtSketch::AddPoint2D ( double s, double t, bool s_free, bool t_free)
{
	QtPoint2DPointer new_point(new QtPoint2D(ais_context_,s,t,GetSketchPlane(),s_free,t_free));
	AddPrimitive(new_point);
	return new_point;
}


QtArc2DPointer QtSketch::AddArc2D (double s_center, double t_center, double theta_1, double theta_2, double radius, bool s_center_free, bool t_center_free, bool theta_1_free, bool theta_2_free, bool radius_free)
{
	QtArc2DPointer new_arc(new QtArc2D(ais_context_,s_center, t_center, theta_1, theta_2, radius, GetSketchPlane(),s_center_free, t_center_free, theta_1_free, theta_2_free, radius_free));
	AddPrimitive(new_arc);
	return new_arc;
}


QtLine2DPointer QtSketch::AddLine2D (const Point2DPointer point1, const Point2DPointer point2)
{
	QtLine2DPointer new_line(new QtLine2D(ais_context_,point1, point2, GetSketchPlane()));
	AddPrimitive(new_line);
	return new_line;
}


QtDistancePoint2DPointer QtSketch::AddDistancePoint2D(const Point2DPointer point1, const Point2DPointer point2, double distance)
{
	QtDistancePoint2DPointer new_constraint(new QtDistancePoint2D(ais_context_,point1,point2,distance));
	AddConstraintEquation(new_constraint);
	return new_constraint;
}

// Add a distance constraint using the current distance
QtDistancePoint2DPointer QtSketch::AddDistancePoint2D(const Point2DPointer point1, const Point2DPointer point2)
{
	QtDistancePoint2DPointer new_constraint(new QtDistancePoint2D(ais_context_,point1,point2,1.0));
	new_constraint->SetValue(new_constraint->GetActualDistance());
	new_constraint->UpdateDisplay();
	AddConstraintEquation(new_constraint);
	return new_constraint;
}

QtParallelLine2DPointer QtSketch::AddParallelLine2D(const Line2DPointer line1, const Line2DPointer line2)
{
	QtParallelLine2DPointer new_constraint (new QtParallelLine2D(ais_context_,line1, line2));
	AddConstraintEquation(new_constraint);
	return new_constraint;
}

QtAngleLine2DPointer QtSketch::AddAngleLine2D(const Line2DPointer line1, const Line2DPointer line2, double angle)
{
	QtAngleLine2DPointer new_constraint(new QtAngleLine2D(ais_context_,line1,line2,angle));
	AddConstraintEquation(new_constraint);
	return new_constraint;
}

QtTangentEdge2DPointer QtSketch::AddTangentEdge2D(Edge2DBasePointer edge1, EdgePointNumber point_num_1, Edge2DBasePointer edge2, EdgePointNumber point_num_2)
{
	QtTangentEdge2DPointer new_constraint(new QtTangentEdge2D(ais_context_,edge1, point_num_1, edge2, point_num_2));
	AddConstraintEquation(new_constraint);
	return new_constraint;
}

// returns the open cascade gp_Ax3 object for the current sketch plane
gp_Ax3 QtSketch::Get_gp_Ax3()
{
	// get the axis that define the plane of the circle (i_vector (x-axis), j_vector (y-axis), and normal_vector (z-axis))
	mmcMatrix j_vector = sketch_plane_->GetUp()->GetmmcMatrix();  // t axis direction vector in sketch plane
	mmcMatrix normal_vector = sketch_plane_->GetNormal()->GetmmcMatrix();
	mmcMatrix i_vector = j_vector.CrossProduct(normal_vector); // s axis direction vector in sketch plane
	mmcMatrix base = sketch_plane_->GetBase()->GetmmcMatrix();

	gp_Dir Zaxis(normal_vector(0,0),normal_vector(1,0),normal_vector(2,0));
	gp_Dir XvAxis(i_vector(0,0),i_vector(1,0),i_vector(2,0));
	gp_Pnt Origin(base(0,0),base(1,0),base(2,0));
	gp_Ax2 Csys(Origin,Zaxis,XvAxis);

	return Csys;
}
