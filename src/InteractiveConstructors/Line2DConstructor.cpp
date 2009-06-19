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

#include "Line2DConstructor.h"

Line2DConstructor::Line2DConstructor(QtSketchPointer parent_sketch):
InteractiveConstructorBase(parent_sketch),
primitive_finished_(false),
point1_defined_(false),
delete_point1_on_cancel_(false),
delete_point2_on_cancel_(false),
temp_point_defined_(false),
temp_line_defined_(false)
{
	parent_sketch_->ApplySelectionMask(Points);
	parent_sketch_->ClearSelected();
}

Line2DConstructor::~Line2DConstructor() 
{
    // Delete any primitives that where not used because this object was killed before the line
    // could be completed.
    if(delete_point1_on_cancel_)
        parent_sketch_->DeletePrimitive(point1_);

    if(delete_point2_on_cancel_)
        parent_sketch_->DeletePrimitive(point2_);

    if(temp_point_defined_)
        parent_sketch_->DeletePrimitive(temp_point_);

    if(temp_line_defined_)
        parent_sketch_->DeletePrimitive(temp_line_);

    parent_sketch_->ApplySelectionMask(All); 
    parent_sketch_->ClearSelected();
}  

void Line2DConstructor::CreateObject()
{
	if(primitive_finished_)
	{
		parent_sketch_->AddLine2D(point1_, point2_);

        // any temporary objects created can now be kept
        delete_point1_on_cancel_ = false;
        delete_point2_on_cancel_ = false;
	}
}

bool Line2DConstructor::MouseMove(MotionEventPropertiesPointer event_props)
{
    if(temp_point_defined_)
    {
        double x = event_props->GetXPosition();
        double y = event_props->GetYPosition();
        double z = event_props->GetZPosition();

        double motion_s, motion_t;
        
        // project x,y,z coordinates onto sketch plane
        parent_sketch_->GetSketchPlane()->GetSTLocation(x,y,z,motion_s,motion_t);

        temp_point_->SetSValue(motion_s);
        temp_point_->SetTValue(motion_t);

        parent_sketch_->UpdateDisplay();
    }

	return false;
}

bool Line2DConstructor::LeftButtonUp(MouseEventPropertiesPointer event_props)
{
    bool new_point_created = false;
    QtPoint2DPointer new_point;
    
	// first, check to see if a point was created
	std::vector<PrimitiveBasePointer> primitive_list = parent_sketch_->GetSelectedPrimitives();

    if(primitive_list.size() >= 1 && dynamic_cast<QtPoint2D*>(primitive_list[0].get()) != 0)
    {
        new_point = boost::dynamic_pointer_cast<QtPoint2D>(primitive_list[0]);
    } else {
        // no point was selected so create a point at the selection point
        
        double x = event_props->GetXPosition();
        double y = event_props->GetYPosition();
        double z = event_props->GetZPosition();

        double click_s, click_t;
        
        // project x,y,z coordinates onto sketch plane
        parent_sketch_->GetSketchPlane()->GetSTLocation(x,y,z,click_s,click_t);

        new_point = parent_sketch_->AddPoint2D(click_s,click_t,true,true);

		new_point_created = true;
    }

    if(point1_defined_)
    {
        point2_ = new_point;
        if(new_point_created)
            delete_point2_on_cancel_ = true;
        
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
        if(new_point_created)
            delete_point1_on_cancel_ = true;

        // create a temp line to provide feedback to the user as the line is created
        temp_point_ = parent_sketch_->AddPoint2D(point1_->GetSValue(), point1_->GetTValue(),true,true);
        temp_point_defined_ = true;
        temp_line_ = parent_sketch_->AddLine2D(point1_,temp_point_);
        temp_line_defined_ = true;
        temp_point_->SetSelectable(false); // cannot select this temp point otherwise other points will not be selected

        return false;
    }

}




