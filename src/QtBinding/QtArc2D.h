#ifndef QtArc2DH
#define QtArc2DH

#include <boost/enable_shared_from_this.hpp>

#include <QGraphicsProxyWidget>
#include "QtPrimitiveBase.h"

class QLineEdit;
class QtArc2DWidget;

// line class
class QtArc2D : public QtPrimitiveBase, public Arc2D, public boost::enable_shared_from_this<QtArc2D>
{
	public:
		QtArc2D (QGraphicsItem * parent, double s_center, double t_center, double theta_1, double theta_2, double radius, 
			      SketchPlanePointer sketch_plane, bool s_center_free = false, bool t_center_free = false, bool theta_1_free = false, bool theta_2_free = false, 
			      bool radius_free = false);
		QtArc2D (QGraphicsItem * parent,DOFPointer s_center, DOFPointer t_center, DOFPointer theta_1, DOFPointer theta_2, DOFPointer radius, SketchPlanePointer sketch_plane);

		void Display() {return QtPrimitiveBase::Display();}
		bool IsSelected() {return QtPrimitiveBase::IsSelected();}
		void SetSelectable(bool input_selectable) {PrimitiveBase::SetSelectable(input_selectable); QtPrimitiveBase::SetSelectable(input_selectable);}

		void UpdateDisplay();

		virtual QRectF boundingRect() const;
		virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

	private:
		QtArc2DWidget *radius_widget_;

};
typedef boost::shared_ptr<QtArc2D> QtArc2DPointer;


// class defining the line edit graphics item used by QtDistancePoint2D
class QtArc2DWidget : public QGraphicsProxyWidget
{
	Q_OBJECT

	public:
		QtArc2DWidget(QtArc2DPointer arc_primitive, QGraphicsItem *parent = 0);
		virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget);
		virtual bool event(QEvent *event);

		void UpdateGeometry(double scale);


	signals:
		void modelChanged();  // emitted when changes are applied

	private slots:
		void textChanged();
		void applyChanges();

	private:
		// methods

		// Parameters
		QtArc2DPointer arc_primitive_; 

		QLineEdit *radius_line_edit_;
};

#endif //QtArc2DH
