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

#ifndef Edge2DBaseH
#define Edge2DBaseH

#include <vector>

#include "Primitive2DBase.h"
#include "Point2D.h"

// Abstract base class for 2D edges
enum EdgePointNumber {Point1 = 1, Point2 = 2};
class Edge2DBase : public Primitive2DBase
{
	public:
		Edge2DBase(SketchPlanePointer sketch_plane);
		Edge2DBase();
		virtual ~Edge2DBase() {;}
		
		virtual void ApplySelectionMask(SelectionMask mask);

		// virtual methods that must be implemented by child classes
		virtual Point2DPointer GetPoint1() = 0;		// returns end point of edge (these may include dependentDOF's as is the case of the arc primitive)
		virtual Point2DPointer GetPoint2() = 0;
		virtual void GetTangent1(DOFPointer & s_component, DOFPointer & t_component) = 0;  // returns dependent dof's that define the tangent vector for each endpoint of the edge
		virtual void GetTangent2(DOFPointer & s_component, DOFPointer & t_component) = 0;

		virtual void GetTangent1(double & s_component, double & t_component) = 0;
		virtual void GetTangent2(double & s_component, double & t_component) = 0;

		// methods implemented by this base class
		bool IsPointCoincident(EdgePointNumber my_point_number, boost::shared_ptr<Edge2DBase> other_edge, EdgePointNumber other_point_number);
	protected:
};
typedef boost::shared_ptr<Edge2DBase> Edge2DBasePointer;


#endif //Edge2DBaseH
