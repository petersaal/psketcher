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

#ifndef GtkEventPropertiesH
#define GtkEventPropertiesH

#include <gtk/gtk.h>

#include "InteractiveConstructorBase.h"

// This is the GTK binding for the mouse and motion events

// Get binding for the mouse event properties (only the constructor needs to be defined) 
class GtkMouseEventProperties : public MouseEventProperties
{
	public:
		// Constructor
		GtkMouseEventProperties(MouseButtonEventType event_type, GdkEventButton *event);

	private:
};
typedef std::auto_ptr<GtkMouseEventProperties> GtkMouseEventPropertiesPointer;



// Get binding for the mouse event properties (only the constructor needs to be defined) 
class GtkMotionEventProperties : public MotionEventProperties
{
	public:
		// Constructor
		GtkMotionEventProperties(GdkEventMotion  *event);

	private:
};
typedef std::auto_ptr<GtkMotionEventProperties> GtkMotionEventPropertiesPointer;

#endif //GtkEventPropertiesH
