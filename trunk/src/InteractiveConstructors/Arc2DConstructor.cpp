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

#include "Arc2DConstructor.h"

Arc2DConstructor::Arc2DConstructor(QtSketchPointer parent_sketch):
InteractiveConstructorBase(parent_sketch),
primitive_finished_(false)
{
	current_point_number_ = 0;
}

void Arc2DConstructor::CreateObject()
{
	if(primitive_finished_)
	{
		parent_sketch_->AddArc2D(point1_s_,point1_t_,point2_s_,point2_t_,point3_s_,point3_t_,true,true,true,true,false);
	}
}

bool Arc2DConstructor::LeftButtonUp(MouseEventPropertiesPointer event_props)
{
	double x = event_props->GetXPosition();
	double y = event_props->GetYPosition();
	double z = event_props->GetZPosition();
	
	current_point_number_++;

	if(current_point_number_ == 1)
	{
		// project x,y,z coordinates onto sketch plane
		parent_sketch_->GetSketchPlane()->GetSTLocation(x,y,z,point1_s_,point1_t_);
	} else if(current_point_number_ == 2) {
		// project x,y,z coordinates onto sketch plane
		parent_sketch_->GetSketchPlane()->GetSTLocation(x,y,z,point2_s_,point2_t_);
	} else if(current_point_number_ == 3) {
		// project x,y,z coordinates onto sketch plane
		parent_sketch_->GetSketchPlane()->GetSTLocation(x,y,z,point3_s_,point3_t_);
	}
	
	
	if(current_point_number_ == 3)
		primitive_finished_ = true;
	
	return primitive_finished_;
}
