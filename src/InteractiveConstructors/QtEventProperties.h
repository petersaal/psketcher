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

#ifndef QtEventPropertiesH
#define QtEventPropertiesH

#include "InteractiveConstructorBase.h"

class QMouseEvent;

// This is the GTK binding for the mouse and motion events

// Get binding for the mouse event properties (only the constructor needs to be defined) 
class QtMouseEventProperties : public MouseEventProperties
{
	public:
		// Constructor
		QtMouseEventProperties(MouseButtonEventType event_type, QMouseEvent *event, QGraphicsView *view);

	private:
};
typedef std::auto_ptr<QtMouseEventProperties> QtMouseEventPropertiesPointer;



// Get binding for the mouse event properties (only the constructor needs to be defined) 
class QtMotionEventProperties : public MotionEventProperties
{
	public:
		// Constructor
		QtMotionEventProperties(QMouseEvent  *event, QGraphicsView *view);

	private:
};
typedef std::auto_ptr<QtMotionEventProperties> QtMotionEventPropertiesPointer;

#endif //QtEventPropertiesH
