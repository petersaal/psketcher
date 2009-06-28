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

#ifndef Circle2DConstructorH
#define Circle2DConstructorH

#include "InteractiveConstructorBase.h"

class Circle2DConstructor : public InteractiveConstructorBase
{
	public:
		Circle2DConstructor(QtSketchPointer parent_sketch);
		~Circle2DConstructor();

		void CreateObject();

		// gui event methods
		bool MouseMove(MotionEventPropertiesPointer event_props);
		bool LeftButtonUp(MouseEventPropertiesPointer event_props);

	private:
		bool primitive_finished_;
	
		Point2DPointer center_point_;
        double center_s_, center_t_, current_radius_;

        Circle2DPointer temp_circle_;
};

#endif //Circle2DConstructorH



