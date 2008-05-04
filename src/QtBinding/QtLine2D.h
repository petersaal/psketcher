#ifndef QtLine2DH
#define QtLine2DH

#include "QtPrimitiveBase.h"

// line class
class QtLine2D : public QtPrimitiveBase, public Line2D
{
	public:
		QtLine2D (Handle(AIS_InteractiveContext) ais_context, const Point2DPointer point1, const Point2DPointer point2, SketchPlanePointer sketch_plane);

		void Display() {return QtPrimitiveBase::Display();}
		bool IsSelected() {return QtPrimitiveBase::IsSelected();}
		void SetSelectable(bool input_selectable) {PrimitiveBase::SetSelectable(input_selectable); QtPrimitiveBase::SetSelectable(selectable_);}

		void UpdateDisplay();

		TopoDS_Shape GetTopoDS_Shape();

	private:
		Handle(Geom_CartesianPoint) oc_point1_;
		Handle(Geom_CartesianPoint) oc_point2_;
};
typedef boost::shared_ptr<QtLine2D> QtLine2DPointer;



#endif //QtLine2DH
