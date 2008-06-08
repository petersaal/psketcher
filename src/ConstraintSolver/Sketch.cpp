#include "Sketch.h"

Sketch::Sketch(VectorPointer normal, VectorPointer up, PointPointer base):
sketch_plane_(new SketchPlane(normal,up,base))
{

}

Point2DPointer Sketch::AddPoint2D ( double s, double t, bool s_free, bool t_free)
{
	Point2DPointer new_point(new Point2D(s,t,sketch_plane_,s_free,t_free));
	AddPrimitive(new_point);
	return new_point;
}


Arc2DPointer Sketch::AddArc2D (double s_center, double t_center, double theta_1, double theta_2, double radius, bool s_center_free, bool t_center_free, bool theta_1_free, bool theta_2_free, bool radius_free)
{
	Arc2DPointer new_arc(new Arc2D(s_center, t_center, theta_1, theta_2, radius, sketch_plane_,s_center_free, t_center_free, theta_1_free, theta_2_free, radius_free));
	AddPrimitive(new_arc);
	return new_arc;
}


Line2DPointer Sketch::AddLine2D (const Point2DPointer point1, const Point2DPointer point2)
{
	Line2DPointer new_line(new Line2D(point1, point2, sketch_plane_));
	AddPrimitive(new_line);
	return new_line;
}


DistancePoint2DPointer Sketch::AddDistancePoint2D(const Point2DPointer point1, const Point2DPointer point2, double distance)
{
	DistancePoint2DPointer new_constraint(new DistancePoint2D(point1,point2,distance));
	AddConstraintEquation(new_constraint);
	return new_constraint;
}

// Add a distance constraint using the current distance
DistancePoint2DPointer Sketch::AddDistancePoint2D(const Point2DPointer point1, const Point2DPointer point2)
{
	DistancePoint2DPointer new_constraint(new DistancePoint2D(point1,point2,1.0));  // using a temp distance of 1.0, will be replated by current distance next
	new_constraint->SetValue(new_constraint->GetActualDistance());
	AddConstraintEquation(new_constraint);
	return new_constraint;
}

ParallelLine2DPointer Sketch::AddParallelLine2D(const Line2DPointer line1, const Line2DPointer line2)
{
	ParallelLine2DPointer new_constraint (new ParallelLine2D(line1, line2));
	AddConstraintEquation(new_constraint);
	return new_constraint;
}

AngleLine2DPointer Sketch::AddAngleLine2D(const Line2DPointer line1, const Line2DPointer line2, double angle, bool interior_angle)
{
	AngleLine2DPointer new_constraint(new AngleLine2D(line1,line2,angle,interior_angle));
	AddConstraintEquation(new_constraint);
	return new_constraint;
}

TangentEdge2DPointer Sketch::AddTangentEdge2D(Edge2DBasePointer edge1, EdgePointNumber point_num_1, Edge2DBasePointer edge2, EdgePointNumber point_num_2)
{
	TangentEdge2DPointer new_constraint(new TangentEdge2D(edge1, point_num_1, edge2, point_num_2));
	AddConstraintEquation(new_constraint);
	return new_constraint;
}
