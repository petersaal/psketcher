#ifndef ARK3DWIDGET_H
#define ARK3DWIDGET_H

#include "qoccviewwidget.h"

/*
extern "C" {
#include <Python.h>
}
*/

#include "../OpenCascadeBinding/OccSketch.h"
#include "../InteractiveConstructors/InteractiveConstructorBase.h"
#include "../InteractiveConstructors/Point2DConstructor.h"
#include "../InteractiveConstructors/Line2DConstructor.h"
#include "../InteractiveConstructors/DistancePoint2DConstructor.h"
#include "../InteractiveConstructors/AngleLine2DConstructor.h"

class Ark3DWidget : public QoccViewWidget 
{
	Q_OBJECT

	public:
		Ark3DWidget( const Handle_AIS_InteractiveContext& aContext = NULL, 
						QWidget *parent = NULL, 
						Qt::WindowFlags wflags = 0 );
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
		OccSketchPointer current_sketch_;
		InteractiveConstructorBase *interactive_primitive_;
	
	
	private: // methods
	
	
	
	public:

};

#endif // ARK3DWIDGET_H
