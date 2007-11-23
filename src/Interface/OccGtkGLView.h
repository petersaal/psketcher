/***************************************************************************
  OccGtkGLView.h  -  description
  -------------------
begin                : |15- Feb|
author               : |Sharjith N.|
email                : |sharjith@gmail.com|
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is under the terms of the Open CASCADE Public License    *
 *                                                                         *
 ***************************************************************************/

#ifndef __OCCGTKGLVIEW_H__
#define __OCCGTKGLVIEW_H__

#include <gtk/gtk.h>
#include <glade/glade.h>
#include <gtk/gtkgl.h>

#ifdef G_OS_WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include <string>

#include <AIS_InteractiveContext.hxx>

#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>

#include <Geom_BSplineSurface.hxx>

#include <AIS_InteractiveObject.hxx>
#include <Graphic3d_NameOfMaterial.hxx>

#include <TopoDS_Shape.hxx>
#include <AIS_Shape.hxx>

#include <Handle_V3d_OrthographicView.hxx>
#include <Handle_V3d_PerspectiveView.hxx>
#include <V3d_OrthographicView.hxx>
#include <V3d_PerspectiveView.hxx>
#include <Aspect_Handle.hxx>
#include <Handle_AIS_Trihedron.hxx>

extern "C" {
#include <Python.h>
}

#include "../OpenCascadeBinding/OccSketch.h"
#include "../InteractiveConstructors/InteractiveConstructorBase.h"
#include "../InteractiveConstructors/Point2DConstructor.h"
#include "../InteractiveConstructors/Line2DConstructor.h"
#include "../InteractiveConstructors/DistancePoint2DConstructor.h"

/**
 *@author Sharjith
 */

class OccGtkGLView
{
    enum CurrentAction3d 
    { 
	CurAction3d_Nothing, 
	CurAction3d_DynamicZooming,
	CurAction3d_WindowZooming, 
	CurAction3d_DynamicPanning,
	CurAction3d_GlobalPanning, 
	CurAction3d_DynamicRotation 
    };
    enum ViewAction
    {
	ViewFitAllId,
	ViewFitAreaId,
	ViewZoomId,
	ViewPanId,
	ViewGlobalPanId,
	ViewFrontId, 
	ViewBackId, 
	ViewTopId, 
	ViewBottomId, 
	ViewLeftId, 
	ViewRightId,
	ViewAxoId, 
	ViewRotationId, 
	ViewResetId, 
	ViewHlrOffId, 
	ViewHlrOnId 
    };
public:
    OccGtkGLView();
    virtual ~OccGtkGLView();
    GtkDrawingArea* GetGtkDrawingArea() {  return GTK_DRAWING_AREA(theWidget); }
    void InitOCC(GtkDrawingArea* widget);
    void ReSize();
    void Draw();
    int	winX;
    int	winY;
    double	mouseX;
    double	mouseY;
    bool pointerIn;
private:
    bool ldown;
    bool mdown;
    bool rdown;

    GdkPoint  lPoint;
    GdkPoint  mPoint;
    GdkPoint  rPoint;

    int	winW;
    int	winH;

    GLdouble nRange;
    GLdouble xRot;
    GLdouble yRot;
    GLdouble zRot;
    GLdouble xTrans;
    GLdouble yTrans;
    GLdouble zTrans;
    GLfloat m_bkRed;
    GLfloat m_bkGreen;
    GLfloat m_bkBlue;
    bool m_antialias;

    GdkPoint DownPoint;
    bool ZoomON;
    bool PanON;
    bool RotON;

    bool gridOn;
    bool coordOn;

    bool multiViewActive;

    GtkDrawingArea* theWidget;	
    gchar* font_string;
    GLuint font_list_base;
    gint font_height;
    // Operations
public:
    void InitCursors();
    void ZoomView(); 
    void ZoomViewBy(const Standard_Integer& factor);
    void PanView();
    void RotateView();
    void GlobPan();    

    void TopView();
    void BottomView();
    void FrontView();
    void BackView();
    void LeftView();
    void RightView();
    void AxonView();
    void MultiView(bool);

    bool SnapShot(const string& fName);

    bool IsPlanarView() const;
    bool IsShadingMode() const;
    
    void FitAll();
    void ZoomWindow();
    void ResetView();
    void MapView();
    void Refresh();
    void HlrOn();
    void HlrOff();
    void ShowWaitCursor(gboolean on);

    void MoveTo(const int& x, const int& y);
    void LButtonDown( GdkEventButton  *event );
    void MButtonDown( GdkEventButton  *event );
    void RButtonDown( GdkEventButton  *event );
    void LButtonUp( GdkEventButton  *event );
    void MButtonUp( GdkEventButton  *event );
    void RButtonUp( GdkEventButton  *event );
    void MouseMove( GdkEventMotion  *event );
    void DragEvent( const int x, const int y, const int TheState );
    void InputEvent( const int x, const int y );
    void MoveEvent( const int x, const int y );
    void MultiMoveEvent( const int x, const int y );
    void MultiDragEvent( const int x, const int y, const int TheState );
    void MultiInputEvent( const int x, const int y );
    void Popup( GdkEventButton  *event );
    void DrawRectangle( const int MinX, const int MinY, const int MaxX, const int MaxY, const bool Draw );
    void ActivateCursor( const CurrentAction3d );  
    void NoActiveActions();

    void SelectionChanged();

    void opencascade_realize (void);
    void opencascade_init    (void);


		// methods for Ark3D 
		void SolveConstraints();
		void GenerateTestSketch();
		void ExecutePythonScript();
		void GenerateDefaultSketch();

		// Interactive constructor callbacks
		void MakeLine() {if(interactive_primitive_ == 0) interactive_primitive_ = new Line2DConstructor(current_sketch_, myView, myViewer);}
		void MakePolyLine() {;}
		void MakeArc() {;}
		void MakePoint() {if(interactive_primitive_ == 0) interactive_primitive_ = new Point2DConstructor(current_sketch_, myView, myViewer);}
		void MakeDistanceConstraint() {if(interactive_primitive_ == 0) interactive_primitive_ = new DistancePoint2DConstructor(current_sketch_, myView, myViewer);}
		void MakeAngleConstraint() {;}
		void MakeTangentConstraint() {;}

		bool convertToPlane(int x_screen, int y_screen,double& x,double& y,double& z);

public:
    bool isInitialized(void)
    {
	return opencascade_inited;
    }


    Handle(V3d_View) & getView(void)
    {
	return myView;
    }

    Handle(V3d_Viewer) & getViewer(void)
    {
	return myViewer;
    }

    Handle(AIS_InteractiveContext) & getContext(void)
    {
	return myContext;
    }

    CurrentAction3d getCurrentAction3d(void) const
    {
	return myCurrentMode;
    }

private:
    Handle(V3d_View)   myView;
    Handle(V3d_Viewer) myViewer;
    Handle(AIS_InteractiveContext) myContext;

		// Ark3D specific members
		OccSketchPointer current_sketch_;
		InteractiveConstructorBase *interactive_primitive_;

    bool opencascade_inited;

    bool is_mouse_button_pressed;
    guint mouse_button_press;

    CurrentAction3d                 myCurrentMode;
    Standard_Integer                myXmin;
    Standard_Integer                myYmin;
    Standard_Integer                myXmax;
    Standard_Integer                myYmax;
    Quantity_Factor                 myCurZoom;
    Standard_Boolean                myDegenerateModeIsOn;

     GdkCursor* zoom_cursor;
     GdkCursor* pan_cursor;
     GdkCursor* glob_pan_cursor;
     GdkCursor* rotate_cursor;
     GdkCursor* pick_cursor;
     GdkCursor* rect_cursor;
     GdkCursor* cross_cursor;
};

#endif
