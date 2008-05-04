#ifndef AngleLine2DH
#define AngleLine2DH

#include "ConstraintEquationBase.h"
#include "Line2D.h"

class AngleLine2D : public ConstraintEquationBase
{
	public:
		AngleLine2D(const Line2DPointer line1, const Line2DPointer line2, double angle /* radians */);

	protected:
		Line2DPointer line1_;
		Line2DPointer line2_;

		DOFPointer angle_;
};
typedef boost::shared_ptr<AngleLine2D> AngleLine2DPointer;



#endif //AngleLine2DH
