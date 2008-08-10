#ifndef Edge2DBaseH
#define Edge2DBaseH

#include "Primitive2DBase.h"
#include "Point2D.h"

// Abstract base class for 2D edges
enum EdgePointNumber {Point1, Point2};
class Edge2DBase : public Primitive2DBase
{
	public:
		Edge2DBase(SketchPlanePointer sketch_plane);
		virtual ~Edge2DBase() {;}
		
		virtual void ApplySelectionMask(SelectionMask mask);

		// virtual methods that must be implemented by child classes
		virtual Point2DPointer GetPoint1() = 0;		// returns end point of edge (these may include dependentDOF's as is the case of the arc primitive)
		virtual Point2DPointer GetPoint2() = 0;
		virtual void GetTangent1(GiNaC::ex & s_component, GiNaC::ex & t_component) = 0;  // returns expression that defines tangent vector for each endpoint of the edge
		virtual void GetTangent2(GiNaC::ex & s_component, GiNaC::ex & t_component) = 0;

		virtual void GetTangent1(double & s_component, double & t_component) = 0;
		virtual void GetTangent2(double & s_component, double & t_component) = 0;

		// methods implemented by this base class
		bool IsPointCoincident(EdgePointNumber my_point_number, boost::shared_ptr<Edge2DBase> other_edge, EdgePointNumber other_point_number);
	protected:
};
typedef boost::shared_ptr<Edge2DBase> Edge2DBasePointer;


#endif //Edge2DBaseH
