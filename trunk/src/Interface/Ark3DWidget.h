#ifndef ARK3DWIDGET_H
#define ARK3DWIDGET_H

/*
extern "C" {
#include <Python.h>
}
*/

#include <QGraphicsView>

#include "../QtBinding/QtSketch.h"
#include "../InteractiveConstructors/InteractiveConstructorBase.h"
#include "../InteractiveConstructors/Point2DConstructor.h"
#include "../InteractiveConstructors/Line2DConstructor.h"
#include "../InteractiveConstructors/DistancePoint2DConstructor.h"
#include "../InteractiveConstructors/AngleLine2DConstructor.h"

class QGraphicsScene;

class Ark3DWidget : public QGraphicsView
{
	Q_OBJECT

	public:
		Ark3DWidget(QGraphicsScene * scene, QWidget * parent = 0);
		~Ark3DWidget();
	
		void GenerateDefaultSketch();
	
	signals:
		void sketchActionFinished();
	
	public slots:
		void GenerateTestSketch();
		void ExecutePythonScript();
		void SolveConstraints();

		void select(); // overides the select solot for QoccViewWidget

		// Interactive constructor slots
		void MakeLine();
		void MakePolyLine();
		void MakeArc();
		void MakePoint();
		void MakeDistanceConstraint();
		void MakeAngleConstraint();
		void MakeTangentConstraint();

		void modelChanged();
	
	protected: // methods
		virtual void paintEvent        ( QPaintEvent* e );
		virtual void resizeEvent       ( QResizeEvent* e );
		virtual void mousePressEvent   ( QMouseEvent* e );
		virtual void mouseReleaseEvent ( QMouseEvent* e );
		virtual void mouseMoveEvent    ( QMouseEvent* e );
		virtual void wheelEvent        ( QWheelEvent* e );
		virtual void leaveEvent		   ( QEvent * e);
		virtual void mouseDoubleClickEvent ( QMouseEvent * event );
	
	private: // members
		QtSketchPointer current_sketch_;
		InteractiveConstructorBase *interactive_primitive_;
	
	
	private: // methods
	
	
	
	public:

};

#endif // ARK3DWIDGET_H
