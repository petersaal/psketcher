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
