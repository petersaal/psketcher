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

#include "ParallelLine2DConstructor.h"

ParallelLine2DConstructor::ParallelLine2DConstructor(QtSketchPointer parent_sketch):
InteractiveConstructorBase(parent_sketch),
primitive_finished_(false),
line1_defined_(false)
{
	parent_sketch_->ApplySelectionMask(Lines);
	parent_sketch_->ClearSelected();
}

void ParallelLine2DConstructor::CreateObject()
{
	if(primitive_finished_)
	{		
		parent_sketch_->AddParallelLine2D(line1_, line2_);
	}
}

bool ParallelLine2DConstructor::MouseMove(MotionEventPropertiesPointer event_props)
{
	// parent_sketch_->GetAISContext()->MoveTo((int)event_props->GetXPosition(), (int)event_props->GetYPosition(), current_view_);

	return false;
}

bool ParallelLine2DConstructor::LeftButtonUp(MouseEventPropertiesPointer event_props)
{
	// perform selection operation
	//parent_sketch_->GetAISContext()->Select();

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
