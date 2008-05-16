#ifndef QtTangentEdge2DH
#define QtTangentEdge2DH

#include "QtPrimitiveBase.h"

class QtTangentEdge2D : public QtPrimitiveBase, public TangentEdge2D
{
	public:
		QtTangentEdge2D (QGraphicsItem * parent,
                       Edge2DBasePointer edge1, EdgePointNumber point_num_1, 
                       Edge2DBasePointer edge2, EdgePointNumber point_num_2);

		void Display() {return QtPrimitiveBase::Display();}
		bool IsSelected() {return QtPrimitiveBase::IsSelected();}
		void SetSelectable(bool input_selectable) {PrimitiveBase::SetSelectable(input_selectable); QtPrimitiveBase::SetSelectable(selectable_);}

		void UpdateDisplay();

	private:

};
typedef boost::shared_ptr<QtTangentEdge2D> QtTangentEdge2DPointer;


#endif //QtTangentEdge2DH