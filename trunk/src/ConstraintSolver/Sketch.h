#ifndef SketchH
#define SketchH

#include "Ark3DModel.h"

class Sketch : public Ark3DModel
{
	public:
		// constructor
		Sketch(VectorPointer normal, VectorPointer up, PointPointer base);
	
		// Accessor methods
		SketchPlanePointer GetSketchPlane() {return sketch_plane_;}

		// methods for adding primitives to the sketch
		Point2DPointer AddPoint2D ( double s, double t, bool s_free, bool t_free);
		Arc2DPointer AddArc2D (double s_center, double t_center, double theta_1, double theta_2, double radius, bool s_center_free, bool t_center_free, bool theta_1_free, bool theta_2_free, bool radius_free);
		Arc2DPointer AddArc2D (double s1, double t1, double s2, double t2, double s3, double t3, bool s_center_free, bool t_center_free, bool theta_1_free, bool theta_2_free, bool radius_free);
		Line2DPointer AddLine2D (const Point2DPointer point1, const Point2DPointer point2);
		DistancePoint2DPointer AddDistancePoint2D(const Point2DPointer point1, const Point2DPointer point2, double distance);
		DistancePoint2DPointer AddDistancePoint2D(const Point2DPointer point1, const Point2DPointer point2);
		ParallelLine2DPointer AddParallelLine2D(const Line2DPointer line1, const Line2DPointer line2);
		AngleLine2DPointer AddAngleLine2D(const Line2DPointer line1, const Line2DPointer line2, double angle, bool interior_angle);
		AngleLine2DPointer AddAngleLine2D(const Line2DPointer line1, const Line2DPointer line2, bool interior_angle);
		TangentEdge2DPointer AddTangentEdge2D(Edge2DBasePointer edge1, EdgePointNumber point_num_1, Edge2DBasePointer edge2, EdgePointNumber point_num_2);

	protected:
		SketchPlanePointer sketch_plane_;
};


#endif //SketchH


