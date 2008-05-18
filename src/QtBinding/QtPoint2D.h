#ifndef QtPoint2DH
#define QtPoint2DH

#include "QtPrimitiveBase.h"

// point class
class QtPoint2D : public QtPrimitiveBase, public Point2D
{
	public:
		QtPoint2D (QGraphicsItem * parent,double s, double t, SketchPlanePointer sketch_plane, bool s_free = false, bool t_free = false);
		QtPoint2D (QGraphicsItem * parent, DOFPointer s, DOFPointer t, SketchPlanePointer sketch_plane);

		void Display() {return QtPrimitiveBase::Display();}
		bool IsSelected() {return QtPrimitiveBase::IsSelected();}
		void SetSelectable(bool input_selectable) {PrimitiveBase::SetSelectable(input_selectable); QtPrimitiveBase::SetSelectable(selectable_);}
		void UpdateDisplay();

		virtual QRectF boundingRect() const;
		virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

	private:

};
typedef boost::shared_ptr<QtPoint2D> QtPoint2DPointer;


#endif //QtPoint2DH
