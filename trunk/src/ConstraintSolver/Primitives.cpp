#include "Primitives.h"

using namespace std;
using namespace GiNaC;

DOF :: DOF(double value, bool free)
{
	free_ = free;
	value_ = value;

}

DOF :: DOF(const char *name, double value, bool free)
{
	free_ = free;
	variable_.set_name(name);
	value_ = value;
}


Point :: Point ( double x, double y, double z, bool x_free, bool y_free, bool z_free):
x_(new DOF(x,x_free)),
y_(new DOF(y,y_free)),
z_(new DOF(z,z_free))
{
	dof_list_.push_back(x_);
	dof_list_.push_back(y_);
	dof_list_.push_back(z_);
}

Line :: Line(const PointPointer point1, const PointPointer point2)
{
	x1_ = point1->GetXDOF();
	y1_ = point1->GetYDOF();
	z1_ = point1->GetZDOF();

	x2_ = point2->GetXDOF();
	y2_ = point2->GetYDOF();
	z2_ = point2->GetZDOF();

	dof_list_.push_back(x1_);
	dof_list_.push_back(y1_);
	dof_list_.push_back(z1_);

	dof_list_.push_back(x2_);
	dof_list_.push_back(y2_);
	dof_list_.push_back(z2_);
}

// Create a constraint that defines the distance between two points
DistanceConstraint::DistanceConstraint(const PointPointer point1, const PointPointer point2, double distance)
{
	// Create a DOF for the distance parameter
	DOFPointer new_dof(new DOF(distance,false));
	
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

// Create a parallelism constrain between two lines
ParallelConstraint::ParallelConstraint(const LinePointer line1, const LinePointer line2)
{
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
	*new_constraint = (1/(line1_length*line2_length))*(line1_dx*line2_dx + line1_dy*line2_dy + line1_dz*line2_dz)-1;

	constraints_.push_back(new_constraint);
	weight_list_.push_back(1.0);
}
