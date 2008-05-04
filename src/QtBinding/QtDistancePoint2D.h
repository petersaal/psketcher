#ifndef QtDistancePoint2DH
#define QtDistancePoint2DH

#include "QtPrimitiveBase.h"

class QtDistancePoint2D : public QtPrimitiveBase, public DistancePoint2D
{
	public:
		QtDistancePoint2D (Handle(AIS_InteractiveContext) ais_context,
																			   const Point2DPointer point1, const Point2DPointer point2, 
																				 double distance);

		void Display() {return QtPrimitiveBase::Display();}
		bool IsSelected() {return QtPrimitiveBase::IsSelected();}
		void SetSelectable(bool input_selectable) {PrimitiveBase::SetSelectable(input_selectable); QtPrimitiveBase::SetSelectable(selectable_);}

		void UpdateDisplay();

		void GenerateAISObject();

	private:
		gp_Pnt oc_point1_;
		gp_Pnt oc_point2_;

		TopoDS_Vertex oc_shape1_;
		TopoDS_Vertex oc_shape2_;

 		Handle(Geom_Plane) oc_plane_;
};
typedef boost::shared_ptr<QtDistancePoint2D> QtDistancePoint2DPointer;



#endif //QtDistancePoint2DH
