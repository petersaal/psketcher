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

#ifndef Arc2DConstructorH
#define Arc2DConstructorH

#include "InteractiveConstructorBase.h"

class Arc2DConstructor : public InteractiveConstructorBase
{
	public:
		Arc2DConstructor(QtSketchPointer parent_sketch);

		void CreateObject();

		// gui event methods
		bool LeftButtonUp(MouseEventPropertiesPointer event_props);

	private:
		bool primitive_finished_;

		int current_point_number_;

		double point1_s_;
		double point1_t_;

		double point2_s_;
		double point2_t_;

		double point3_s_;
		double point3_t_;
};

#endif //Arc2DConstructorH




