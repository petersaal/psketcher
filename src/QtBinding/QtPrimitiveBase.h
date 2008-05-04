#ifndef QtPrimitiveBaseH
#define QtPrimitiveBaseH

#include "QtCommon.h"
#include "../ConstraintSolver/Primitives.h"


// QtPrimitiveBase class
class QtPrimitiveBase
{
	public:
		QtPrimitiveBase(Handle(AIS_InteractiveContext) ais_context) {ais_context_ = ais_context;}
		
		// erase from the AIS_InteractiveContext and clear vector container
		virtual ~QtPrimitiveBase() { Erase();}

		virtual void Display();

		void Erase();

		// update coordinates of primitive
		virtual void UpdateDisplay();

		// return a TopoDS shape object of primitive if appropriate (if not overridden, it will return a Null TopoDS_Shape)
		virtual TopoDS_Shape GetTopoDS_Shape() {TopoDS_Shape null_shape; return null_shape;}

		virtual bool IsSelected();
		virtual void SetSelectable(bool selectable_);

	protected:
		Handle(AIS_InteractiveContext) ais_context_;
		std::vector<Handle(AIS_InteractiveObject)> ais_object_list_;
};


#endif //QtPrimitiveBaseH
