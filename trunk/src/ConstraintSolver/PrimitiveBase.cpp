#include "PrimitiveBase.h"

using namespace std;
using namespace GiNaC;

// Initialize private static variables for DOF and PrimitiveBase classes
unsigned PrimitiveBase::next_id_number_ = 1;

PrimitiveBase::PrimitiveBase():
id_number_(next_id_number_++),
selected_(false),
selectable_(true)
{

}

void PrimitiveBase::SetSelectable(bool selectable)
{
	selectable_ = selectable;
	
	// if not selectable, need to make sure selected flag is false
	if(!selectable_)
		selected_ = false;
}

// Default selection mask implementation, each derived class should override this method
void PrimitiveBase::ApplySelectionMask(SelectionMask mask)
{
	if(mask == All)
		SetSelectable(true);
	else
		SetSelectable(false);
}