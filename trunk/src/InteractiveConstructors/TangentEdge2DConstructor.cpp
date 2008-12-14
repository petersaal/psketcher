#include "TangentEdge2DConstructor.h"

TangentEdge2DConstructor::TangentEdge2DConstructor(QtSketchPointer parent_sketch):
InteractiveConstructorBase(parent_sketch),
primitive_finished_(false),
edge1_defined_(false)
{
	parent_sketch_->ApplySelectionMask(Edges);
	parent_sketch_->ClearSelected();
}

void TangentEdge2DConstructor::CreateObject()
{
	if(primitive_finished_)
	{		
		parent_sketch_->AddTangentEdge2D(edge1_, edge1_point_number_,edge2_,edge2_point_number_);
	}
}

bool TangentEdge2DConstructor::LeftButtonUp(MouseEventPropertiesPointer event_props)
{
	// first, make sure a point is selected
	std::vector<PrimitiveBasePointer> primitive_list = parent_sketch_->GetSelectedPrimitives();

	if(primitive_list.size() < 1 )
		return false; 							// no primitive selected

	if(dynamic_cast<Edge2DBase*>(primitive_list[0].get()) != 0){
		Edge2DBasePointer new_edge = boost::dynamic_pointer_cast<Edge2DBase>(primitive_list[0]);

		if(edge1_defined_)
		{
			edge2_ = new_edge;
			
			if(!(edge1_ == edge2_))
			{
				// make sure the two edges share an endpoint
				edge1_point_number_ = Point1;
				edge2_point_number_ = Point1;	
				if(edge1_->IsPointCoincident(edge1_point_number_,edge2_,edge2_point_number_))
				{
					primitive_finished_ = true;
					return true;
				} else if(edge1_->IsPointCoincident(Point2,edge2_,Point1)) {
					edge1_point_number_ = Point2;
					edge2_point_number_ = Point1;

					primitive_finished_ = true;
					return true;
				} else if(edge1_->IsPointCoincident(Point1,edge2_,Point2)) {
					edge1_point_number_ = Point1;
					edge2_point_number_ = Point2;

					primitive_finished_ = true;
					return true;
				} else if(edge1_->IsPointCoincident(Point2,edge2_,Point2)) {
					edge1_point_number_ = Point2;
					edge2_point_number_ = Point2;

					primitive_finished_ = true;
					return true;
				}

				// the two edges do not share a common end point
				return false;
			} else {
				return false;  // edge2_ is the same as edge1_ 
			}
		} else {
			edge1_ = new_edge;
			edge1_defined_ = true;
			return false;
		}

	} else {
		return false; // point not selected
	}
}
