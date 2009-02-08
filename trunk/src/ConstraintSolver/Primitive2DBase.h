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

#ifndef Primitive2DBaseH
#define Primitive2DBaseH

#include "PrimitiveBase.h"
#include "SketchPlane.h"

// Base class for points constrained to a sketch plane
class Primitive2DBase : public PrimitiveBase
{
	public:
		Primitive2DBase(SketchPlanePointer sketch_plane) {sketch_plane_ = sketch_plane;}
		Primitive2DBase() {;}
		SketchPlanePointer GetSketchPlane() { return sketch_plane_;}
		void SetSketchPlane(SketchPlanePointer sketch_plane) {sketch_plane_ = sketch_plane;}

	protected:
		SketchPlanePointer sketch_plane_;
};
typedef boost::shared_ptr<Primitive2DBase> Primitive2DBasePointer;


#endif //Primitive2DBaseH
