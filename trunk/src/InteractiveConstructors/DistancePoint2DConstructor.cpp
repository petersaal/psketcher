/****************************************************************************
**
** This file is part of the pSketcher project.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Copyright (C) 2006-2008 Michael Greminger. All rights reserved.
**
****************************************************************************/

#include "DistancePoint2DConstructor.h"

DistancePoint2DConstructor::DistancePoint2DConstructor(QtSketchPointer parent_sketch):
InteractiveConstructorBase(parent_sketch),
primitive_finished_(false),
point1_defined_(false)
{
	parent_sketch_->ApplySelectionMask(Points);
	parent_sketch_->ClearSelected();
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
	// parent_sketch_->GetAISContext()->MoveTo((int)event_props->GetXPosition(), (int)event_props->GetYPosition(), current_view_);

	return false;
}

bool DistancePoint2DConstructor::LeftButtonUp(MouseEventPropertiesPointer event_props)
{
	// perform selection operation
	// parent_sketch_->GetAISContext()->Select();

	// first, make sure a point is selected
	std::vector<PrimitiveBasePointer> primitive_list = parent_sketch_->GetSelectedPrimitives();

	if(primitive_list.size() < 1 )
		return false; 							// no primitive selected

	if(dynamic_cast<QtPoint2D*>(primitive_list[0].get()) != 0){
		QtPoint2DPointer new_point = boost::dynamic_pointer_cast<QtPoint2D>(primitive_list[0]);

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
