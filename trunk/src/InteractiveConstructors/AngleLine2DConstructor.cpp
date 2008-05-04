#include "AngleLine2DConstructor.h"

AngleLine2DConstructor::AngleLine2DConstructor(QtSketchPointer parent_sketch, Handle(V3d_View) current_view, Handle(V3d_Viewer) current_viewer):
InteractiveConstructorBase(parent_sketch, current_view, current_viewer),
primitive_finished_(false),
line1_defined_(false)
{
	parent_sketch_->ApplySelectionMask(Lines);
	parent_sketch_->GetAISContext()->ClearSelected();
}

void AngleLine2DConstructor::CreateObject()
{
	if(primitive_finished_)
	{

		double angle = 10.0*(mmcPI/180.0);
		
		parent_sketch_->AddAngleLine2D(line1_, line2_,angle);
	}
}

bool AngleLine2DConstructor::MouseMove(MotionEventPropertiesPointer event_props)
{
	parent_sketch_->GetAISContext()->MoveTo((int)event_props->GetXPosition(), (int)event_props->GetYPosition(), current_view_);

	return false;
}

bool AngleLine2DConstructor::LeftButtonUp(MouseEventPropertiesPointer event_props)
{
	// perform selection operation
	parent_sketch_->GetAISContext()->Select();

	// first, make sure a point is selected
	std::vector<PrimitiveBasePointer> primitive_list = parent_sketch_->GetSelectedPrimitives();

	if(primitive_list.size() < 1 )
		return false; 							// no primitive selected

	if(dynamic_cast<QtLine2D*>(primitive_list[0].get()) != 0){
		QtLine2DPointer new_line = boost::dynamic_pointer_cast<QtLine2D>(primitive_list[0]);

		if(line1_defined_)
		{
			line2_ = new_line;
			
			if(!(line1_ == line2_))
			{
				primitive_finished_ = true;
				return true;
			} else {
				return false;  // line2_ is the same as line1_ so line cannot be created
			}
		} else {
			line1_ = new_line;
			line1_defined_ = true;
			return false;
		}

	} else {
		return false; // point not selected
	}
}
