#include "Point2DConstructor.h"

Point2DConstructor::Point2DConstructor(QtSketchPointer parent_sketch):
InteractiveConstructorBase(parent_sketch),
primitive_finished_(false)
{

}

void Point2DConstructor::CreateObject()
{
	if(primitive_finished_)
	{
		parent_sketch_->AddPoint2D(current_s_, current_t_, false, false);
	}
}

bool Point2DConstructor::LeftButtonUp(MouseEventPropertiesPointer event_props)
{
	int x_screen = (int)event_props->GetXPosition();
	int y_screen = (int)event_props->GetYPosition();

	double x,y,z;
	
	// if control button is pressed, force grid snapping. Otherwise used the parent_sketch's grid snapping preference
	bool success;
	if(event_props->ControlPressed())
		success = GetGlobalLocation(x_screen, y_screen, x, y, z, true);
	else
		success = GetGlobalLocation(x_screen, y_screen, x, y, z, parent_sketch_->GetGridSnap());
		
	if(success)
	{
		parent_sketch_->GetSketchPlane()->GetSTLocation(x,y,z,current_s_,current_t_);
		primitive_finished_ = true;
	}
	
	return success;
}
