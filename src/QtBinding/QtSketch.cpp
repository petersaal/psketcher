#include "QtSketch.h"

QtSketch::QtSketch(VectorPointer normal, VectorPointer up, PointPointer base, bool grid_snap):
Sketch(normal,up,base),
grid_snap_(grid_snap)
{

}

QtPoint2DPointer QtSketch::AddPoint2D ( double s, double t, bool s_free, bool t_free)
{
	QtPoint2DPointer new_point(new QtPoint2D(0,s,t,GetSketchPlane(),s_free,t_free));
	AddPrimitive(new_point);
	return new_point;
}


QtArc2DPointer QtSketch::AddArc2D (double s_center, double t_center, double theta_1, double theta_2, double radius, bool s_center_free, bool t_center_free, bool theta_1_free, bool theta_2_free, bool radius_free)
{
	QtArc2DPointer new_arc(new QtArc2D(0,s_center, t_center, theta_1, theta_2, radius, GetSketchPlane(),s_center_free, t_center_free, theta_1_free, theta_2_free, radius_free));
	AddPrimitive(new_arc);
	return new_arc;
}


QtLine2DPointer QtSketch::AddLine2D (const Point2DPointer point1, const Point2DPointer point2)
{
	QtLine2DPointer new_line(new QtLine2D(0,point1, point2, GetSketchPlane()));
	AddPrimitive(new_line);
	return new_line;
}


QtDistancePoint2DPointer QtSketch::AddDistancePoint2D(const Point2DPointer point1, const Point2DPointer point2, double distance)
{
	QtDistancePoint2DPointer new_constraint(new QtDistancePoint2D(0,point1,point2,distance));
	AddConstraintEquation(new_constraint);
	return new_constraint;
}

// Add a distance constraint using the current distance
QtDistancePoint2DPointer QtSketch::AddDistancePoint2D(const Point2DPointer point1, const Point2DPointer point2)
{
	QtDistancePoint2DPointer new_constraint(new QtDistancePoint2D(0,point1,point2,1.0));
	new_constraint->SetValue(new_constraint->GetActualDistance());
	new_constraint->UpdateDisplay();
	AddConstraintEquation(new_constraint);
	return new_constraint;
}

QtParallelLine2DPointer QtSketch::AddParallelLine2D(const Line2DPointer line1, const Line2DPointer line2)
{
	QtParallelLine2DPointer new_constraint (new QtParallelLine2D(0,line1, line2));
	AddConstraintEquation(new_constraint);
	return new_constraint;
}

QtAngleLine2DPointer QtSketch::AddAngleLine2D(const Line2DPointer line1, const Line2DPointer line2, double angle)
{
	QtAngleLine2DPointer new_constraint(new QtAngleLine2D(0,line1,line2,angle));
	AddConstraintEquation(new_constraint);
	return new_constraint;
}

QtTangentEdge2DPointer QtSketch::AddTangentEdge2D(Edge2DBasePointer edge1, EdgePointNumber point_num_1, Edge2DBasePointer edge2, EdgePointNumber point_num_2)
{
	QtTangentEdge2DPointer new_constraint(new QtTangentEdge2D(0,edge1, point_num_1, edge2, point_num_2));
	AddConstraintEquation(new_constraint);
	return new_constraint;
}

