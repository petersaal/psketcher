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

#include "HoriVertLine2DConstructor.h"

HoriVertLine2DConstructor::HoriVertLine2DConstructor(QtSketchPointer parent_sketch, bool vertical_constraint):
InteractiveConstructorBase(parent_sketch),
vertical_constraint_(vertical_constraint),
primitive_finished_(false)
{
	parent_sketch_->ApplySelectionMask(Lines);
	parent_sketch_->ClearSelected();
}

void HoriVertLine2DConstructor::CreateObject()
{
	if(primitive_finished_)
	{		
		// @fixme need to determine if user intended an interior angle or an exterior angle
		parent_sketch_->AddHoriVertLine2D(line_, vertical_constraint_);
	}
}

bool HoriVertLine2DConstructor::MouseMove(MotionEventPropertiesPointer event_props)
{

	return false;
}

bool HoriVertLine2DConstructor::LeftButtonUp(MouseEventPropertiesPointer event_props)
{
	// perform selection operation
	//parent_sketch_->GetAISContext()->Select();

	// first, make sure a point is selected
	std::vector<PrimitiveBasePointer> primitive_list = parent_sketch_->GetSelectedPrimitives();

	if(primitive_list.size() < 1 )
		return false; 							// no primitive selected

	if(dynamic_cast<QtLine2D*>(primitive_list[0].get()) != 0){
		line_ = boost::dynamic_pointer_cast<QtLine2D>(primitive_list[0]);
		
		primitive_finished_ = true;
		
		return true;

	} else {
		return false; // line not selected
	}
}
