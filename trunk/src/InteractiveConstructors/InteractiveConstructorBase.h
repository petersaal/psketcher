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

#ifndef InteractiveConstructorBaseH
#define InteractiveConstructorBaseH

#include <boost/shared_ptr.hpp>

#include "../QtBinding/QtSketch.h"

enum MouseButton {LeftButton, MiddleButton, RightButton, Button4, Button5, OtherButton};
enum MouseButtonEventType {ButtonPress, ButtonRelease};

class MouseEventProperties
{
	public:
		// Accessor methods used to retrieve values (the constructor will define all of the values)
		double GetXPosition() {return global_x_position_;}
		double GetYPosition() {return global_y_position_;}
		double GetZPosition() {return global_z_position_;}

		double GetXScreenPosition() {return screen_x_position_;}
		double GetYScreenPosition() {return screen_y_position_;}
		
		bool ControlPressed() {return control_;}
		bool ShiftPressed() {return shift_;}
		bool AltPressed() {return alt_;}

		MouseButton GetButton() {return mouse_button_;}
		MouseButtonEventType GetEventType() {return mouse_event_type_;}

	protected:
		double screen_x_position_;
		double screen_y_position_;

		double global_x_position_;
		double global_y_position_;
		double global_z_position_;
		
		bool control_;
		bool shift_;
		bool alt_;

		MouseButton mouse_button_;
		MouseButtonEventType mouse_event_type_;
};
typedef std::auto_ptr<MouseEventProperties> MouseEventPropertiesPointer;



class MotionEventProperties
{
	public:
		// Accessor methods used to retrieve values (the constructor will define all of the values)
		double GetXPosition() {return global_x_position_;}
		double GetYPosition() {return global_y_position_;}
		double GetZPosition() {return global_z_position_;}

		double GetXScreenPosition() {return screen_x_position_;}
		double GetYScreenPosition() {return screen_y_position_;}
		
		bool ControlPressed() {return control_;}
		bool ShiftPressed() {return shift_;}
		bool AltPressed() {return alt_;}

		bool LeftButtonPressed() {return left_button_;}
		bool MiddleButtonPressed() {return middle_button_;}
		bool RightButtonPressed() {return right_button_;}
		bool Button4Pressed() {return button_4_;}
		bool Button5Pressed() {return button_5_;}

	protected:
		double screen_x_position_;
		double screen_y_position_;
		
		double global_x_position_;
		double global_y_position_;
		double global_z_position_;

		bool control_;
		bool shift_;
		bool alt_;

		bool left_button_;
		bool middle_button_;
		bool right_button_;
		bool button_4_;
		bool button_5_;
};
typedef std::auto_ptr<MotionEventProperties> MotionEventPropertiesPointer;

class InteractiveConstructorBase
{
	public:
		// Constructors and destructor
		InteractiveConstructorBase(QtSketchPointer parent_sketch);
		virtual ~InteractiveConstructorBase() {;}
		
		// all derived classes must implement this method
		virtual void CreateObject()=0; // Adds the created object to parent_sketch_

		// GUI event methods
		// each method returns true if the object being created is completely defined and false otherwise
		virtual bool LeftButtonDown(MouseEventPropertiesPointer event_props) {return false;}
		virtual bool MiddleButtonDown(MouseEventPropertiesPointer event_props) {return false;}
		virtual bool RightButtonDown(MouseEventPropertiesPointer event_props) {return false;}

		virtual bool LeftButtonUp(MouseEventPropertiesPointer event_props) {return false;}
		virtual bool MiddleButtonUp(MouseEventPropertiesPointer event_props) {return false;}
		virtual bool RightButtonUp(MouseEventPropertiesPointer event_props) {return false;}

		virtual bool MouseMove(MotionEventPropertiesPointer event_props) {return false;}

	protected:
		QtSketchPointer parent_sketch_;
};


#endif //InteractiveConstructorBaseH
