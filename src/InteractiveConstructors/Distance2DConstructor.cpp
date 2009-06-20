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

#include "Distance2DConstructor.h"

Distance2DConstructor::Distance2DConstructor(QtSketchPointer parent_sketch):
InteractiveConstructorBase(parent_sketch),
primitive_finished_(false),
primitive1_defined_(false)
{
    parent_sketch_->ApplySelectionMask(PointsAndLines);
    parent_sketch_->ClearSelected();
}

void Distance2DConstructor::CreateObject()
{
    if(primitive_finished_)
    {
        if(point1_.get() != 0 && point2_.get() != 0)
        {
            parent_sketch_->AddDistancePoint2D(point1_, point2_);
        } else if(point1_.get() != 0 && line2_.get() != 0) {
            parent_sketch_->AddDistancePointLine2D(point1_,line2_);
        } else if(line1_.get() != 0 && point2_.get() != 0) {
            parent_sketch_->AddDistancePointLine2D(point2_,line1_);
        } else { 
            // only case left is distance between two lines (use point from first line to make a point to line constraint)
            parent_sketch_->AddDistancePointLine2D(line1_->GetPoint1(),line2_);
        }
    }
}

bool Distance2DConstructor::MouseMove(MotionEventPropertiesPointer event_props)
{
    // parent_sketch_->GetAISContext()->MoveTo((int)event_props->GetXPosition(), (int)event_props->GetYPosition(), current_view_);

    return false;
}

bool Distance2DConstructor::LeftButtonUp(MouseEventPropertiesPointer event_props)
{
    // perform selection operation
    // parent_sketch_->GetAISContext()->Select();

    // first, make sure a point is selected
    std::vector<PrimitiveBasePointer> primitive_list = parent_sketch_->GetSelectedPrimitives();

    if(primitive_list.size() < 1 )
        return false;                           // no primitive selected

    if(dynamic_cast<QtPoint2D*>(primitive_list[0].get()) != 0){
        QtPoint2DPointer new_point = boost::dynamic_pointer_cast<QtPoint2D>(primitive_list[0]);

        if(primitive1_defined_)
        {
            point2_ = new_point;

            if(!(point2_.get() == point1_.get() || (line1_.get() != 0 && line1_->GetPoint1().get() == point2_.get()) || (line1_.get() != 0 && line1_->GetPoint2().get() == point2_.get())  ) )
            {
                primitive_finished_ = true;
                return true;
            } else {
                return false;  // point2_ is the same as point1_ so line cannot be created
            }
        } else {
            point1_ = new_point;
            primitive1_defined_ = true;
            return false;
        }

    } else if(dynamic_cast<QtLine2D*>(primitive_list[0].get()) != 0) {
        QtLine2DPointer new_line = boost::dynamic_pointer_cast<QtLine2D>(primitive_list[0]);

        if(primitive1_defined_)
        {
            line2_ = new_line;

            if(!(line1_.get() == line2_.get() || line2_->GetPoint1().get() == point1_.get() || line2_->GetPoint2().get() == point1_.get() ))
            {
                primitive_finished_ = true;
                return true;
            } else {
                return false;  // line2_ is the same as line1_ so line cannot be created
            }
        } else {
            line1_ = new_line;
            primitive1_defined_ = true;
            return false;
        }

    } else {
        return false; // line or point not selected
    }
}
