#ifndef SketchPlaneH
#define SketchPlaneH

#include "PrimitiveBase.h"
#include "Vector.h"
#include "Point.h"

// sketch plane class (includes up vector)
class SketchPlane : public PrimitiveBase
{
	public:
		SketchPlane ( VectorPointer normal, VectorPointer up, PointPointer base);
		void Get3DLocation ( double s, double t, double & x, double & y, double & z);
		double GetSTLocation( double x, double y, double z, double &s, double &t);
		void GetABCD ( double & coef_a, double & coef_b, double & coef_c, double & coef_d);
		
		void ApplySelectionMask(SelectionMask mask);

		VectorPointer GetNormal() {return normal_;}
		VectorPointer GetUp() {return up_;}
		PointPointer GetBase() {return base_;}
	private:
		VectorPointer normal_;
		VectorPointer up_;
		PointPointer base_;
};
typedef boost::shared_ptr<SketchPlane> SketchPlanePointer;


#endif //SketchPlaneH
