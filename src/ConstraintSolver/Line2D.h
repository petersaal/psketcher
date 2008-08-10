#ifndef Line2DH
#define Line2DH

#include "Edge2DBase.h"
#include "Point2D.h"

// Line2D class
class Line2D : public Edge2DBase
{
	public:
		Line2D (const Point2DPointer point1, const Point2DPointer point2, SketchPlanePointer sketch_plane);

		DOFPointer GetS1()const {return s1_;}
		DOFPointer GetT1()const {return t1_;}

		DOFPointer GetS2()const {return s2_;}
		DOFPointer GetT2()const {return t2_;}

		void Get3DLocations(double & x1, double & y1, double & z1,
												double & x2, double & y2, double & z2);

		Point2DPointer GetPoint1();
		Point2DPointer GetPoint2();

		void GetTangent1(GiNaC::ex & s_component, GiNaC::ex & t_component);  // returns expression that defines tangent vector for each endpoint of the edge
		void GetTangent2(GiNaC::ex & s_component, GiNaC::ex & t_component);

		void GetTangent1(double & s_component, double & t_component);
		void GetTangent2(double & s_component, double & t_component);

		void ApplySelectionMask(SelectionMask mask);

	private:
		DOFPointer s1_;
		DOFPointer t1_;

		DOFPointer s2_;
		DOFPointer t2_;
};
typedef boost::shared_ptr<Line2D> Line2DPointer;


#endif //Line2DH
