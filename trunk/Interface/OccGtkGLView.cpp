/***************************************************************************
  OccGtkGLView.cpp  -  description
  -------------------
begin                : |15 - Feb|
copyright            : (C) |2003| by |Sharjith N.|
email                : |sharjith_ssn@hotmail.com|
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "OccGtkGLView.h"

#include "Interface.h"
#include "Callbacks.h"
#include "Support.h"

#ifdef WNT
#include <WNT_Window.hxx>
#include <Graphic3d_WNTGraphicDevice.hxx>
#else
#include <Xw_Window.hxx>
#include <Graphic3d_GraphicDevice.hxx>
#endif

#ifdef G_OS_WIN32
#include <gdk/gdkwin32.h>
#endif

#ifdef G_OS_UNIX
#include <gdk/gdkx.h>
#endif

// for elastic bean selection
#define ValZWMin 1


OccGtkGLView::OccGtkGLView()
{   
    theWidget = 0;
    ldown = mdown = rdown = false;    

    winW = winH = 0;     

    m_antialias = false;

    ZoomON = false;
    PanON = false;
    RotON = false;

    gridOn = false;
    coordOn = false;
    pointerIn = true;

    multiViewActive = false;

    myCurZoom = 0;
    myXmin=0;
    myYmin=0;
    myXmax=0;
    myYmax=0;  

    font_string = "Sans 12";

    myCurrentMode = CurAction3d_Nothing;
    myDegenerateModeIsOn = Standard_True;
}

OccGtkGLView::~OccGtkGLView()
{
    g_free(zoom_cursor);
    g_free(pan_cursor);
    g_free(glob_pan_cursor);
    g_free(rotate_cursor);
    g_free(pick_cursor);
    g_free(cross_cursor);
    g_free(rect_cursor);
}

void OccGtkGLView::InitOCC(GtkDrawingArea* widget)
{    
    theWidget = widget;
    PangoFontDescription *font_desc;
    PangoFont *font;
    PangoFontMetrics *font_metrics;

    InitCursors();

    opencascade_realize();
    if(! myView.IsNull() )
    {
	myView->MustBeResized(); 
	myView->Redraw();
    }
}

void OccGtkGLView::ReSize()
{
    GdkGLContext *glcontext = gtk_widget_get_gl_context (GTK_WIDGET(theWidget));
    GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (GTK_WIDGET(theWidget));

    // OpenGL BEGIN
    if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext))
	return;

    //resize stuff
    if( !myView.IsNull() )
    {
	myView->MustBeResized();
	myView->Redraw();
    }

    gdk_gl_drawable_gl_end (gldrawable);
    // OpenGL END 
}

void OccGtkGLView::Draw()
{
    //draw stuff
    opencascade_init();
    if(!myView.IsNull())
	myView->Redraw();
}

void OccGtkGLView::InitCursors()
{
    // create required cursors
    GdkPixbuf* cur_image = create_pixbuf ("zoomcursor.png");
    zoom_cursor = gdk_cursor_new_from_pixbuf(gtk_widget_get_display(GTK_WIDGET(theWidget)), cur_image, 16, 16);
    cur_image = create_pixbuf ("pancursor.png");
    pan_cursor = gdk_cursor_new_from_pixbuf(gtk_widget_get_display(GTK_WIDGET(theWidget)), cur_image, 16, 16);
    cur_image = create_pixbuf ("globpancursor.png");
    glob_pan_cursor = gdk_cursor_new_from_pixbuf(gtk_widget_get_display(GTK_WIDGET(theWidget)), cur_image, 16, 16);
    cur_image = create_pixbuf ("rotatecursor.png");
    rotate_cursor = gdk_cursor_new_from_pixbuf(gtk_widget_get_display(GTK_WIDGET(theWidget)), cur_image, 10, 10);
    cur_image = create_pixbuf ("pickcursor.png");
    pick_cursor = gdk_cursor_new_from_pixbuf(gtk_widget_get_display(GTK_WIDGET(theWidget)), cur_image, 16, 16);
    cur_image = create_pixbuf ("zoomwincursor.png");
    rect_cursor = gdk_cursor_new_from_pixbuf(gtk_widget_get_display(GTK_WIDGET(theWidget)), cur_image, 18, 18);
    cur_image = create_pixbuf ("crosscursor.png");
    cross_cursor = gdk_cursor_new_from_pixbuf(gtk_widget_get_display(GTK_WIDGET(theWidget)), cur_image, 16, 16);
    /*arrow_cursor = gdk_cursor_new(NULL);*/
}

void OccGtkGLView::ZoomView()
{
    myCurrentMode = CurAction3d_DynamicZooming;
}

void OccGtkGLView::ZoomViewBy(const Standard_Integer& factor)
{
    Standard_Integer x1 = 0;
    Standard_Integer x2 = factor;
    Standard_Integer y1 = 0;
    Standard_Integer y2 = factor;
    myView->Zoom(x1, y1, x2, y2);
}

void OccGtkGLView::PanView()
{
    myCurrentMode = CurAction3d_DynamicPanning;
}

void OccGtkGLView::RotateView()
{
    myCurrentMode = CurAction3d_DynamicRotation;
}

void OccGtkGLView::GlobPan()
{
    // save the current zoom value
    myCurZoom = myView->Scale();
    // Do a Global Zoom
    myView->FitAll();
    // Set the mode
    myCurrentMode = CurAction3d_GlobalPanning;
}

void OccGtkGLView::TopView()
{ 
    myView->SetProj( V3d_Zpos );
}

void OccGtkGLView::BottomView()
{ 
    myView->SetProj( V3d_Zneg );
}

void OccGtkGLView::FrontView()
{	
    myView->SetProj( V3d_Yneg );
}

void OccGtkGLView::BackView()
{
    myView->SetProj( V3d_Ypos );
}

void OccGtkGLView::LeftView()
{ 	
    myView->SetProj( V3d_Xneg );
}

void OccGtkGLView::RightView()
{	
    myView->SetProj( V3d_Xpos );
}

void OccGtkGLView::AxonView()
{	
    myView->SetProj( V3d_XposYnegZpos );
}

void OccGtkGLView::MultiView(bool status)
{
    multiViewActive = status;
}

bool OccGtkGLView::SnapShot(const string& fName)
{    
    gdk_window_process_updates (GTK_WIDGET(theWidget)->window, TRUE);
    myView->Redraw();
    return myView->Dump( (Standard_CString)fName.data() );
}

bool OccGtkGLView::IsPlanarView() const
{    
    return false;
}

void OccGtkGLView::FitAll()
{
    myView->FitAll();
    myView->ZFitAll();
    myView->Redraw();
}

void OccGtkGLView::ZoomWindow()
{
    myCurrentMode = CurAction3d_WindowZooming;
}

void OccGtkGLView::ResetView()
{
    myView->Reset();
}

void OccGtkGLView::MapView()
{
    myView->DoMapping();
}

void OccGtkGLView::Refresh() 
{ 
    myView->Redraw();
}

void OccGtkGLView::HlrOn()
{
    ShowWaitCursor(true);
    myView->SetDegenerateModeOn();
    myDegenerateModeIsOn = Standard_True;
    ShowWaitCursor(false);  
}

void OccGtkGLView::HlrOff()
{
    ShowWaitCursor(true);
    myView->SetDegenerateModeOff();
    myDegenerateModeIsOn = Standard_False;
    ShowWaitCursor(false);
}

void OccGtkGLView::ShowWaitCursor(gboolean on)
{
    if(on)
	gdk_window_set_cursor(GTK_WIDGET(theWidget)->window, gdk_cursor_new(GDK_CLOCK));
    else
	gdk_window_set_cursor(GTK_WIDGET(theWidget)->window, NULL);
}

void OccGtkGLView::MoveTo(const int& x, const int& y)
{    
    if(!myContext.IsNull())
    {
	myContext->MoveTo( x, y, myView );
	myView->Redraw();
    }
}

void OccGtkGLView::LButtonDown( GdkEventButton  *event )
{
    //  save the current mouse coordinate in min
    myXmin = (Standard_Integer)event->x;
    myYmin = (Standard_Integer)event->y;
    myXmax = (Standard_Integer)event->x;
    myYmax = (Standard_Integer)event->y;

    if ( event->state & GDK_CONTROL_MASK )
    {
	myCurrentMode = CurAction3d_DynamicZooming;
    }
    else
    {
	switch ( myCurrentMode )
	{
	    case CurAction3d_Nothing:
		if ( event->state & GDK_SHIFT_MASK )
		    MultiDragEvent( myXmax, myYmax, -1 );
		else
		    DragEvent( myXmax, myYmax, -1 );
		break;
	    case CurAction3d_DynamicZooming:
		break;
	    case CurAction3d_WindowZooming:
		break;
	    case CurAction3d_DynamicPanning:
		break;
	    case CurAction3d_GlobalPanning:
		break;
	    case CurAction3d_DynamicRotation:
		if ( !myDegenerateModeIsOn )
		    myView->SetDegenerateModeOn();
		myView->StartRotation( (Standard_Integer)event->x, (Standard_Integer)event->y );
		break;
	    default:
		Standard_Failure::Raise( "incompatible Current Mode" );
		break;
	}
    }
    ActivateCursor( myCurrentMode );
}

void OccGtkGLView::MButtonDown( GdkEventButton  *event )
{
    if ( event->state & GDK_CONTROL_MASK )
	myCurrentMode = CurAction3d_DynamicPanning;
    ActivateCursor( myCurrentMode );

}

void OccGtkGLView::RButtonDown( GdkEventButton  *event )
{
    if ( event->state & GDK_CONTROL_MASK )
    {
	if ( !myDegenerateModeIsOn )
	    myView->SetDegenerateModeOn();
	myCurrentMode = CurAction3d_DynamicRotation;
	myView->StartRotation( (Standard_Integer)event->x, (Standard_Integer)event->y );
    }
    else
    {
	Popup( event );
    }
    ActivateCursor( myCurrentMode );
}

void OccGtkGLView::LButtonUp( GdkEventButton  *event )
{
    switch( myCurrentMode )
    {
	case CurAction3d_Nothing:
	    if ( event->x == myXmin && event->y == myYmin )
	    {
		// no offset between down and up --> selectEvent
		myXmax = (Standard_Integer)event->x;
		myYmax = (Standard_Integer)event->y;
		if ( event->state & GDK_SHIFT_MASK )
		    MultiInputEvent( (Standard_Integer)event->x, (Standard_Integer)event->y );
		else
		    InputEvent( (Standard_Integer)event->x, (Standard_Integer)event->y );
	    }
	    else
	    {
		DrawRectangle( myXmin, myYmin, myXmax, myYmax, Standard_False );
		myXmax = (Standard_Integer)event->x;
		myYmax = (Standard_Integer)event->y;
		if ( event->state & GDK_SHIFT_MASK )
		    MultiDragEvent( (Standard_Integer)event->x, (Standard_Integer)event->y, 1 );
		else
		    DragEvent( (Standard_Integer)event->x, (Standard_Integer)event->y, 1 );
	    }
	    break;
	case CurAction3d_DynamicZooming:
	    myCurrentMode = CurAction3d_Nothing;
	    NoActiveActions();
	    break;
	case CurAction3d_WindowZooming:
	    DrawRectangle( myXmin, myYmin, myXmax, myYmax, Standard_False );
	    DrawRectangle( myXmin, myYmin, myXmax, myYmax, Standard_True );//,LongDash);
	    myXmax = (Standard_Integer)event->x;
	    myYmax = (Standard_Integer)event->y;
	    if ( (abs( myXmin - myXmax ) > ValZWMin ) ||
		    (abs( myYmin - myYmax ) > ValZWMin ) )
		myView->WindowFitAll( myXmin, myYmin, myXmax, myYmax );
	    myCurrentMode = CurAction3d_Nothing;
	    NoActiveActions();
	    break;
	case CurAction3d_DynamicPanning:
	    myCurrentMode = CurAction3d_Nothing;
	    NoActiveActions();
	    break;
	case CurAction3d_GlobalPanning :
	    myView->Place( (Standard_Integer)event->x, (Standard_Integer)event->y, myCurZoom );
	    myCurrentMode = CurAction3d_Nothing;
	    NoActiveActions();
	    break;
	case CurAction3d_DynamicRotation:
	    myCurrentMode = CurAction3d_Nothing;
	    NoActiveActions();
	    break;
	default:
	    Standard_Failure::Raise(" incompatible Current Mode ");
	    break;
    }
    ActivateCursor( myCurrentMode );
}

void OccGtkGLView::MButtonUp( GdkEventButton  *event )
{
    myCurrentMode = CurAction3d_Nothing;
    ActivateCursor( myCurrentMode );
}

void OccGtkGLView::RButtonUp( GdkEventButton  *event )
{
    if ( myCurrentMode == CurAction3d_Nothing )
	Popup( event );
    else
    {
	ShowWaitCursor(true);
	// reset tyhe good Degenerated mode according to the strored one
	//   --> dynamic rotation may have change it
	if ( !myDegenerateModeIsOn )
	{
	    myView->SetDegenerateModeOff();
	    myDegenerateModeIsOn = Standard_False;
	}
	else
	{
	    myView->SetDegenerateModeOn();
	    myDegenerateModeIsOn = Standard_True;
	}
	ShowWaitCursor(false);
	myCurrentMode = CurAction3d_Nothing;
    }
    ActivateCursor( myCurrentMode );
}

void OccGtkGLView::MouseMove( GdkEventMotion  *event )
{
    if ( event->state & GDK_BUTTON1_MASK || event->state & GDK_BUTTON2_MASK || event->state & GDK_BUTTON3_MASK )
    {
	switch ( myCurrentMode )
	{
	    case CurAction3d_Nothing:
		myXmax = (Standard_Integer)event->x;
		myYmax = (Standard_Integer)event->y;
		DrawRectangle( myXmin, myYmin, myXmax, myYmax, Standard_False );
		if ( event->state & GDK_SHIFT_MASK )
		    MultiDragEvent( myXmax, myYmax, 0 );
		else
		    DragEvent( myXmax, myYmax, 0 );
		DrawRectangle( myXmin, myYmin, myXmax, myYmax, Standard_True );
		break;
	    case CurAction3d_DynamicZooming:
		myView->Zoom( myXmax, myYmax, (Standard_Integer)event->x, (Standard_Integer)event->y );
		myXmax = (Standard_Integer)event->x;
		myYmax = (Standard_Integer)event->y;
		break;
	    case CurAction3d_WindowZooming:
		myXmax = (Standard_Integer)event->x;
		myYmax = (Standard_Integer)event->y;
		DrawRectangle( myXmin, myYmin, myXmax, myYmax, Standard_False );
		DrawRectangle( myXmin, myYmin, myXmax, myYmax, Standard_True );
		break;
	    case CurAction3d_DynamicPanning:
		myView->Pan( (Standard_Integer)event->x - myXmax, myYmax - (Standard_Integer)event->y );
		myXmax = (Standard_Integer)event->x;
		myYmax = (Standard_Integer)event->y;
		break;
	    case CurAction3d_GlobalPanning:
		break;
	    case CurAction3d_DynamicRotation:
		myView->Rotation( (Standard_Integer)event->x, (Standard_Integer)event->y );
		myView->Redraw();
		break;
	    default:
		Standard_Failure::Raise( "incompatible Current Mode" );
		break;
	}
    }
    else
    {
	myXmax = (Standard_Integer)event->x;
	myYmax = (Standard_Integer)event->y;
	if ( event->state & GDK_SHIFT_MASK )
	    MultiMoveEvent( (Standard_Integer)event->x, (Standard_Integer)event->y );
	else
	    MoveEvent( (Standard_Integer)event->x, (Standard_Integer)event->y );
    }
}

void OccGtkGLView::DragEvent( const int x, const int y, const int TheState )
{
    // TheState == -1  button down
    // TheState ==  0  move
    // TheState ==  1  button up

    static Standard_Integer theButtonDownX = 0;
    static Standard_Integer theButtonDownY = 0;

    if ( TheState == -1 )
    {
	theButtonDownX = x;
	theButtonDownY = y;
    }

    if ( TheState == 0 )
    {
	myContext->Select( theButtonDownX, theButtonDownY, x, y, myView );
	SelectionChanged();
    }
}

void OccGtkGLView::InputEvent( const int x, const int y )
{
    myContext->Select();
    SelectionChanged();
}

void OccGtkGLView::MoveEvent( const int x, const int y ) 
{
    myContext-> MoveTo( x, y, myView );
}

void OccGtkGLView::MultiMoveEvent( const int x, const int y )
{
    myContext->MoveTo( x, y, myView );
}

void OccGtkGLView::MultiDragEvent( const int x, const int y, const int TheState )
{
    static Standard_Integer theButtonDownX = 0;
    static Standard_Integer theButtonDownY = 0;

    if ( TheState == -1 )
    {
	theButtonDownX = x;
	theButtonDownY = y;
    }
    if ( TheState == 0 )
    {
	myContext->ShiftSelect( theButtonDownX, theButtonDownY, x, y, myView );
	SelectionChanged();
    }
}

void OccGtkGLView::MultiInputEvent( const int x, const int y )
{
    myContext->ShiftSelect();
    SelectionChanged();
}

void OccGtkGLView::Popup( GdkEventButton  *event )
{
    GtkWidget* popup;
    GtkWidget* menuitem;
    bool OneOrMoreInShading = false;
    bool OneOrMoreInWireframe = false;
    int numSel = myContext->NbSelected();
    if ( numSel )
    {
	popup = create_popup_object();	
	for ( myContext->InitCurrent(); myContext->MoreCurrent(); myContext->NextCurrent() )
	{
	    if ( myContext->IsDisplayed( myContext->Current(), 1 ) )
		OneOrMoreInShading = true;
	    if ( myContext->IsDisplayed( myContext->Current(), 0 ) )
		OneOrMoreInWireframe = true;
	}
	menuitem = lookup_widget(GTK_WIDGET(popup), "wireframe1");
	gtk_widget_set_sensitive(GTK_WIDGET(menuitem), OneOrMoreInShading);
	menuitem = lookup_widget(GTK_WIDGET(popup), "shading1");
	gtk_widget_set_sensitive(GTK_WIDGET(menuitem), OneOrMoreInWireframe);
	menuitem = lookup_widget(GTK_WIDGET(popup), "color1");
	gtk_widget_set_sensitive(GTK_WIDGET(menuitem), true);
	menuitem = lookup_widget(GTK_WIDGET(popup), "material1");
	gtk_widget_set_sensitive(GTK_WIDGET(menuitem), OneOrMoreInShading);    
	menuitem = lookup_widget(GTK_WIDGET(popup), "transparency1");
	gtk_widget_set_sensitive(GTK_WIDGET(menuitem), OneOrMoreInShading);	
	menuitem = lookup_widget(GTK_WIDGET(popup), "delete1");
	gtk_widget_set_sensitive(GTK_WIDGET(menuitem), true);
	
	GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(theWidget));
	menuitem = glade_xml_get_widget(MainWindowXML, "material_toolbutton");
	gtk_widget_set_sensitive(GTK_WIDGET(menuitem), OneOrMoreInShading);
	
	
    }
    else
    {	
	popup = create_popup_bg();
    }  
    gtk_menu_popup (GTK_MENU (popup), NULL, NULL, NULL, NULL, event->button, event->time);
}

void OccGtkGLView::DrawRectangle( const int MinX, const int MinY, const int MaxX, const int MaxY, const bool Draw )
{
    GdkGC* gc = gdk_gc_new (GTK_WIDGET(theWidget)->window);
    GdkColor color;
    color.red = 65535;
    color.green = 65535;
    color.blue = 65535;
    gdk_gc_set_function(gc, GDK_XOR);
    gdk_gc_set_rgb_fg_color (gc, &color);

    static		int StoredMinX, StoredMaxX, StoredMinY, StoredMaxY;
    static		bool m_IsVisible;

    GdkRectangle aRect;
    if ( m_IsVisible && !Draw) // move or up  : erase at the old position
    {
	aRect.x = StoredMinX;
	aRect.y = StoredMinY;
	aRect.width = abs(StoredMaxX-StoredMinX);
	aRect.height = abs(StoredMaxY-StoredMinY);
	gdk_draw_rectangle(GTK_WIDGET(theWidget)->window, gc, FALSE, aRect.x, aRect.y, aRect.width, aRect.height);
	m_IsVisible = FALSE;
    }

    StoredMinX = (MinX < MaxX) ? MinX: MaxX ;
    StoredMinY = (MinY < MaxY) ? MinY: MaxY ;
    StoredMaxX = (MinX > MaxX) ? MinX: MaxX ;
    StoredMaxY = (MinY > MaxY) ? MinY: MaxY ;

    if (Draw) // move : draw
    {
	aRect.x = StoredMinX;
	aRect.y = StoredMinY;
	aRect.width = abs(StoredMaxX-StoredMinX);
	aRect.height = abs(StoredMaxY-StoredMinY);
	gdk_draw_rectangle(GTK_WIDGET(theWidget)->window, gc, FALSE, aRect.x, aRect.y, aRect.width, aRect.height);	
	m_IsVisible = TRUE;
    }
}

void OccGtkGLView::ActivateCursor( const CurrentAction3d mode)
{
    switch( mode )
    {
	case CurAction3d_DynamicPanning:
	    gdk_window_set_cursor (GTK_WIDGET(theWidget)->window, pan_cursor);
	    break;
	case CurAction3d_DynamicZooming:
	    gdk_window_set_cursor (GTK_WIDGET(theWidget)->window, zoom_cursor);
	    break;
	case CurAction3d_DynamicRotation:
	    gdk_window_set_cursor (GTK_WIDGET(theWidget)->window, rotate_cursor);
	    break;
	case CurAction3d_GlobalPanning:
	    gdk_window_set_cursor (GTK_WIDGET(theWidget)->window, glob_pan_cursor);
	    break;
	case CurAction3d_WindowZooming:
	    gdk_window_set_cursor (GTK_WIDGET(theWidget)->window, rect_cursor);
	    break;
	case CurAction3d_Nothing:
	default:
	    gdk_window_set_cursor (GTK_WIDGET(theWidget)->window, NULL);
	    break;
    }
}

//
void OccGtkGLView::opencascade_realize(void)
{
    glong windowHandle;
    GdkWindow* gdkwin = GTK_WIDGET(theWidget)->window;

#ifdef G_OS_UNIX
    std::string display;
    Standard_CString aDisplay;

    Handle(Xw_Window) myWindow;
    Handle(Graphic3d_GraphicDevice) myDevice;

    windowHandle = (glong) GDK_WINDOW_XID(gdkwin);

    display  = g_getenv("DISPLAY");
    aDisplay = (char *) display.c_str(); 

    short hi, lo;
    lo = (short) windowHandle;
    hi = (short) (windowHandle >> 16);

    myDevice = new Graphic3d_GraphicDevice(aDisplay);
    myWindow = new Xw_Window(myDevice, (int)hi, (int)lo);
#endif

#ifdef G_OS_WIN32
    Handle(WNT_Window) myWindow;  
    Handle(Graphic3d_WNTGraphicDevice) myDevice;

    windowHandle = (glong) GDK_WINDOW_HWND(gdkwin);

    short hi, lo;
    lo = (short) windowHandle;
    hi = (short) (windowHandle >> 16);

    myDevice = new Graphic3d_WNTGraphicDevice();
    myWindow = new WNT_Window(myDevice, (int)hi, (int)lo);
#endif

    TCollection_ExtendedString Name("Viewer1");

    TCollection_AsciiString Domain("");

    //myViewer = new V3d_Viewer(myDevice,Name.ToExtString());
    myViewer = new V3d_Viewer(myDevice,Name.ToExtString(),Domain.ToCString(),1000.0,V3d_XposYnegZpos,Quantity_NOC_GRAY25,V3d_ZBUFFER,V3d_GOURAUD,V3d_WAIT,Standard_True,V3d_TEX_ALL);


    //myView = myViewer->CreateView();
    myView= new V3d_OrthographicView(myViewer);

    myView->SetWindow(myWindow);    

    myViewer->Init();
    myViewer->SetDefaultLights();
    myViewer->SetLightOn();

    myContext = new AIS_InteractiveContext(myViewer);

    if ( !myWindow->IsMapped() )
	myWindow->Map();

    myView->SetBackgroundColor(Quantity_NOC_GRAY25);
}

void OccGtkGLView::opencascade_init(void)
{
    if(!opencascade_inited)
    {
	myView->MustBeResized();

	myViewer->InitActiveViews();

	myView->Redraw();

	opencascade_inited = true;
    }
}

void OccGtkGLView::NoActiveActions()
{
    GtkWidget* toggle;
	
	GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(theWidget));
	
    toggle = glade_xml_get_widget(MainWindowXML, "dyn_zoom_toolbutton");
    gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(toggle), false);
    toggle = glade_xml_get_widget(MainWindowXML, "dyn_pan_toolbutton");
    gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(toggle), false);
    toggle = glade_xml_get_widget(MainWindowXML, "dyn_rotate_toolbutton");
    gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(toggle), false);
    toggle = glade_xml_get_widget(MainWindowXML,"glob_pan_toolbutton");
    gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(toggle), false);
    toggle = glade_xml_get_widget(MainWindowXML, "zoom_win_toolbutton");
    gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(toggle), false);
    ActivateCursor(CurAction3d_Nothing);
}

void OccGtkGLView::SelectionChanged()
{
    GtkWidget* menuitem;
	GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(theWidget));
	
    bool OneOrMoreInShading = false;
    bool OneOrMoreInWireframe = false;
    int numSel = myContext->NbSelected();
    if ( numSel )
    {
		for ( myContext->InitCurrent(); myContext->MoreCurrent(); myContext->NextCurrent() )
		{
			if ( myContext->IsDisplayed( myContext->Current(), 1 ) )
			OneOrMoreInShading = true;
			if ( myContext->IsDisplayed( myContext->Current(), 0 ) )
			OneOrMoreInWireframe = true;
		}
		
		menuitem = glade_xml_get_widget(MainWindowXML, "material_toolbutton");
		gtk_widget_set_sensitive(GTK_WIDGET(menuitem), OneOrMoreInShading);
		menuitem = glade_xml_get_widget(MainWindowXML, "color_toolbutton");
		gtk_widget_set_sensitive(GTK_WIDGET(menuitem), true);
		menuitem = glade_xml_get_widget(MainWindowXML, "transparency_toolbutton");
		gtk_widget_set_sensitive(GTK_WIDGET(menuitem), OneOrMoreInShading);
    }
    else
    {	
		menuitem = glade_xml_get_widget(MainWindowXML, "material_toolbutton");
		gtk_widget_set_sensitive(GTK_WIDGET(menuitem), false);
		menuitem = glade_xml_get_widget(MainWindowXML, "color_toolbutton");
		gtk_widget_set_sensitive(GTK_WIDGET(menuitem), false);
		menuitem = glade_xml_get_widget(MainWindowXML, "transparency_toolbutton");
		gtk_widget_set_sensitive(GTK_WIDGET(menuitem), false);
    }
    menuitem = glade_xml_get_widget(MainWindowXML, "export_iges2");
    gtk_widget_set_sensitive(GTK_WIDGET(menuitem), (gboolean)numSel);
    menuitem = glade_xml_get_widget(MainWindowXML, "export_iges_toolbutton");
    gtk_widget_set_sensitive(GTK_WIDGET(menuitem), (gboolean)numSel);

    menuitem = glade_xml_get_widget(MainWindowXML, "export_step2");
    gtk_widget_set_sensitive(GTK_WIDGET(menuitem), (gboolean)numSel);
    menuitem = glade_xml_get_widget(MainWindowXML, "export_step_toolbutton");
    gtk_widget_set_sensitive(GTK_WIDGET(menuitem), (gboolean)numSel);

    menuitem = glade_xml_get_widget(MainWindowXML, "export_brep2");
    gtk_widget_set_sensitive(GTK_WIDGET(menuitem), (gboolean)numSel);
    menuitem = glade_xml_get_widget(MainWindowXML, "export_brep_toolbutton");
    gtk_widget_set_sensitive(GTK_WIDGET(menuitem), (gboolean)numSel);

    menuitem = glade_xml_get_widget(MainWindowXML, "export_csfdb2");
    gtk_widget_set_sensitive(GTK_WIDGET(menuitem), (gboolean)numSel);
    menuitem = glade_xml_get_widget(MainWindowXML, "export_csfdb_toolbutton");
    gtk_widget_set_sensitive(GTK_WIDGET(menuitem), (gboolean)numSel);

    menuitem = glade_xml_get_widget(MainWindowXML, "export_stl1");
    gtk_widget_set_sensitive(GTK_WIDGET(menuitem), (gboolean)numSel);
    menuitem = glade_xml_get_widget(MainWindowXML, "export_stl_toolbutton");
    gtk_widget_set_sensitive(GTK_WIDGET(menuitem), (gboolean)numSel);

    menuitem = glade_xml_get_widget(MainWindowXML, "export_vrml2");
    gtk_widget_set_sensitive(GTK_WIDGET(menuitem), (gboolean)numSel);
    menuitem = glade_xml_get_widget(MainWindowXML, "export_vrml_toolbutton");
    gtk_widget_set_sensitive(GTK_WIDGET(menuitem), (gboolean)numSel);
}

