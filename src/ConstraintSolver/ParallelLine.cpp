#include "ParallelLine.h"

using namespace std;
using namespace GiNaC;

// Create a parallelism constrain between two lines
ParallelLine::ParallelLine(const LinePointer line1, const LinePointer line2)
{
	AddPrimitive(line1);
	AddPrimitive(line2);

	// create the expression that defines the parallel constraint and add it the the constraint list
	boost::shared_ptr<ex> new_constraint(new ex);
	
	ex line1_dx = line1->GetX1()->GetVariable() - line1->GetX2()->GetVariable();
	ex line1_dy = line1->GetY1()->GetVariable() - line1->GetY2()->GetVariable();
	ex line1_dz = line1->GetZ1()->GetVariable() - line1->GetZ2()->GetVariable();
	ex line1_length = sqrt(pow(line1_dx,2)+pow(line1_dy,2)+pow(line1_dz,2));

	ex line2_dx = line2->GetX1()->GetVariable() - line2->GetX2()->GetVariable();
	ex line2_dy = line2->GetY1()->GetVariable() - line2->GetY2()->GetVariable();
	ex line2_dz = line2->GetZ1()->GetVariable() - line2->GetZ2()->GetVariable();
	ex line2_length = sqrt(pow(line2_dx,2)+pow(line2_dy,2)+pow(line2_dz,2));

	// Calculate the dot product normalized by the vector lengths and subtract one
	// this expression will be zero when the lines are parallel
	// Ideally, I would use abs() instead of pow() but abs is not differentiable. 
	// @todo find a better function besides pow to use for the parallel constraint
	*new_constraint = pow((1/(line1_length*line2_length))*(line1_dx*line2_dx + line1_dy*line2_dy + line1_dz*line2_dz),2)-1;

	constraints_.push_back(new_constraint);
	weight_list_.push_back(1.0);
}
