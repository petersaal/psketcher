#ifndef AngleLine2DConstructorH
#define AngleLine2DConstructorH

#include "InteractiveConstructorBase.h"

class AngleLine2DConstructor : public InteractiveConstructorBase
{
	public:
		AngleLine2DConstructor(QtSketchPointer parent_sketch);
		~AngleLine2DConstructor() {parent_sketch_->ApplySelectionMask(All); parent_sketch_->ClearSelected();}  // turn off selection mask

		void CreateObject();

		// gui event methods
		bool MouseMove(MotionEventPropertiesPointer event_props);
		bool LeftButtonUp(MouseEventPropertiesPointer event_props);

	private:
		bool primitive_finished_;
		bool line1_defined_;
		double current_s_;
		double current_t_;
	
		Line2DPointer line1_;
		Line2DPointer line2_;
};

#endif //AngleLine2DConstructorH



