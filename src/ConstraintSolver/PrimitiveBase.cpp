#include "PrimitiveBase.h"
#include "DependentDOF.h"

using namespace std;
using namespace GiNaC;

// Initialize private static variables for DOF and PrimitiveBase classes
unsigned PrimitiveBase::next_id_number_ = 1;

PrimitiveBase::PrimitiveBase():
id_number_(next_id_number_++),
selected_(false),
selectable_(true),
delete_me_(false)
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

void PrimitiveBase::AddDOF(DOFPointer new_dof)
{
	// add the new dof to the list
	dof_list_.push_back(new_dof);

	// check to see if the dof to be added is a dependent, if so at the dof's that it dependends on as well
	if(dynamic_cast<DependentDOF*>(new_dof.get()) != 0)
	{
		DependentDOFPointer dependent_dof = boost::dynamic_pointer_cast<DependentDOF>(new_dof);
		
		std::vector<DOFPointer> temp_dof_list = dependent_dof->GetDOFList();

		for(unsigned int current_dof = 0; current_dof < temp_dof_list.size(); current_dof++)
			dof_list_.push_back(temp_dof_list[current_dof]);
	}
	
	// lastly, remove any duplicate degrees of freedom in the list
	sort( dof_list_.begin(), dof_list_.end());
	dof_list_.erase( unique( dof_list_.begin(), dof_list_.end()), dof_list_.end());
}

// returns true if deletion flag changes, otherwise returns false
bool PrimitiveBase::FlagForDeletionIfDependent(boost::shared_ptr<PrimitiveBase> input_primitive)
{
	// short circuit if already flagged for deletion
	if(IsFlaggedForDeletion())
		return false;

	// check to see if this primitive depends on the input primitive
	if (find(primitive_list_.begin(), primitive_list_.end(), input_primitive) != primitive_list_.end())
	{
		FlagForDeletion();
		return true;
	} else {
		return false;
	}
}
