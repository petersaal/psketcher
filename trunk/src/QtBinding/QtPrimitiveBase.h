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

	protected:

};


#endif //QtPrimitiveBaseH
