#include "Vector.h"

#include "IndependentDOF.h"

using namespace std;
using namespace GiNaC;

Vector :: Vector ( double x, double y, double z, bool x_free, bool y_free, bool z_free):
x_(new IndependentDOF(x,x_free)),
y_(new IndependentDOF(y,y_free)),
z_(new IndependentDOF(z,z_free))
{
	dof_list_.push_back(x_);
	dof_list_.push_back(y_);
	dof_list_.push_back(z_);
}

// Return mmaMatrix vector of current x,y,z values
// used for numerical calculations only, loses associativity with DOF's
mmcMatrix Vector::GetmmcMatrix()
{
	mmcMatrix result(3,1);

	result(0,0) = x_->GetValue();
	result(1,0) = y_->GetValue();
	result(2,0) = z_->GetValue();

	return result;
}

void Vector::ApplySelectionMask(SelectionMask mask)
{
	if(mask == All)
		SetSelectable(true);
	else
		SetSelectable(false);
}