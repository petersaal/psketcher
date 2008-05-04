#ifndef Primitive2DBaseH
#define Primitive2DBaseH

#include "PrimitiveBase.h"
#include "SketchPlane.h"

// Base class for points constrained to a sketch plane
class Primitive2DBase : public PrimitiveBase
{
	public:
		Primitive2DBase(SketchPlanePointer sketch_plane) {sketch_plane_ = sketch_plane;}
		SketchPlanePointer GetSketchPlane() { return sketch_plane_;}

	protected:
		SketchPlanePointer sketch_plane_;
};
typedef boost::shared_ptr<Primitive2DBase> Primitive2DBasePointer;


#endif //Primitive2DBaseH
