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

#ifndef Point2DConstructorH
#define Point2DConstructorH

#include "InteractiveConstructorBase.h"

class Point2DConstructor : public InteractiveConstructorBase
{
	public:
		Point2DConstructor(QtSketchPointer parent_sketch);

		void CreateObject();

		// gui event methods
		bool LeftButtonUp(MouseEventPropertiesPointer event_props);

	private:
		bool primitive_finished_;
		double current_s_;
		double current_t_;
};

#endif //Point2DConstructorH




