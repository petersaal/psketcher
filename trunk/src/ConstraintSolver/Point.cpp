#include "Point.h"

#include "IndependentDOF.h"

using namespace std;
using namespace GiNaC;

Point :: Point ( double x, double y, double z, bool x_free, bool y_free, bool z_free):
x_(new IndependentDOF(x,x_free)),
y_(new IndependentDOF(y,y_free)),
z_(new IndependentDOF(z,z_free))
{
	AddDOF(x_);
	AddDOF(y_);
	AddDOF(z_);
}

void Point::ApplySelectionMask(SelectionMask mask)
{
	if(mask == All || mask == Points)
		SetSelectable(true);
	else
		SetSelectable(false);
}

// Return mmaMatrix vector of current x,y,z values
// used for numerical calculations only, loses associativity with DOF's
mmcMatrix Point::GetmmcMatrix()
{
	mmcMatrix result(3,1);

	result(0,0) = x_->GetValue();
	result(1,0) = y_->GetValue();
	result(2,0) = z_->GetValue();

	return result;
}
