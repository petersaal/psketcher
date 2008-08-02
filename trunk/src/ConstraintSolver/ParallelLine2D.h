#ifndef ParallelLine2DH
#define ParallelLine2DH

#include "ConstraintEquationBase.h"
#include "Line2D.h"

class ParallelLine2D : public ConstraintEquationBase
{
	public:
		ParallelLine2D(const Line2DPointer line1, const Line2DPointer line2);

		double GetMarkerPosition() const {return marker_position_;}
		void SetMarkerPosition(const double marker_position) {marker_position_ = marker_position;} // @fixme need to maker sure that the new marker position is in the set [0.0,1.0]

		Line2DPointer GetLine1() const {return line1_;}
		Line2DPointer GetLine2() const {return line2_;}
	
	protected:
		Line2DPointer line1_;
		Line2DPointer line2_;

		double marker_position_; // value from 0.0 to 1.0 that determines where the parallel marker will be displayed along each constrained line
};
typedef boost::shared_ptr<ParallelLine2D> ParallelLine2DPointer;


#endif //ParallelLine2DH
