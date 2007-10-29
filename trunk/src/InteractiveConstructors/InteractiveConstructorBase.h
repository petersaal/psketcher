#ifndef InteractiveConstructorBaseH
#define InteractiveConstructorBaseH

enum MouseButton {LeftButton, MiddleButton, RightButton, Button4, Button5};
enum MouseButtonEventType {ButtonPress, ButtonRelease};

class MouseEventProperties
{
	public:
		// Constructor (there is no default constructor, each binding will need to define its own)
		MouseEventProperties()=0;

		// Accessor methods used to retrieve values (the constructor will define all of the values)
		double GetXPosition() {return x_position;}
		double GetYPosition() {return y_position;}
		
		bool ControlPressed() {return control_;}
		bool ShiftPressed() {return shift_;}
		bool AltPressed() {return alt_;}

		MouseButton GetButton() {return mouse_button_;}
		MouseButtonEventType GetEventType() {return mouse_event_type_;}

	protected:
		double x_position_;
		double y_position_;
		
		bool control_;
		bool shift_;
		bool alt_;

		MouseButton mouse_button_;
		MouseButtonEventType mouse_event_type_;
};
typedef boost::shared_ptr<MouseEventProperties> MouseEventPropertiesPointer;

// Get binding for the mouse event properties (only the constructor needs to be defined) 
class GtkMouseEventProperties : public MouseEventProperties
{
	// Constructor
	GtkMouseEventProperties(GdkEventButton  *event);
};
typedef boost::shared_ptr<GtkMouseEventProperties> GtkMouseEventPropertiesPointer;

class MotionEventProperties
{
	public:
		// Constructor (there is no default constructor, each binding will need to define its own)
		MotionEventProperties()=0;

		// Accessor methods used to retrieve values (the constructor will define all of the values)
	private:
};
typedef boost::shared_ptr<MotionEventProperties> MotionEventPropertiesPointer;

// Get binding for the mouse event properties (only the constructor needs to be defined) 
class GtkMotionEventProperties : public MotionEventProperties
{
	// Constructor
	GtkMotionEventProperties(GdkEventMotion  *event);
};
typedef boost::shared_ptr<GtkMotionEventProperties> GtkMotionEventPropertiesPointer;


class InteractiveConstructorBase
{
	public:
		// Constructors and destructor
		InteractiveConstructorBase(OccSketchPointer parent_sketch, Handle(V3d_Viewer) current_viewer);
		virtual ~InteractiveConstructorBase() {;}
		
		// all derived classes must implement this method
		void CreateObject()=0; // Adds the created object to parent_sketch_

		// GUI event methods
		// each method returns true if the object being created is completely defined and false otherwise
		bool LeftButtonDown(MouseEventPropertiesPointer event_props) {return false;}
		bool MiddleButtonDown(MouseEventPropertiesPointer event_props) {return false;}
		bool MiddleButtonDown(MouseEventPropertiesPointer event_props) {return false;}

		bool LeftButtonUp(MouseEventPropertiesPointer event_props) {return false;}
		bool MiddleButtonUp(MouseEventPropertiesPointer event_props) {return false;}
		bool MiddleButtonUp(MouseEventPropertiesPointer event_props) {return false;}

		bool MouseMove(MoveEventPropertiesPointer event_props) {return false;}

	private:
		OccSketchPointer parent_sketch_;
		Handle(V3d_Viewer) current_viewer_;
};


#endif //InteractiveConstructorBaseH