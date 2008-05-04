#ifndef QtTangentEdge2DH
#define QtTangentEdge2DH

#include "QtPrimitiveBase.h"

class QtTangentEdge2D : public QtPrimitiveBase, public TangentEdge2D
{
	public:
		QtTangentEdge2D (Handle(AIS_InteractiveContext) ais_context,
                       Edge2DBasePointer edge1, EdgePointNumber point_num_1, 
                       Edge2DBasePointer edge2, EdgePointNumber point_num_2);

		void Display() {return QtPrimitiveBase::Display();}
		bool IsSelected() {return QtPrimitiveBase::IsSelected();}
		void SetSelectable(bool input_selectable) {PrimitiveBase::SetSelectable(input_selectable); QtPrimitiveBase::SetSelectable(selectable_);}

		void UpdateDisplay();

		void GenerateAISObject();

	private:
		TopoDS_Shape oc_shape1_;
		TopoDS_Shape oc_shape2_;

 		Handle(Geom_Plane) oc_plane_;
};
typedef boost::shared_ptr<QtTangentEdge2D> QtTangentEdge2DPointer;


#endif //QtTangentEdge2DH
