#ifndef ParallelLine2DConstructorH
#define ParallelLine2DConstructorH

#include "InteractiveConstructorBase.h"

class ParallelLine2DConstructor : public InteractiveConstructorBase
{
	public:
		ParallelLine2DConstructor(QtSketchPointer parent_sketch);
		~ParallelLine2DConstructor() {parent_sketch_->ApplySelectionMask(All); parent_sketch_->ClearSelected();}  // turn off selection mask

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

#endif //ParallelLine2DConstructorH



