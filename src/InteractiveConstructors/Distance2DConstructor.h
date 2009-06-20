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

#ifndef Distance2DConstructorH
#define Distance2DConstructorH

#include "InteractiveConstructorBase.h"

class Distance2DConstructor : public InteractiveConstructorBase
{
    public:
        Distance2DConstructor(QtSketchPointer parent_sketch);
        ~Distance2DConstructor() {parent_sketch_->ApplySelectionMask(All); parent_sketch_->ClearSelected();}  // turn off selection mask

        void CreateObject();

        // gui event methods
        bool MouseMove(MotionEventPropertiesPointer event_props);
        bool LeftButtonUp(MouseEventPropertiesPointer event_props);

    private:
        bool primitive_finished_;
        bool primitive1_defined_;
        double current_s_;
        double current_t_;
    
        Point2DPointer point1_;
        Point2DPointer point2_;

        Line2DPointer line1_;
        Line2DPointer line2_;
};

#endif //Distance2DConstructorH



