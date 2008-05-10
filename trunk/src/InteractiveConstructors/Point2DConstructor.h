#ifndef Point2DConstructorH
#define Point2DConstructorH

#include "InteractiveConstructorBase.h"

class Point2DConstructor : public InteractiveConstructorBase
{
	public:
		Point2DConstructor(QtSketchPointer parent_sketch);

		void CreateObject();

		// gui event methods
		bool LeftButtonUp(MouseEventPropertiesPointer event_props);

	private:
		bool primitive_finished_;
		double current_s_;
		double current_t_;
};

#endif //Point2DConstructorH




