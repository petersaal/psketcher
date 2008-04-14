#include "DistancePoint2DConstructor.h"

DistancePoint2DConstructor::DistancePoint2DConstructor(OccSketchPointer parent_sketch, Handle(V3d_View) current_view, Handle(V3d_Viewer) current_viewer):
InteractiveConstructorBase(parent_sketch, current_view, current_viewer),
primitive_finished_(false),
point1_defined_(false)
{
	parent_sketch_->ApplySelectionMask(Points);
	parent_sketch_->GetAISContext()->ClearSelected();
}

void DistancePoint2DConstructor::CreateObject()
{
	if(primitive_finished_)
	{
		parent_sketch_->AddDistancePoint2D(point1_, point2_);
	}
}

bool DistancePoint2DConstructor::MouseMove(MotionEventPropertiesPointer event_props)
{
	parent_sketch_->GetAISContext()->MoveTo((int)event_props->GetXPosition(), (int)event_props->GetYPosition(), current_view_);

	return false;
}

bool DistancePoint2DConstructor::LeftButtonUp(MouseEventPropertiesPointer event_props)
{
	// perform selection operation
	parent_sketch_->GetAISContext()->Select();

	// first, make sure a point is selected
	std::vector<PrimitiveBasePointer> primitive_list = parent_sketch_->GetSelectedPrimitives();

	if(primitive_list.size() < 1 )
		return false; 							// no primitive selected

	if(dynamic_cast<OccPoint2D*>(primitive_list[0].get()) != 0){
		OccPoint2DPointer new_point = boost::dynamic_pointer_cast<OccPoint2D>(primitive_list[0]);

		if(point1_defined_)
		{
			point2_ = new_point;
			
			if(!(point2_->GetSDOF() == point1_->GetSDOF() && point2_->GetTDOF() == point1_->GetTDOF()))
			{
				primitive_finished_ = true;
				return true;
			} else {
				return false;  // point2_ is the same as point1_ so line cannot be created
			}
		} else {
			point1_ = new_point;
			point1_defined_ = true;
			return false;
		}

	} else {
		return false; // point not selected
	}
}