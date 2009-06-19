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

#ifndef Line2DConstructorH
#define Line2DConstructorH

#include "InteractiveConstructorBase.h"

class Line2DConstructor : public InteractiveConstructorBase
{
	public:
		Line2DConstructor(QtSketchPointer parent_sketch);
		~Line2DConstructor();

		void CreateObject();

		// gui event methods
		bool MouseMove(MotionEventPropertiesPointer event_props);
		bool LeftButtonUp(MouseEventPropertiesPointer event_props);

	private:
		bool primitive_finished_;
		bool point1_defined_;
	
		Point2DPointer point1_;
		Point2DPointer point2_;

        Point2DPointer temp_point_;
        bool temp_point_defined_;
        Line2DPointer temp_line_;
        bool temp_line_defined_;
        
        bool delete_point1_on_cancel_;
        bool delete_point2_on_cancel_;
};

#endif //Line2DConstructorH



