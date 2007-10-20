#include "OccSketch.h"

OccSketch::OccSketch(Handle(AIS_InteractiveContext) ais_context, VectorPointer normal, VectorPointer up, PointPointer base):
Sketch(normal,up,base),
ais_context_(ais_context)
{

}

OccPoint2DPointer OccSketch::AddPoint2D ( double s, double t, bool s_free, bool t_free)
{
	OccPoint2DPointer new_point(new OccPoint2D(ais_context_,s,t,GetSketchPlane(),s_free,t_free));
	AddPrimitive(new_point);
	return new_point;
}


OccArc2DPointer OccSketch::AddArc2D (double s_center, double t_center, double theta_1, double theta_2, double radius, bool s_center_free, bool t_center_free, bool theta_1_free, bool theta_2_free, bool radius_free)
{
	OccArc2DPointer new_arc(new OccArc2D(ais_context_,s_center, t_center, theta_1, theta_2, radius, GetSketchPlane(),s_center_free, t_center_free, theta_1_free, theta_2_free, radius_free));
	AddPrimitive(new_arc);
	return new_arc;
}


OccLine2DPointer OccSketch::AddLine2D (const Point2DPointer point1, const Point2DPointer point2)
{
	OccLine2DPointer new_line(new OccLine2D(ais_context_,point1, point2, GetSketchPlane()));
	AddPrimitive(new_line);
	return new_line;
}


OccDistancePoint2DPointer OccSketch::AddDistancePoint2D(const Point2DPointer point1, const Point2DPointer point2, double distance)
{
	OccDistancePoint2DPointer new_constraint(new OccDistancePoint2D(ais_context_,point1,point2,distance));
	AddConstraintEquation(new_constraint);
	return new_constraint;
}

OccParallelLine2DPointer OccSketch::AddParallelLine2D(const Line2DPointer line1, const Line2DPointer line2)
{
	OccParallelLine2DPointer new_constraint (new OccParallelLine2D(ais_context_,line1, line2));
	AddConstraintEquation(new_constraint);
	return new_constraint;
}

OccAngleLine2DPointer OccSketch::AddAngleLine2D(const Line2DPointer line1, const Line2DPointer line2, double angle)
{
	OccAngleLine2DPointer new_constraint(new OccAngleLine2D(ais_context_,line1,line2,angle));
	AddConstraintEquation(new_constraint);
	return new_constraint;
}

OccTangentEdge2DPointer OccSketch::AddTangentEdge2D(Edge2DBasePointer edge1, EdgePointNumber point_num_1, Edge2DBasePointer edge2, EdgePointNumber point_num_2)
{
	OccTangentEdge2DPointer new_constraint(new OccTangentEdge2D(ais_context_,edge1, point_num_1, edge2, point_num_2));
	AddConstraintEquation(new_constraint);
	return new_constraint;
}
