#include "GtkEventProperties.h"

// Constructor
GtkMouseEventProperties::GtkMouseEventProperties(MouseButtonEventType event_type, GdkEventButton  *event)
{
	x_position_ = event->x;
	y_position_ = event->y;
	

	control_ = event->state & GDK_CONTROL_MASK;
	shift_ = event->state & GDK_SHIFT_MASK;
	alt_ = event->state &GDK_MOD1_MASK;
	
	switch (event->button) {
		case 1: mouse_button_ = LeftButton; break;
		case 2: mouse_button_ = MiddleButton; break;
		case 3: mouse_button_ = RightButton; break;
		case 4: mouse_button_ = Button4; break;
		case 5: mouse_button_ = Button5; break;
		default: mouse_button_ = OtherButton;
	}

	mouse_event_type_ = event_type;
}

GtkMotionEventProperties::GtkMotionEventProperties(GdkEventMotion  *event)
{
	x_position_ = event->x;
	y_position_ = event->y;
	
	control_ = event->state & GDK_CONTROL_MASK;
	shift_ = event->state & GDK_SHIFT_MASK;
	alt_ = event->state &GDK_MOD1_MASK;
	
	left_button_ = event->state & GDK_BUTTON1_MASK;
	middle_button_ = event->state & GDK_BUTTON2_MASK;
	right_button_ = event->state & GDK_BUTTON3_MASK;
	button_4_ = event->state & GDK_BUTTON4_MASK;
	button_5_ = event->state & GDK_BUTTON5_MASK;
}
