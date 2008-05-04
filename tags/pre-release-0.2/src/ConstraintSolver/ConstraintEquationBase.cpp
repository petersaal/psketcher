#include "ConstraintEquationBase.h"

using namespace std;
using namespace GiNaC;

void ConstraintEquationBase::ApplySelectionMask(SelectionMask mask)
{
	if(mask == All || mask == Constraints)
		SetSelectable(true);
	else
		SetSelectable(false);
}
