#ifndef Line2DConstructorH
#define Line2DConstructorH

#include "InteractiveConstructorBase.h"

class Line2DConstructor : public InteractiveConstructorBase
{
	public:
		Line2DConstructor(QtSketchPointer parent_sketch);
		~Line2DConstructor() {parent_sketch_->ApplySelectionMask(All); parent_sketch_->ClearSelected();}  // turn off selection mask

		void CreateObject();

		// gui event methods
		bool MouseMove(MotionEventPropertiesPointer event_props);
		bool LeftButtonUp(MouseEventPropertiesPointer event_props);

	private:
		bool primitive_finished_;
		bool point1_defined_;
		double current_s_;
		double current_t_;
	
		Point2DPointer point1_;
		Point2DPointer point2_;
};

#endif //Line2DConstructorH



