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
		parent_sketch_->AddPoint2D(current_s_, current_t_, true, true);
	}
}

bool Point2DConstructor::LeftButtonUp(MouseEventPropertiesPointer event_props)
{
	double x = event_props->GetXPosition();
	double y = event_props->GetYPosition();
	double z = event_props->GetZPosition();
	
	// project x,y,z coordinates onto sketch plane
	parent_sketch_->GetSketchPlane()->GetSTLocation(x,y,z,current_s_,current_t_);
	primitive_finished_ = true;
	
	return primitive_finished_;
}
