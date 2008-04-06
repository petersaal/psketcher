#ifndef ParallelLine2DH
#define ParallelLine2DH

#include "ConstraintEquationBase.h"
#include "Line2D.h"

class ParallelLine2D : public ConstraintEquationBase
{
	public:
		ParallelLine2D(const Line2DPointer line1, const Line2DPointer line2);

	protected:
		Line2DPointer line1_;
		Line2DPointer line2_;

};
typedef boost::shared_ptr<ParallelLine2D> ParallelLine2DPointer;


#endif //ParallelLine2DH
