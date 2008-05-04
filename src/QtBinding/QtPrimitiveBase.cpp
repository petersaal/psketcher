#include "OccPrimitiveBase.h"

void OccPrimitiveBase::Display()
{
	for(unsigned int current_index = 0; current_index < ais_object_list_.size(); current_index++)
	{
		ais_context_->Display(ais_object_list_[current_index], Standard_False);
	}
}

void OccPrimitiveBase::UpdateDisplay()
{
	for(unsigned int current_index = 0; current_index < ais_object_list_.size(); current_index++)
	{
		ais_context_->Redisplay(ais_object_list_[current_index]);
	}
}

bool OccPrimitiveBase::IsSelected()
{
	bool selected = false;

	for(unsigned int current_index = 0; current_index < ais_object_list_.size(); current_index++)
	{
		if(ais_context_->IsCurrent(ais_object_list_[current_index]))
			selected = true;
	}

	return selected;
}

void OccPrimitiveBase::SetSelectable(bool selectable)
{
	for(unsigned int current_index = 0; current_index < ais_object_list_.size(); current_index++)
	{
		if(!selectable)
		{
			ais_context_->Deactivate(ais_object_list_[current_index]);
			//ais_context_->Remove(ais_object_list_[current_index]);
			//ais_context_->Display(ais_object_list_[current_index],0, -1);
			//ais_context_->Redisplay(ais_object_list_[current_index]);
		} else {
			ais_context_->Activate(ais_object_list_[current_index]);
			//ais_context_->Deactivate(ais_object_list_[current_index]);
			//ais_context_->Remove(ais_object_list_[current_index]);
			//ais_context_->Display(ais_object_list_[current_index],0, 0);
			//ais_context_->Redisplay(ais_object_list_[current_index]);
		}
	}
}

// Erase current ais_object from the ais_context
void OccPrimitiveBase::Erase()
{
	for(unsigned int current_index = 0; current_index < ais_object_list_.size(); current_index++)
	{
		ais_context_->Erase(ais_object_list_[current_index]);
	}

	ais_object_list_.clear();
}
