#ifndef TangentEdge2DH
#define TangentEdge2DH

#include "ConstraintEquationBase.h"
#include "Edge2DBase.h"

class TangentEdge2D : public ConstraintEquationBase
{
	public:
		TangentEdge2D(Edge2DBasePointer edge1, EdgePointNumber point_num_1, Edge2DBasePointer edge2, EdgePointNumber point_num_2);

	protected:
		Edge2DBasePointer edge1_;
		Edge2DBasePointer edge2_;
};
typedef boost::shared_ptr<TangentEdge2D> TangentEdge2DPointer;

#endif //TangentEdge2DH
