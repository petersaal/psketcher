#ifndef QtAngleLine2DH
#define QtAngleLine2DH

#include "QtPrimitiveBase.h"

class QtAngleLine2D : public QtPrimitiveBase, public AngleLine2D
{
	public:
		QtAngleLine2D (QGraphicsItem * parent, const Line2DPointer line1, const Line2DPointer line2, double angle);

		void Display() {return QtPrimitiveBase::Display();}
		bool IsSelected() {return QtPrimitiveBase::IsSelected();}
		void SetSelectable(bool input_selectable) {PrimitiveBase::SetSelectable(input_selectable); QtPrimitiveBase::SetSelectable(selectable_);}

		void UpdateDisplay();

		// Implement some of QGraphicsItem's methods
		virtual QRectF boundingRect() const;
		virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

	private:

};
typedef boost::shared_ptr<QtAngleLine2D> QtAngleLine2DPointer;



#endif //QtAngleLine2DH
