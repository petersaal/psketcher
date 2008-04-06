#ifndef DistancePointH
#define DistancePointH

#include "ConstraintEquationBase.h"
#include "Point.h"

class DistancePoint : public ConstraintEquationBase
{
	public:
		DistancePoint(const PointPointer point1, const PointPointer point2, double distance);

	private:
};
typedef boost::shared_ptr<DistancePoint> DistancePointPointer;


#endif //DistancePointH
