#include "Arc2D.h"

#include "IndependentDOF.h"
#include "DependentDOF.h"

using namespace std;
using namespace GiNaC;

// create an arc
Arc2D::Arc2D (double s_center, double t_center, double theta_1, double theta_2, double radius, SketchPlanePointer sketch_plane,
			  bool s_center_free, bool t_center_free, bool theta_1_free, bool theta_2_free, bool radius_free):
s_center_(new IndependentDOF(s_center,s_center_free)),
t_center_(new IndependentDOF(t_center,t_center_free)),
theta_1_(new IndependentDOF(theta_1,theta_1_free)),
theta_2_(new IndependentDOF(theta_2,theta_2_free)),
radius_(new IndependentDOF(radius,radius_free)),
Edge2DBase(sketch_plane)
{
	AddPrimitive(sketch_plane);

	AddDOF(s_center_);
	AddDOF(t_center_);
	AddDOF(theta_1_);
	AddDOF(theta_2_);
	AddDOF(radius_);

	point1_ = GeneratePoint1();
	point2_ = GeneratePoint2();
	center_point_ = GenerateCenterPoint();

	SetDefaultTextLocation();
}

// construct 2D arc from three points on the sketch plane
Arc2D::Arc2D (double s1, double t1, double s2, double t2, double s3, double t3, SketchPlanePointer sketch_plane,
              bool s_center_free, bool t_center_free, bool theta_1_free, bool theta_2_free, bool radius_free):
Edge2DBase(sketch_plane)
{
	double m11 = s1*(t2-t3)-t1*(s2-s3)+(s2*t3-t2*s3);
	double mag1 = s1*s1+t1*t1;
	double mag2 = s2*s2+t2*t2;
	double mag3 = s3*s3+t3*t3;
	double m12 = mag1*(t2-t3)-t1*(mag2-mag3)+(mag2*t3-mag3*t2);
	double m13 = mag1*(s2-s3) - s1*(mag2-mag3) + (mag2*s3-mag3*s2);
	double m14 = mag1*(s2*t3-t2*s3) - s1*(mag2*t3-t2*mag3) + t1*(mag2*s3-s2*mag3);

	// check to insure that the lines are not colinears1
	if(m11 == 0.0)
	{	
		// lines are colinear, throw an exception
		throw PrimitiveException();
	} else {
		double s_center = 0.5*(m12/m11);
		double t_center = -0.5*(m13/m11);
		
		double radius = sqrt(s_center*s_center + t_center*t_center + m14/m11);
	
		double theta_1 = atan2(t1-t_center, s1-s_center);
		double theta_2 = atan2(t2-t_center, s2-s_center);
		double theta_3 = atan2(t3-t_center, s3-s_center);

		if(!((theta_2 <= theta_3 && theta_2 >= theta_1) || (theta_2 <= theta_1 && theta_2 >= theta_3)))
		{
			if(theta_3 < theta_1)
				theta_3 += 2.0*mmcPI;
			else
				theta_1 += 2.0*mmcPI;	
		}

		s_center_.reset(new IndependentDOF(s_center,s_center_free));
		t_center_.reset(new IndependentDOF(t_center,t_center_free));
		theta_1_.reset(new IndependentDOF(theta_1,theta_1_free));
		theta_2_.reset(new IndependentDOF(theta_3,theta_2_free));
		radius_.reset(new IndependentDOF(radius,radius_free));

		AddPrimitive(sketch_plane);
	
		AddDOF(s_center_);
		AddDOF(t_center_);
		AddDOF(theta_1_);
		AddDOF(theta_2_);
		AddDOF(radius_);
	
		point1_ = GeneratePoint1();
		point2_ = GeneratePoint2();
		center_point_ = GenerateCenterPoint();
	
		SetDefaultTextLocation();
	}
}


Arc2D::Arc2D (DOFPointer s_center, DOFPointer t_center, DOFPointer theta_1, DOFPointer theta_2, DOFPointer radius, SketchPlanePointer sketch_plane):
s_center_(s_center),
t_center_(t_center),
theta_1_(theta_1),
theta_2_(theta_2),
radius_(radius),
Edge2DBase(sketch_plane)
{
	AddPrimitive(sketch_plane);

	AddDOF(s_center_);
	AddDOF(t_center_);
	AddDOF(theta_1_);
	AddDOF(theta_2_);
	AddDOF(radius_);

	point1_ = GeneratePoint1();
	point2_ = GeneratePoint2();
	center_point_ = GenerateCenterPoint();

	SetDefaultTextLocation();
}

void Arc2D::Get3DLocations(double & x_center, double & y_center, double & z_center)
{
	sketch_plane_->Get3DLocation(s_center_->GetValue(), t_center_->GetValue(), x_center, y_center, z_center);
}


void Arc2D::ApplySelectionMask(SelectionMask mask)
{
	if(mask == All || mask == Edges || mask == Arcs)
		SetSelectable(true);
	else
		SetSelectable(false);
}


// Return a point that will follow the first endpoint of the arc
// This point will use dependent DOF's to define its location
Point2DPointer Arc2D::GeneratePoint1()
{
	// Create expressions defining s and t coordinates of the first endpoint of the arc
	ex s_1 = s_center_->GetVariable() + radius_->GetVariable()*cos(theta_1_->GetVariable());
	ex t_1 = t_center_->GetVariable() + radius_->GetVariable()*sin(theta_1_->GetVariable());

	// create DOF lists for each DOF
	std::vector <DOFPointer> s_1_dof_list;
	s_1_dof_list.push_back(s_center_);
	s_1_dof_list.push_back(radius_);
	s_1_dof_list.push_back(theta_1_);

	std::vector <DOFPointer> t_1_dof_list;
	t_1_dof_list.push_back(t_center_);
	t_1_dof_list.push_back(radius_);
	t_1_dof_list.push_back(theta_1_);

	// create dependent DOF's based on the above expressions
	DOFPointer s_dof(new DependentDOF(s_1, s_1_dof_list));
	DOFPointer t_dof(new DependentDOF(t_1, t_1_dof_list));

	// create the actual point object
	Point2DPointer result(new Point2D(s_dof, t_dof, sketch_plane_));
	return result;
}

// Return a point that will follow the second endpoint of the arc
// This point will use dependent DOF's to define its location
Point2DPointer Arc2D::GeneratePoint2()
{
	// Create expressions defining s and t coordinates of the second endpoint of the arc
	ex s_2 = s_center_->GetVariable() + radius_->GetVariable()*cos(theta_2_->GetVariable());
	ex t_2 = t_center_->GetVariable() + radius_->GetVariable()*sin(theta_2_->GetVariable());

	// create DOF lists for each DOF
	std::vector <DOFPointer> s_2_dof_list;
	s_2_dof_list.push_back(s_center_);
	s_2_dof_list.push_back(radius_);
	s_2_dof_list.push_back(theta_2_);

	std::vector <DOFPointer> t_2_dof_list;
	t_2_dof_list.push_back(t_center_);
	t_2_dof_list.push_back(radius_);
	t_2_dof_list.push_back(theta_2_);

	// create dependent DOF's based on the above expressions
	DOFPointer s_dof(new DependentDOF(s_2, s_2_dof_list));
	DOFPointer t_dof(new DependentDOF(t_2, t_2_dof_list));

	// create the actual point object
	Point2DPointer result(new Point2D(s_dof, t_dof, sketch_plane_));
	return result;
}

// Returns a point which will follow the center point of the arc
Point2DPointer Arc2D::GenerateCenterPoint()
{
	Point2DPointer result(new Point2D(s_center_, t_center_, sketch_plane_));
	return result;
}

void Arc2D::GetTangent1(GiNaC::ex & s_component, GiNaC::ex & t_component)
{
	s_component = sin(GetTheta1()->GetVariable());
	t_component = -cos(GetTheta1()->GetVariable());
}

void Arc2D::GetTangent2(GiNaC::ex & s_component, GiNaC::ex & t_component)
{
	s_component = -sin(GetTheta2()->GetVariable());
	t_component = cos(GetTheta2()->GetVariable());
}


void Arc2D::GetTangent1(double & s_component, double & t_component)
{
	s_component = sin(GetTheta1()->GetValue());
	t_component = -cos(GetTheta1()->GetValue());
}

void Arc2D::GetTangent2(double & s_component, double & t_component)
{
	s_component = -sin(GetTheta2()->GetValue());
	t_component = cos(GetTheta2()->GetValue());
}


void Arc2D::SetDefaultTextLocation()
{
	text_angle_ = (theta_1_->GetValue() + theta_2_->GetValue())*0.5;

	text_radius_ = 0.5*radius_->GetValue();
}

void Arc2D::SetSTTextLocation(double text_s, double text_t)
{
	text_radius_ = sqrt((s_center_->GetValue() - text_s)*(s_center_->GetValue() - text_s) + (t_center_->GetValue() - text_t)*(t_center_->GetValue() - text_t));
	text_angle_ = atan2(text_t-t_center_->GetValue(), text_s-s_center_->GetValue());
}