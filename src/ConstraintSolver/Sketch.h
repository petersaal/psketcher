/****************************************************************************
**
** This file is part of the pSketcher project.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Copyright (C) 2006-2008 Michael Greminger. All rights reserved.
**
****************************************************************************/

#ifndef SketchH
#define SketchH

#include "Ark3DModel.h"

class Sketch : public Ark3DModel
{
	public:
		// constructor
		Sketch(VectorPointer normal, VectorPointer up, PointPointer base,PrimitiveBasePointer (*current_primitive_factory)(unsigned, Ark3DModel &) = Ark3DModel::PrimitiveFactory, ConstraintEquationBasePointer (*current_constraint_factory)(unsigned, Ark3DModel &) = Ark3DModel::ConstraintFactory);

		Sketch(const std::string &file_name,PrimitiveBasePointer (*current_primitive_factory)(unsigned, Ark3DModel &) = Ark3DModel::PrimitiveFactory, ConstraintEquationBasePointer (*current_constraint_factory)(unsigned, Ark3DModel &) = Ark3DModel::ConstraintFactory);
	
		void AddToDatabase();

		// Accessor methods
		SketchPlanePointer GetSketchPlane() {return sketch_plane_;}

		// methods for adding primitives to the sketch
		Point2DPointer AddPoint2D ( double s, double t, bool s_free, bool t_free);
		Arc2DPointer AddArc2D (double s_center, double t_center, double theta_1, double theta_2, double radius, bool s_center_free, bool t_center_free, bool theta_1_free, bool theta_2_free, bool radius_free);
		Arc2DPointer AddArc2D (double s1, double t1, double s2, double t2, double s3, double t3, bool s_center_free, bool t_center_free, bool theta_1_free, bool theta_2_free, bool radius_free);
		Line2DPointer AddLine2D (const Point2DPointer point1, const Point2DPointer point2);
		DistancePoint2DPointer AddDistancePoint2D(const Point2DPointer point1, const Point2DPointer point2, double distance);
		DistancePoint2DPointer AddDistancePoint2D(const Point2DPointer point1, const Point2DPointer point2);
        DistancePointLine2DPointer AddDistancePointLine2D(const Point2DPointer point, const Line2DPointer line);
		ParallelLine2DPointer AddParallelLine2D(const Line2DPointer line1, const Line2DPointer line2);
		HoriVertLine2DPointer AddHoriVertLine2D(const Line2DPointer line, bool vertical_constraint);
		AngleLine2DPointer AddAngleLine2D(const Line2DPointer line1, const Line2DPointer line2, double angle, bool interior_angle);
		AngleLine2DPointer AddAngleLine2D(const Line2DPointer line1, const Line2DPointer line2, bool interior_angle);
		TangentEdge2DPointer AddTangentEdge2D(Edge2DBasePointer edge1, EdgePointNumber point_num_1, Edge2DBasePointer edge2, EdgePointNumber point_num_2);

	protected:
		SketchPlanePointer sketch_plane_;
};


#endif //SketchH


