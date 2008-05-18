#ifndef QtDistancePoint2DH
#define QtDistancePoint2DH

#include "QtPrimitiveBase.h"

class QtDistancePoint2D : public QtPrimitiveBase, public DistancePoint2D
{
	public:
		QtDistancePoint2D (QGraphicsItem * parent, const Point2DPointer point1, const Point2DPointer point2, 
						   double distance);

		void Display() {return QtPrimitiveBase::Display();}
		bool IsSelected() {return QtPrimitiveBase::IsSelected();}
		void SetSelectable(bool input_selectable) {PrimitiveBase::SetSelectable(input_selectable); QtPrimitiveBase::SetSelectable(selectable_);}

		void UpdateDisplay();

		virtual QRectF boundingRect() const;
		virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

	private:

};
typedef boost::shared_ptr<QtDistancePoint2D> QtDistancePoint2DPointer;



#endif //QtDistancePoint2DH
