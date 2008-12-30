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

#ifndef DistancePoint2DH
#define DistancePoint2DH

#include "ConstraintEquationBase.h"
#include "Point2D.h"

class DistancePoint2D : public ConstraintEquationBase
{
	public:
		DistancePoint2D(const Point2DPointer point1, const Point2DPointer point2, double distance);
		double GetActualDistance();
		double GetValue()const {return distance_->GetValue();}
		void SetValue(double value) {distance_->SetValue(value);}
		void SetTextLocation(double text_position, double text_offset) {text_position_ = text_position; text_offset_ = text_offset;}
		void SetSTTextLocation(double s, double t);
		void SetDefaultTextLocation();
		double GetTextPosition() {return text_position_;}	
		double GetTextOffset() {return text_offset_;}

		Point2DPointer GetPoint1() const {return point1_;}
		Point2DPointer GetPoint2() const {return point2_;}
	protected:
		Point2DPointer point1_;
		Point2DPointer point2_;
		DOFPointer distance_;

		double text_position_;
		double text_offset_;
};
typedef boost::shared_ptr<DistancePoint2D> DistancePoint2DPointer;


#endif //DistancePoint2DH
