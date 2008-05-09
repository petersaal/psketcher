#ifndef QtSketchH
#define QtSketchH

#include "../ConstraintSolver/Sketch.h"
#include "QtPrimitives.h"

class QtSketch : public Sketch
{
	public:
		// constructor
		QtSketch(VectorPointer normal, VectorPointer up, PointPointer base, bool grid_snap = false);

		// accessor methods
		bool GetGridSnap() {return grid_snap_;}
		void SetGridSnap(bool grid_snap) {grid_snap_ = grid_snap;}

		// @fixme implement ClearSelected() method
		void ClearSelected() {;}

		// methods for adding primitives to the sketch
		QtPoint2DPointer AddPoint2D ( double s, double t, bool s_free, bool t_free);
		QtArc2DPointer AddArc2D (double s_center, double t_center, double theta_1, double theta_2, double radius, bool s_center_free, bool t_center_free, bool theta_1_free, bool theta_2_free, bool radius_free);
		QtLine2DPointer AddLine2D (const Point2DPointer point1, const Point2DPointer point2);
		QtDistancePoint2DPointer AddDistancePoint2D(const Point2DPointer point1, const Point2DPointer point2, double distance);
		QtDistancePoint2DPointer AddDistancePoint2D(const Point2DPointer point1, const Point2DPointer point2);
		QtParallelLine2DPointer AddParallelLine2D(const Line2DPointer line1, const Line2DPointer line2);
		QtAngleLine2DPointer AddAngleLine2D(const Line2DPointer line1, const Line2DPointer line2, double angle);
		QtTangentEdge2DPointer AddTangentEdge2D(Edge2DBasePointer edge1, EdgePointNumber point_num_1, Edge2DBasePointer edge2, EdgePointNumber point_num_2);

	private:
		bool grid_snap_;
};
typedef boost::shared_ptr<QtSketch> QtSketchPointer;


#endif //QtSketchH
