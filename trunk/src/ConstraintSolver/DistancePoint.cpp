#include "DistancePoint.h"
#include "IndependentDOF.h"

using namespace std;
using namespace GiNaC;

// Create a constraint that defines the distance between two points
DistancePoint::DistancePoint(const PointPointer point1, const PointPointer point2, double distance)
{
	// Create a DOF for the distance parameter
	DOFPointer new_dof(new IndependentDOF(distance,false));
	
	dof_list_.push_back(new_dof);

	// create the expression that defines the constraint and add it the the constraint list
	boost::shared_ptr<ex> new_constraint(new ex);
	
	*new_constraint = sqrt( pow(point1->GetXDOF()->GetVariable() - point2->GetXDOF()->GetVariable(),2) +
													pow(point1->GetYDOF()->GetVariable() - point2->GetYDOF()->GetVariable(),2) +
													pow(point1->GetZDOF()->GetVariable() - point2->GetZDOF()->GetVariable(),2))
										- new_dof->GetVariable();

	constraints_.push_back(new_constraint);
	weight_list_.push_back(1.0);
}
