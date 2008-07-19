#include <QMouseEvent>
#include <QGraphicsView>
#include "QtEventProperties.h"

// Constructor
QtMouseEventProperties::QtMouseEventProperties(MouseButtonEventType event_type, QMouseEvent  *event, QGraphicsView *view)
{
	screen_x_position_ = event->x();
	screen_y_position_ = event->y();
	
	// translate the screen coordinates to world coordinates
	QPointF scene_location = view->mapToScene(screen_x_position_,screen_y_position_);
	global_x_position_ = scene_location.x();
	global_y_position_ = -scene_location.y();
	global_z_position_ = 0.0;
	
	control_ = event->modifiers() & Qt::ControlModifier;
	shift_ = event->modifiers() & Qt::ShiftModifier;
	alt_ = event->modifiers() & Qt::AltModifier;
	
	switch (event->button()) {
		case Qt::LeftButton:  mouse_button_ = LeftButton; break;
		case Qt::MidButton:   mouse_button_ = MiddleButton; break;
		case Qt::RightButton: mouse_button_ = RightButton; break;
		case Qt::XButton1:    mouse_button_ = Button4; break;
		case Qt::XButton2:    mouse_button_ = Button5; break;
		default:              mouse_button_ = OtherButton; 
	}

	mouse_event_type_ = event_type;
}

QtMotionEventProperties::QtMotionEventProperties(QMouseEvent  *event, QGraphicsView *view)
{
	screen_x_position_ = event->x();
	screen_y_position_ = event->y();

	// translate the screen coordinates to world coordinates
	QPointF scene_location = view->mapToScene(screen_x_position_,screen_y_position_);
	global_x_position_ = scene_location.x();
	global_y_position_ = -scene_location.y();
	global_z_position_ = 0.0;

	control_ = event->modifiers() & Qt::ControlModifier;
	shift_ = event->modifiers() & Qt::ShiftModifier;
	alt_ = event->modifiers() & Qt::AltModifier;
	
	left_button_ = event->buttons() & Qt::LeftButton;
	middle_button_ = event->buttons() & Qt::MidButton;
	right_button_ = event->buttons() & Qt::RightButton;
	button_4_ = event->buttons() & Qt::XButton1;
	button_5_ = event->buttons() & Qt::XButton2;
}
