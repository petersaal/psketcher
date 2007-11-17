#include "InteractiveConstructorBase.h"

class Point2DConstructor : public InteractiveConstructorBase
{
	public:
		Point2DConstructor(OccSketchPointer parent_sketch, Handle(V3d_View) current_view, Handle(V3d_Viewer) current_viewer);

		void CreateObject();

		// gui event methods
		bool LeftButtonUp(MouseEventPropertiesPointer event_props);

	private:
		bool primitive_finished_;
		double current_s_;
		double current_t_;
};

