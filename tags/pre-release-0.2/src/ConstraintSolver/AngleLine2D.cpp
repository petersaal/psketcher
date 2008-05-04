#include "AngleLine2D.h"

#include "IndependentDOF.h"

using namespace std;
using namespace GiNaC;

// Create an angle constraint between two lines
AngleLine2D::AngleLine2D(const Line2DPointer line1, const Line2DPointer line2, double angle /* radians */):
line1_(line1),
line2_(line2)
{
	// Create a DOF for the angle parameter
	DOFPointer new_dof(new IndependentDOF(angle,false));
	angle_ = new_dof;	

	dof_list_.push_back(angle_);

	// create the expression that defines the parallel constraint and add it the the constraint list
	boost::shared_ptr<ex> new_constraint(new ex);
	
	ex line1_ds = line1->GetS1()->GetVariable() - line1->GetS2()->GetVariable();
	ex line1_dt = line1->GetT1()->GetVariable() - line1->GetT2()->GetVariable();
	ex line1_length = sqrt(pow(line1_ds,2)+pow(line1_dt,2));

	ex line2_ds = line2->GetS1()->GetVariable() - line2->GetS2()->GetVariable();
	ex line2_dt = line2->GetT1()->GetVariable() - line2->GetT2()->GetVariable();
	ex line2_length = sqrt(pow(line2_ds,2)+pow(line2_dt,2));

	ex desired_angle = angle_->GetVariable();

	// Calculate the dot product normalized by the vector lengths and subtract the cos of the desired angle
	// this expression will be zero when the lines are at the desired angle
	*new_constraint = (1/(line1_length*line2_length))*(line1_ds*line2_ds + line1_dt*line2_dt)-cos(mmcPI-desired_angle);

	constraints_.push_back(new_constraint);
	weight_list_.push_back(1.0);
}
