#ifndef VectorH
#define VectorH

#include "PrimitiveBase.h"

// vector class
class Vector : public PrimitiveBase
{
	public:
		Vector ( double x, double y, double z, bool x_free = false, bool y_free = false, bool z_free = false);
		//Vector ( DOFPointer x, DOFPointer y, DOFPointer z );
		mmcMatrix GetmmcMatrix();  // returns mmcMatrix vector containing current location 

		void ApplySelectionMask(SelectionMask mask);

	private:
		DOFPointer x_;
		DOFPointer y_;
		DOFPointer z_;
};
typedef boost::shared_ptr<Vector> VectorPointer;



#endif //VectorH
