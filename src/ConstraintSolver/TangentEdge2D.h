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

#ifndef TangentEdge2DH
#define TangentEdge2DH

#include "ConstraintEquationBase.h"
#include "Edge2DBase.h"

class TangentEdge2D : public ConstraintEquationBase
{
	public:
		TangentEdge2D(Edge2DBasePointer edge1, EdgePointNumber point_num_1, Edge2DBasePointer edge2, EdgePointNumber point_num_2);

		// Accessor methods
		Edge2DBasePointer GetEdge1() const {return edge1_;}
		Edge2DBasePointer GetEdge2() const {return edge2_;}

		EdgePointNumber GetPointNum1() const {return point_num_1_;}
		EdgePointNumber GetPointNum2() const {return point_num_2_;}

	protected:
		Edge2DBasePointer edge1_;
		Edge2DBasePointer edge2_;

		EdgePointNumber point_num_1_;
		EdgePointNumber point_num_2_;
};
typedef boost::shared_ptr<TangentEdge2D> TangentEdge2DPointer;

#endif //TangentEdge2DH
