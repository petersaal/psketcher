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


Point :: Point (double x,double y,double z):
x_(new DOF(x,false)),
y_(new DOF(y,false)),
z_(new DOF(z,false))
{

}

Line :: Line(const Point &point1, const Point &point2)
{
	x1_ = point1.GetXDOF();
	y1_ = point1.GetYDOF();
	z1_ = point1.GetZDOF();

	x2_ = point2.GetXDOF();
	y2_ = point2.GetYDOF();
	z2_ = point2.GetZDOF();
}

// Create a constraint that defines the distance between two points
DisplacementConstraint::DisplacementConstraint(const Point &point1, const Point &point2, double distance)
{
	// First must clear existing constraint since this class is only intended to manage a single constraint type at a time
	constraints_.clear();

	// clear any existing DOF's
	dof_list_.clear();

	// Create a DOF for the distance parameter
	DOFPointer new_dof(new DOF(distance,false));
	
	dof_list_.push_back(new_dof);

	// create the expression that defines the constraint and add it the the constraint list
	boost::shared_ptr<ex> new_constraint(new ex);
	
	*new_constraint = sqrt( pow(point1.GetXDOF()->GetVariable() - point2.GetXDOF()->GetVariable(),2) +
													pow(point1.GetYDOF()->GetVariable() - point2.GetYDOF()->GetVariable(),2) +
													pow(point1.GetZDOF()->GetVariable() - point2.GetZDOF()->GetVariable(),2))
										- new_dof->GetVariable();

	constraints_.push_back(new_constraint);
}
