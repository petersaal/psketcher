#ifndef Arc2DH
#define Arc2DH

#include "Edge2DBase.h"

// Line2D class
class Arc2D : public Edge2DBase
{
	public:
		Arc2D (double s_center, double t_center, double theta_1, double theta_2, double radius, SketchPlanePointer sketch_plane,
               bool s_center_free = false, bool t_center_free = false, bool theta_1_free = false, bool theta_2_free = false, bool radius_free = false);
		Arc2D (DOFPointer s_center, DOFPointer t_center, DOFPointer theta_1, DOFPointer theta_2, DOFPointer radius, SketchPlanePointer sketch_plane);

		DOFPointer GetSCenter()const {return s_center_;}
		DOFPointer GetTCenter()const {return t_center_;}

		double GetSCenterValue()const {return s_center_->GetValue();}
		double GetTCenterValue()const {return t_center_->GetValue();}

		DOFPointer GetTheta1()const {return theta_1_;}
		DOFPointer GetTheta2()const {return theta_2_;}

		double GetTheta1Value()const {return theta_1_->GetValue();}
		double GetTheta2Value()const {return theta_2_->GetValue();}

		DOFPointer GetRadius()const {return radius_;}
		double GetRadiusValue()const {return radius_->GetValue();}
		void SetRadiusValue(double radius) {radius_->SetValue(radius);}

		void Get3DLocations(double & x_center, double & y_center, double & z_center);

		Point2DPointer GetPoint1(){return point1_;}
		Point2DPointer GetPoint2(){return point2_;}

		Point2DPointer GeneratePoint1();
		Point2DPointer GeneratePoint2();

		void GetTangent1(GiNaC::ex & s_component, GiNaC::ex & t_component);  // returns expression that defines tangent vector for each endpoint of the edge
		void GetTangent2(GiNaC::ex & s_component, GiNaC::ex & t_component);

		void ApplySelectionMask(SelectionMask mask);

		void SetTextLocation(double text_radius, double text_angle) {text_radius_ = text_radius; text_angle_ = text_angle;}
		void SetDefaultTextLocation();
		double GetTextRadius() {return text_radius_;}	
		double GetTextAngle() {return text_angle_;}
	
	protected:
		// parameters that define the arc
		DOFPointer s_center_;
		DOFPointer t_center_;

		DOFPointer theta_1_;	// start angle
		DOFPointer theta_2_;	// end angle

		DOFPointer radius_;

		// Points based on dependent DOF's that provide the location of the arc endpoints
		// It is important that these points be maintained here so that Edge2DCBase point coincident checks can be performed by comparing DOF pointers
		// If these points are generated for each call to GetPoint1() then, for every call to GetPoint*(), the pointers to the S and T DOF's will be unique
		Point2DPointer point1_;
		Point2DPointer point2_;

		double text_radius_;
		double text_angle_;
};
typedef boost::shared_ptr<Arc2D> Arc2DPointer;



#endif //Arc2DH
