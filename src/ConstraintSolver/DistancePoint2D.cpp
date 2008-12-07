#include "DistancePoint2D.h"

#include "IndependentDOF.h"

using namespace std;
using namespace GiNaC;

// Create a constraint that defines the distance between two points confined to a sketch plane
DistancePoint2D::DistancePoint2D(const Point2DPointer point1, const Point2DPointer point2, double distance):
point1_(point1),
point2_(point2)
{
	AddPrimitive(point1);
	AddPrimitive(point2);

	// Create a DOF for the distance parameter
	DOFPointer new_dof(new IndependentDOF(distance,false));
	distance_ = new_dof;

	AddDOF(distance_);

	// create the expression that defines the constraint and add it the the constraint list
	boost::shared_ptr<ex> new_constraint(new ex);
	
	*new_constraint = sqrt( pow(point1->GetSDOF()->GetVariable() - point2->GetSDOF()->GetVariable(),2) +
													pow(point1->GetTDOF()->GetVariable() - point2->GetTDOF()->GetVariable(),2))
										- distance_->GetVariable();

	cout << *new_constraint << endl;

	constraints_.push_back(new_constraint);
	weight_list_.push_back(1.0);

	// text location was not specified so provide a reasonable default
	SetDefaultTextLocation();
}

// Calculate the current actual distance between the two points
// This method is used to set the distance when graphically generating a distance constraint
double DistancePoint2D::GetActualDistance()
{
		// calculate the distance between point1_ and point2_
		double term1 = (point1_->GetSDOF()->GetValue() - point2_->GetSDOF()->GetValue());
		term1 = term1*term1;
		double term2 = (point1_->GetTDOF()->GetValue() - point2_->GetTDOF()->GetValue());
		term2 = term2*term2;

		return sqrt(term1+term2);
}


void DistancePoint2D::SetDefaultTextLocation()
{
	text_position_ = 0.5*GetActualDistance();
	text_offset_   = 0.25*GetActualDistance();
}

void DistancePoint2D::SetSTTextLocation(double s, double t)
{
	mmcMatrix point1 = point1_->GetmmcMatrix();
	mmcMatrix point2 = point2_->GetmmcMatrix();

	mmcMatrix tangent = (point2-point1);
	double tangent_magnitude = tangent.GetMagnitude();
	if (tangent_magnitude > 0.0)
	{
		tangent = tangent.GetScaled(1.0/tangent_magnitude);
	} else {
		// tangent vector has zero length, define an arbitrary tangent vector to avoid divide by zero
		tangent(0,0) = 1.0;
		tangent(1,0) = 0.0;	
	}

	mmcMatrix normal(2,1);
	normal(0,0) = -tangent(1,0);
	normal(1,0) = tangent(0,0);

	mmcMatrix text_location(2,1);
	text_location(0,0) = s;
	text_location(1,0) = t;

	mmcMatrix inverse(2,2);
	inverse(0,0) = tangent(1,0);
	inverse(0,1) = -tangent(0,0);
	inverse(1,0) = -normal(1,0);
	inverse(1,1) = normal(0,0);
	inverse = (1.0/(normal(0,0)*tangent(1,0) - tangent(0,0)*normal(1,0)))*inverse;

	mmcMatrix solution = inverse*(text_location - point1);
	text_offset_ = solution(0,0);
	text_position_ = solution(1,0);
}

