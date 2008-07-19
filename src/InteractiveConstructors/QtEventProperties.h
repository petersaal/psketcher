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
