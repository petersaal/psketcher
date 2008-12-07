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
		void SetTextLocation(double text_position, double text_offset) {text_position_ = text_position; text_offset_ = text_offset;}
		void SetSTTextLocation(double s, double t);
		void SetDefaultTextLocation();
		double GetTextPosition() {return text_position_;}	
		double GetTextOffset() {return text_offset_;}

		Point2DPointer GetPoint1() const {return point1_;}
		Point2DPointer GetPoint2() const {return point2_;}
	protected:
		Point2DPointer point1_;
		Point2DPointer point2_;
		DOFPointer distance_;

		double text_position_;
		double text_offset_;
};
typedef boost::shared_ptr<DistancePoint2D> DistancePoint2DPointer;


#endif //DistancePoint2DH
