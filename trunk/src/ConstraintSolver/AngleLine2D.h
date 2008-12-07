#ifndef AngleLine2DH
#define AngleLine2DH

#include "ConstraintEquationBase.h"
#include "Line2D.h"

class AngleLine2D : public ConstraintEquationBase
{
	public:
		AngleLine2D(const Line2DPointer line1, const Line2DPointer line2, double angle /* radians */, bool interior_angle);

		void SetTextLocation(double text_radius, double text_angle) {text_radius_ = text_radius; text_angle_ = text_angle;}
		void SetDefaultTextLocation();
		double GetTextRadius() const {return text_radius_;}	
		double GetTextAngle() const {return text_angle_;}

		Line2DPointer GetLine1() const {return line1_;}
		Line2DPointer GetLine2() const {return line2_;}

		bool IsInteriorAngle() const {return interior_angle_;}

		double GetAngleValue() const {return angle_->GetValue();}
		void SetAngleValue(double angle) {angle_->SetValue(angle);}

		double GetActualAngle() const;
		
	protected:
		Line2DPointer line1_;
		Line2DPointer line2_;

		DOFPointer angle_;
		
		bool interior_angle_;

		double text_radius_;
		double text_angle_;

		// the following two parameters are used to position the angle dimension label if line1_ and line2_ happen to be parallel
		double text_s_;
		double text_t_;
};
typedef boost::shared_ptr<AngleLine2D> AngleLine2DPointer;



#endif //AngleLine2DH
