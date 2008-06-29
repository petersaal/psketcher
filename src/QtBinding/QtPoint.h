#ifndef QtPointH
#define QtPointH

#include "QtPrimitiveBase.h"

// point class
class QtPoint : public QtPrimitiveBase, public Point
{
	public:
		QtPoint (QGraphicsItem * parent, double x, double y, double z, bool x_free = false, bool y_free = false, bool z_free = false);

		void Display() {return QtPrimitiveBase::Display();}
		bool IsSelected() {return QtPrimitiveBase::IsSelected();}
		void SetSelectable(bool input_selectable) {PrimitiveBase::SetSelectable(input_selectable); QtPrimitiveBase::SetSelectable(input_selectable);}

		void UpdateDisplay();

		virtual QRectF boundingRect() const;
		virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

	private:

};
typedef boost::shared_ptr<QtPoint> QtPointPointer;


#endif //QtPointH
