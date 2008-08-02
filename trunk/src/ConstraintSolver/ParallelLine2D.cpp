#include "ParallelLine2D.h"

using namespace std;
using namespace GiNaC;

// Create a parallelism constrain between two lines
ParallelLine2D::ParallelLine2D(const Line2DPointer line1, const Line2DPointer line2):
line1_(line1),
line2_(line2),
marker_position_(0.5)   // by default place marker at the middle of the constrained lines
{
	AddPrimitive(line1);
	AddPrimitive(line2);

	// create the expression that defines the parallel constraint and add it the the constraint list
	boost::shared_ptr<ex> new_constraint(new ex);
	
	ex line1_ds = line1->GetS1()->GetVariable() - line1->GetS2()->GetVariable();
	ex line1_dt = line1->GetT1()->GetVariable() - line1->GetT2()->GetVariable();
	ex line1_length = sqrt(pow(line1_ds,2)+pow(line1_dt,2));

	ex line2_ds = line2->GetS1()->GetVariable() - line2->GetS2()->GetVariable();
	ex line2_dt = line2->GetT1()->GetVariable() - line2->GetT2()->GetVariable();
	ex line2_length = sqrt(pow(line2_ds,2)+pow(line2_dt,2));

	// Calculate the dot product normalized by the vector lengths and subtract one
	// this expression will be zero when the lines are parallel
	// Ideally, I would use abs() instead of pow() but abs is not differentiable. 
	*new_constraint = pow((1/(line1_length*line2_length))*(line1_ds*line2_ds + line1_dt*line2_dt),2)-1;

	constraints_.push_back(new_constraint);
	weight_list_.push_back(1.0);
}


