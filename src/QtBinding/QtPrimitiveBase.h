#ifndef QtPrimitiveBaseH
#define QtPrimitiveBaseH

#include <QGraphicsItem>
#include <QRectF>
#include "../ConstraintSolver/Primitives.h"

// QtPrimitiveBase class
class QtPrimitiveBase : public QGraphicsItem
{
	public:
		QtPrimitiveBase(QGraphicsItem * parent = 0);
		
		// erase from the AIS_InteractiveContext and clear vector container
		virtual ~QtPrimitiveBase() { Erase();}

		virtual void Display();

		void Erase();

		// update coordinates of primitive
		virtual void UpdateDisplay();

		virtual bool IsSelected();
		virtual void SetSelectable(bool selectable_);

		// some utility methods used to paint primitives
		void PaintPoint(QPainter *painter, const QStyleOptionGraphicsItem *option, double x, double y);
		QPolygonF GetArrowPolygon(double x1, double y1, double x2, double y2, double arrow_head_length, double arrow_head_width, bool double_arrow = false) const;
		QPainterPath GetArcArrowPath(double x_center, double y_center, double radius, double theta1, double theta2, double arrow_head_length, double arrow_head_width) const;

	protected:

};


#endif //QtPrimitiveBaseH
