#ifndef DistancePoint2DH
#define DistancePoint2DH

#include "ConstraintEquationBase.h"
#include "Point2D.h"

class DistancePoint2D : public ConstraintEquationBase
{
	public:
		DistancePoint2D(const Point2DPointer point1, const Point2DPointer point2, double distance);
		double GetActualDistance();
		double GetValue()const {return distance_->GetValue();}
		void SetValue(double value) {distance_->SetValue(value);}
		void SetTextLocation(double text_s, double text_t) {text_s_ = text_s; text_t_ = text_t;}
		void SetDefaultTextLocation();
		double GetTextS() {return text_s_;}	
		double GetTextT() {return text_t_;}

		Point2DPointer GetPoint1() const {return point1_;}
		Point2DPointer GetPoint2() const {return point2_;}
	protected:
		Point2DPointer point1_;
		Point2DPointer point2_;
		DOFPointer distance_;

		double text_s_;
		double text_t_;
};
typedef boost::shared_ptr<DistancePoint2D> DistancePoint2DPointer;


#endif //DistancePoint2DH
