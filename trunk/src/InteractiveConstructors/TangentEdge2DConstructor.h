#ifndef TangentEdge2DConstructorH
#define TangentEdge2DConstructorH

#include "InteractiveConstructorBase.h"

class TangentEdge2DConstructor : public InteractiveConstructorBase
{
	public:
		TangentEdge2DConstructor(QtSketchPointer parent_sketch);
		~TangentEdge2DConstructor() {parent_sketch_->ApplySelectionMask(All); parent_sketch_->ClearSelected();}  // turn off selection mask

		void CreateObject();

		// gui event methods
		bool LeftButtonUp(MouseEventPropertiesPointer event_props);

	private:
		bool primitive_finished_;
		bool edge1_defined_;	

		Edge2DBasePointer edge1_;
		EdgePointNumber edge1_point_number_;
		Edge2DBasePointer edge2_;
		EdgePointNumber edge2_point_number_;
};

#endif //TangentEdge2DConstructorH



