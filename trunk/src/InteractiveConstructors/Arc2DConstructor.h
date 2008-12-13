#ifndef Arc2DConstructorH
#define Arc2DConstructorH

#include "InteractiveConstructorBase.h"

class Arc2DConstructor : public InteractiveConstructorBase
{
	public:
		Arc2DConstructor(QtSketchPointer parent_sketch);

		void CreateObject();

		// gui event methods
		bool LeftButtonUp(MouseEventPropertiesPointer event_props);

	private:
		bool primitive_finished_;

		int current_point_number_;

		double point1_s_;
		double point1_t_;

		double point2_s_;
		double point2_t_;

		double point3_s_;
		double point3_t_;
};

#endif //Arc2DConstructorH




