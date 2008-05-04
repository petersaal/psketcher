#ifndef DistancePoint2DH
#define DistancePoint2DH

#include "ConstraintEquationBase.h"
#include "Point2D.h"

class DistancePoint2D : public ConstraintEquationBase
{
	public:
		DistancePoint2D(const Point2DPointer point1, const Point2DPointer point2, double distance);
		double GetActualDistance();
		void SetValue(double value) {distance_->SetValue(value);}

	protected:
		Point2DPointer point1_;
		Point2DPointer point2_;
		DOFPointer distance_;
};
typedef boost::shared_ptr<DistancePoint2D> DistancePoint2DPointer;


#endif //DistancePoint2DH
