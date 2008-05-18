#ifndef QtLineH
#define QtLineH

#include "QtPrimitiveBase.h"

// line class
class QtLine : public QtPrimitiveBase, public Line
{
	public:
		QtLine (QGraphicsItem * parent, const PointPointer point1, const PointPointer point2);

		void Display() {return QtPrimitiveBase::Display();}
		bool IsSelected() {return QtPrimitiveBase::IsSelected();}
		void SetSelectable(bool input_selectable) {PrimitiveBase::SetSelectable(input_selectable); QtPrimitiveBase::SetSelectable(selectable_);}

		void UpdateDisplay();

		virtual QRectF boundingRect() const;
		virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

	private:

};
typedef boost::shared_ptr<QtLine> QtLinePointer;



#endif //QtLineH
