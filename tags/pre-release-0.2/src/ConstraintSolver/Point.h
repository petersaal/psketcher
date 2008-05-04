#ifndef PointH
#define PointH

#include "PrimitiveBase.h"

// point class
class Point : public PrimitiveBase
{
	public:
		Point ( double x, double y, double z, bool x_free = false, bool y_free = false, bool z_free = false);
		//Point ( DOFPointer x, DOFPointer y, DOFPointer z );

		void ApplySelectionMask(SelectionMask mask);

		DOFPointer GetXDOF()const {return x_;}
		DOFPointer GetYDOF()const {return y_;}
		DOFPointer GetZDOF()const {return z_;}

		mmcMatrix GetmmcMatrix();  // returns mmcMatrix vector containing current location 

	private:
		DOFPointer x_;
		DOFPointer y_;
		DOFPointer z_;
};
typedef boost::shared_ptr<Point> PointPointer;


#endif //PointH
