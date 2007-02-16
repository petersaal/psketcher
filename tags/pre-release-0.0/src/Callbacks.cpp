#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "Callbacks.h"
#include "Interface.h"
#include "Support.h"
#include "OccGtkGLView.h"

#include <AIS_InteractiveObject.hxx>
#include <AIS_Trihedron.hxx>
#include <Geom_Axis2Placement.hxx>

static bool firstTime = true;

TopoDS_Shape
MakeBottle(const Standard_Real myWidth , const Standard_Real myHeight , const Standard_Real myThickness);

    gboolean
on_MainWindow_configure_event          (GtkWidget       *widget,
	GdkEventConfigure *event,
	gpointer         user_data)
{
    return FALSE;
}


    void
on_MainWindow_realize                  (GtkWidget       *widget,
	gpointer         user_data)
{
    GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(widget));
	
    GtkWidget* menuitem;
    menuitem = glade_xml_get_widget(MainWindowXML, "export_iges2");
    gtk_widget_set_sensitive(GTK_WIDGET(menuitem), false);

    menuitem = glade_xml_get_widget(MainWindowXML, "export_step2");
    gtk_widget_set_sensitive(GTK_WIDGET(menuitem), false);

    menuitem = glade_xml_get_widget(MainWindowXML, "export_brep2");
    gtk_widget_set_sensitive(GTK_WIDGET(menuitem), false);

    menuitem = glade_xml_get_widget(MainWindowXML, "export_csfdb2");
    gtk_widget_set_sensitive(GTK_WIDGET(menuitem), false);

    menuitem = glade_xml_get_widget(MainWindowXML, "export_stl1");
    gtk_widget_set_sensitive(GTK_WIDGET(menuitem), false);

    menuitem = glade_xml_get_widget(MainWindowXML, "export_vrml2");
    gtk_widget_set_sensitive(GTK_WIDGET(menuitem), false);

    menuitem = glade_xml_get_widget(MainWindowXML, "color_toolbutton");
    gtk_widget_set_sensitive(GTK_WIDGET(menuitem), false);

    menuitem = glade_xml_get_widget(MainWindowXML, "material_toolbutton");
    gtk_widget_set_sensitive(GTK_WIDGET(menuitem), false);

    menuitem = glade_xml_get_widget(MainWindowXML, "transparency_toolbutton");
    gtk_widget_set_sensitive(GTK_WIDGET(menuitem), false);
}


    gboolean
on_MainWindow_destroy_event            (GtkWidget       *widget,
	GdkEvent        *event,
	gpointer         user_data)
{
    return FALSE;
}


    gboolean
on_MainWindow_delete_event             (GtkWidget       *widget,
	GdkEvent        *event,
	gpointer         user_data)
{
    GtkWidget* dialog = gtk_message_dialog_new (GTK_WINDOW(widget),
	    GTK_DIALOG_DESTROY_WITH_PARENT,
	    GTK_MESSAGE_QUESTION,
	    GTK_BUTTONS_YES_NO,
	    "Do You Want To Quit?");
    int result = gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
    if(result == GTK_RESPONSE_YES)
	return FALSE;
    else
	return TRUE;
}


#include "Translate.h"
    void
on_import_iges1_activate               (GtkMenuItem     *menuitem,
	gpointer         user_data)
{    
    GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(menuitem));
	GtkWidget* mainwindow = glade_xml_get_widget(MainWindowXML, "MainWindow");
    Translate* anTrans = new Translate( mainwindow);
    Handle(AIS_InteractiveContext) context = theViewer->getContext();
    bool status = anTrans->importModel( Translate::FormatIGES, context );   
}


    void
on_import_step1_activate               (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(menuitem));
	GtkWidget* mainwindow = glade_xml_get_widget(MainWindowXML, "MainWindow");
    Translate* anTrans = new Translate( mainwindow);
    Handle(AIS_InteractiveContext) context = theViewer->getContext();
    bool status = anTrans->importModel( Translate::FormatSTEP, context );
}


    void
on_import_brep1_activate               (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(menuitem));
	GtkWidget* mainwindow = glade_xml_get_widget(MainWindowXML, "MainWindow");
    Translate* anTrans = new Translate( mainwindow);
    Handle(AIS_InteractiveContext) context = theViewer->getContext();
    bool status = anTrans->importModel( Translate::FormatBREP, context );
}


    void
on_import_csfdb1_activate              (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(menuitem));
	GtkWidget* mainwindow = glade_xml_get_widget(MainWindowXML, "MainWindow");
    Translate* anTrans = new Translate( mainwindow);
    Handle(AIS_InteractiveContext) context = theViewer->getContext();
    bool status = anTrans->importModel( Translate::FormatCSFDB, context );
}


    void
on_export_iges2_activate               (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(menuitem));
	GtkWidget* mainwindow = glade_xml_get_widget(MainWindowXML, "MainWindow");
    Translate* anTrans = new Translate( mainwindow);
    Handle(AIS_InteractiveContext) context = theViewer->getContext();
    bool status = anTrans->exportModel( Translate::FormatIGES, context );
}


    void
on_export_step2_activate               (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(menuitem));
	GtkWidget* mainwindow = glade_xml_get_widget(MainWindowXML, "MainWindow");
    Translate* anTrans = new Translate( mainwindow);
    Handle(AIS_InteractiveContext) context = theViewer->getContext();
    bool status = anTrans->exportModel( Translate::FormatSTEP, context );
}


    void
on_export_brep2_activate               (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(menuitem));
	GtkWidget* mainwindow = glade_xml_get_widget(MainWindowXML, "MainWindow");
    Translate* anTrans = new Translate( mainwindow);
    Handle(AIS_InteractiveContext) context = theViewer->getContext();
    bool status = anTrans->exportModel( Translate::FormatBREP, context );
}


    void
on_export_csfdb2_activate              (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(menuitem));
	GtkWidget* mainwindow = glade_xml_get_widget(MainWindowXML, "MainWindow");
    Translate* anTrans = new Translate( mainwindow);
    Handle(AIS_InteractiveContext) context = theViewer->getContext();
    bool status = anTrans->exportModel( Translate::FormatCSFDB, context );
}


    void
on_export_stl1_activate                (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(menuitem));
	GtkWidget* mainwindow = glade_xml_get_widget(MainWindowXML, "MainWindow");
    Translate* anTrans = new Translate( mainwindow);
    Handle(AIS_InteractiveContext) context = theViewer->getContext();
    bool status = anTrans->exportModel( Translate::FormatSTL, context );
}


    void
on_export_vrml2_activate               (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(menuitem));
	GtkWidget* mainwindow = glade_xml_get_widget(MainWindowXML, "MainWindow");
    Translate* anTrans = new Translate( mainwindow);
    Handle(AIS_InteractiveContext) context = theViewer->getContext();
    bool status = anTrans->exportModel( Translate::FormatVRML, context );
}


    void
on_export_image2_activate              (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(menuitem));
	GtkWidget* toolbutton = glade_xml_get_widget(MainWindowXML, "snapshot_toolbutton");
    on_snapshot_toolbutton_clicked(GTK_TOOL_BUTTON(toolbutton), user_data);
}


    void
on_quit1_activate                      (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(menuitem));
	GtkWidget* widget = glade_xml_get_widget(MainWindowXML, "MainWindow");
    GtkWidget* dialog = gtk_message_dialog_new (GTK_WINDOW(widget),
	    GTK_DIALOG_DESTROY_WITH_PARENT,
	    GTK_MESSAGE_QUESTION,
	    GTK_BUTTONS_YES_NO,
	    "Do You Want To Quit?");
    int result = gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
    if(result == GTK_RESPONSE_YES)
	gtk_main_quit();
}


    void
on_top_view1_activate                  (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    theViewer->TopView();
}


    void
on_bottom_view1_activate               (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    theViewer->BottomView();
}


    void
on_front_view1_activate                (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    theViewer->FrontView();
}


    void
on_back_view1_activate                 (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    theViewer->BackView();
}


    void
on_left_view1_activate                 (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    theViewer->LeftView();
}


    void
on_right_view1_activate                (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    theViewer->RightView();
}


    void
on_axonometric_view1_activate          (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    theViewer->AxonView();
}


    void
on_reset_view1_activate                (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    theViewer->ResetView();
}


    void
on_zoom_view1_activate                 (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    theViewer->ZoomView();
}


    void
on_pan_view1_activate                  (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    theViewer->PanView();
}


    void
on_rotate_view1_activate               (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    theViewer->RotateView();
}


    void
on_zoom_window1_activate               (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    theViewer->ZoomWindow();
}


    void
on_fit_all1_activate                   (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    theViewer->FitAll();
}


    void
on_about1_activate                     (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(menuitem));
	GtkWidget* mw = glade_xml_get_widget(MainWindowXML, "MainWindow");
    GtkWidget* about = create_AboutDialog();
    gtk_window_set_transient_for(GTK_WINDOW(about), GTK_WINDOW(mw));
    gtk_dialog_run(GTK_DIALOG(about));
    gtk_widget_destroy(about);
}


    void
on_import_iges_toolbutton_clicked      (GtkToolButton   *toolbutton,
	gpointer         user_data)
{
		GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(toolbutton));
		GtkWidget* menuitem = glade_xml_get_widget(MainWindowXML, "import_iges1");
    on_import_iges1_activate(GTK_MENU_ITEM(menuitem), NULL);
}


    void
on_import_step_toolbutton_clicked      (GtkToolButton   *toolbutton,
	gpointer         user_data)
{
		GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(toolbutton));
		GtkWidget* menuitem = glade_xml_get_widget(MainWindowXML, "import_step1");
    on_import_step1_activate(GTK_MENU_ITEM(menuitem), NULL);
}


    void
on_import_brep_toolbutton_clicked      (GtkToolButton   *toolbutton,
	gpointer         user_data)
{
		GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(toolbutton));
		GtkWidget* menuitem = glade_xml_get_widget(MainWindowXML, "import_brep1");
    on_import_brep1_activate(GTK_MENU_ITEM(menuitem), NULL);
}


    void
on_import_csfdb_toolbutton_clicked     (GtkToolButton   *toolbutton,
	gpointer         user_data)
{
		GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(toolbutton));
		GtkWidget* menuitem = glade_xml_get_widget(MainWindowXML, "import_csfdb1");
    on_import_csfdb1_activate(GTK_MENU_ITEM(menuitem), NULL);
}


    void
on_export_iges_toolbutton_clicked      (GtkToolButton   *toolbutton,
	gpointer         user_data)
{
		GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(toolbutton));
		GtkWidget* menuitem = glade_xml_get_widget(MainWindowXML, "export_iges2");
    on_export_iges2_activate(GTK_MENU_ITEM(menuitem), NULL);
}


    void
on_export_step_toolbutton_clicked      (GtkToolButton   *toolbutton,
	gpointer         user_data)
{
		GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(toolbutton));
		GtkWidget* menuitem = glade_xml_get_widget(MainWindowXML, "export_step2");
    on_export_step2_activate(GTK_MENU_ITEM(menuitem), NULL);
}


    void
on_export_brep_toolbutton_clicked      (GtkToolButton   *toolbutton,
	gpointer         user_data)
{
		GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(toolbutton));
		GtkWidget* menuitem = glade_xml_get_widget(MainWindowXML, "export_brep2");
    on_export_brep2_activate(GTK_MENU_ITEM(menuitem), NULL);
}


    void
on_export_csfdb_toolbutton_clicked     (GtkToolButton   *toolbutton,
	gpointer         user_data)
{
		GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(toolbutton));
		GtkWidget* menuitem = glade_xml_get_widget(MainWindowXML, "export_csfdb2");
    on_export_csfdb2_activate(GTK_MENU_ITEM(menuitem), NULL);
}


    void
on_export_stl_toolbutton_clicked       (GtkToolButton   *toolbutton,
	gpointer         user_data)
{
		GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(toolbutton));
		GtkWidget* menuitem = glade_xml_get_widget(MainWindowXML, "export_stl1");
    on_export_stl1_activate(GTK_MENU_ITEM(menuitem), NULL);
}


    void
on_export_vrml_toolbutton_clicked      (GtkToolButton   *toolbutton,
	gpointer         user_data)
{
		GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(toolbutton));
		GtkWidget* menuitem = glade_xml_get_widget(MainWindowXML, "export_vrml2");
    on_export_vrml2_activate(GTK_MENU_ITEM(menuitem), NULL);
}


    void
on_export_image_toolbutton_clicked     (GtkToolButton   *toolbutton,
	gpointer         user_data)
{
		GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(toolbutton));
		GtkWidget* menuitem = glade_xml_get_widget(MainWindowXML, "export_image2");
    on_export_image2_activate(GTK_MENU_ITEM(menuitem), NULL);
}


    void
on_about_toolbutton_clicked            (GtkToolButton   *toolbutton,
	gpointer         user_data)
{
		GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(toolbutton));
		GtkWidget* menuitem = glade_xml_get_widget(MainWindowXML, "about1");
    on_about1_activate(GTK_MENU_ITEM(menuitem), NULL);
}


    void
on_top_view_toolbutton_clicked         (GtkToolButton   *toolbutton,
	gpointer         user_data)
{
    theViewer->TopView();
}


    void
on_bottom_view_toolbutton_clicked      (GtkToolButton   *toolbutton,
	gpointer         user_data)
{
    theViewer->BottomView();
}


    void
on_front_view_toolbutton_clicked       (GtkToolButton   *toolbutton,
	gpointer         user_data)
{
    theViewer->FrontView();
}


    void
on_back_view_toolbutton_clicked        (GtkToolButton   *toolbutton,
	gpointer         user_data)
{
    theViewer->BackView();
}


    void
on_left_view_toolbutton_clicked        (GtkToolButton   *toolbutton,
	gpointer         user_data)
{
    theViewer->LeftView();
}


    void
on_right_view_toolbutton_clicked       (GtkToolButton   *toolbutton,
	gpointer         user_data)
{
    theViewer->RightView();
}


    void
on_axo_view_toolbutton_clicked         (GtkToolButton   *toolbutton,
	gpointer         user_data)
{
    theViewer->AxonView();
}


    void
on_reset_view_toolbutton_clicked       (GtkToolButton   *toolbutton,
	gpointer         user_data)
{
    theViewer->ResetView();
}


    void
on_dyn_zoom_toolbutton_toggled         (GtkToggleToolButton *toggletoolbutton,
	gpointer         user_data)
{
    if(gtk_toggle_tool_button_get_active(toggletoolbutton))
    {
	theViewer->ZoomView();
	theViewer->ActivateCursor(theViewer->getCurrentAction3d());
    }
}


    void
on_dyn_pan_toolbutton_toggled          (GtkToggleToolButton *toggletoolbutton,
	gpointer         user_data)
{
    if(gtk_toggle_tool_button_get_active(toggletoolbutton))
    {
	theViewer->PanView();
	theViewer->ActivateCursor(theViewer->getCurrentAction3d());
    }
}


    void
on_dyn_rotate_toolbutton_toggled       (GtkToggleToolButton *toggletoolbutton,
	gpointer         user_data)
{
    if(gtk_toggle_tool_button_get_active(toggletoolbutton))
    {
	theViewer->RotateView();
	theViewer->ActivateCursor(theViewer->getCurrentAction3d());
    }
}


    void
on_glob_pan_toolbutton_toggled         (GtkToggleToolButton *toggletoolbutton,
	gpointer         user_data)
{
    if(gtk_toggle_tool_button_get_active(toggletoolbutton))
    {
	theViewer->GlobPan();
	theViewer->ActivateCursor(theViewer->getCurrentAction3d());
    }
}


    void
on_zoom_win_toolbutton_toggled         (GtkToggleToolButton *toggletoolbutton,
	gpointer         user_data)
{
    if(gtk_toggle_tool_button_get_active(toggletoolbutton))
    {
	theViewer->ZoomWindow();
	theViewer->ActivateCursor(theViewer->getCurrentAction3d());
    }
}


    void
on_fit_all_toolbutton_clicked          (GtkToolButton   *toolbutton,
	gpointer         user_data)
{
    theViewer->FitAll();
}

    void
on_color_toolbutton_clicked            (GtkToolButton   *toolbutton,
	gpointer         user_data)
{
    GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(toolbutton));
	GtkWidget* mainwindow = glade_xml_get_widget(MainWindowXML, "MainWindow");

    GtkWidget *dialog;
    GdkColor color;
    GtkColorSelection *colorsel;
    gint response;

    Quantity_Color aShapeColor;
    Standard_Real R1;
    Standard_Real G1;
    Standard_Real B1;

    Handle(AIS_InteractiveContext) myContext = theViewer->getContext();
    myContext->InitCurrent();
    Handle_AIS_InteractiveObject Current = myContext->Current() ;
    if ( Current->HasColor () )
    {
	aShapeColor = myContext->Color( Current );	
	color.red = (guint16)(aShapeColor.Red()*255*257);
	color.green = (guint16)(aShapeColor.Green()*255*257);
	color.blue = (guint16)( aShapeColor.Blue()*255*257);
    }
    else
    {
	color.red = 65535;
	color.green = 65535;
	color.blue = 65535;
    }

    dialog = gtk_color_selection_dialog_new ("Select Object color");

    gtk_window_set_transient_for (GTK_WINDOW (dialog), GTK_WINDOW (mainwindow));

    colorsel = GTK_COLOR_SELECTION (GTK_COLOR_SELECTION_DIALOG (dialog)->colorsel);
    gtk_color_selection_set_previous_color (colorsel, &color);
    gtk_color_selection_set_current_color (colorsel, &color);
    gtk_color_selection_set_has_palette (colorsel, TRUE);

    response = gtk_dialog_run (GTK_DIALOG (dialog));

    if (response == GTK_RESPONSE_OK)
    {
	gtk_color_selection_get_current_color (colorsel,
		&color);
	R1 = double(color.red/256)/255.0; 
	G1 = double(color.green/256)/255.0;
	B1 = double(color.blue/256)/255.0;
	Quantity_Color color( R1, G1, B1, Quantity_TOC_RGB );
	for (; myContext->MoreCurrent(); myContext->NextCurrent() )
	    myContext->SetColor( myContext->Current(), color.Name() );
    }
    gtk_widget_destroy (dialog);
}


    void
on_material_toolbutton_clicked         (GtkToolButton   *toolbutton,
	gpointer         user_data)
{
    GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(toolbutton));
	GtkWidget* mainwindow = glade_xml_get_widget(MainWindowXML, "MainWindow");
    GtkWidget* MatlDlg = create_MatlDlg();
    gtk_window_set_transient_for (GTK_WINDOW (MatlDlg), GTK_WINDOW (mainwindow));
    gtk_dialog_run(GTK_DIALOG(MatlDlg));    
    gtk_widget_destroy(MatlDlg);
}

    void
on_transparency_toolbutton_clicked     (GtkToolButton   *toolbutton,
	gpointer         user_data)
{
    GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(toolbutton));
	GtkWidget* mainwindow = glade_xml_get_widget(MainWindowXML, "MainWindow");
    GtkWidget* TranspDlg = create_Transparency();
    gtk_window_set_transient_for (GTK_WINDOW (TranspDlg), GTK_WINDOW (mainwindow));
    gtk_dialog_run(GTK_DIALOG(TranspDlg));    
    gtk_widget_destroy(TranspDlg);
}


    gboolean
on_drawingarea1_configure_event        (GtkWidget       *widget,
	GdkEventConfigure *event,
	gpointer         user_data)
{
    if(theViewer == NULL)
    {
		cout << "NULL VIEWER\n";
		exit(-1);
    }

	GdkGLContext *current_glcontext = gtk_widget_get_gl_context(widget);
	GdkGLDrawable *current_gldrawable = gtk_widget_get_gl_drawable(widget);

	// must set the active opengl context
	if (gdk_gl_drawable_gl_begin(current_gldrawable, current_glcontext))
	{
		if(firstTime)
		{
		theViewer->InitOCC(GTK_DRAWING_AREA(widget));
		firstTime = false;
		}    
		theViewer->ReSize();
		
		gdk_gl_drawable_gl_end (current_gldrawable);
	}else{
		cout << "Unable to obtain gl drawable context.\n";
	}

    return FALSE;
}


    void
on_drawingarea1_realize                (GtkWidget       *widget,
	gpointer         user_data)
{	
	GdkGLContext *current_glcontext = gtk_widget_get_gl_context(widget);
	GdkGLDrawable *current_gldrawable = gtk_widget_get_gl_drawable(widget);

	// must set the active opengl context
	if (gdk_gl_drawable_gl_begin(current_gldrawable, current_glcontext))
	{	
		//theViewer->opencascade_realize();
		Handle(Geom_Axis2Placement) aTrihedronAxis=new Geom_Axis2Placement(gp::XOY());
		Handle(AIS_Trihedron) myTrihedron = new AIS_Trihedron(aTrihedronAxis);
		theViewer->getContext()->Display(myTrihedron);
		
		gdk_gl_drawable_gl_end (current_gldrawable);
	}else{
		cout << "Unable to obtain gl drawable context.\n";
	}
}


    gboolean
on_drawingarea1_expose_event           (GtkWidget       *widget,
	GdkEventExpose  *event,
	gpointer         user_data)
{
	
	
	GdkGLContext *current_glcontext = gtk_widget_get_gl_context(widget);
	GdkGLDrawable *current_gldrawable = gtk_widget_get_gl_drawable(widget);

	// must set the active opengl context
	if (gdk_gl_drawable_gl_begin(current_gldrawable, current_glcontext))
	{
		theViewer->Draw();
		gdk_gl_drawable_gl_end (current_gldrawable);
	}else{
		cout << "Unable to obtain gl drawable context.\n";
	}
		
	return FALSE;
}


    gboolean
on_drawingarea1_button_press_event     (GtkWidget       *widget,
	GdkEventButton  *event,
	gpointer         user_data)
{
	GdkGLContext *current_glcontext = gtk_widget_get_gl_context(widget);
	GdkGLDrawable *current_gldrawable = gtk_widget_get_gl_drawable(widget);

	// must set the active opengl context
	if (gdk_gl_drawable_gl_begin(current_gldrawable, current_glcontext))
	{
		if ( event->button == 1 )
		{
		theViewer->LButtonDown( event );
		//std::cout << " First Button Pressed\n";
		}
		else if ( event->button == 2 )
		{
		theViewer->MButtonDown( event );
		//std::cout << " Second Button Pressed\n";
		}
		else if ( event->button == 3 )
		{
		theViewer->RButtonDown( event );
		//std::cout << " Third Button Pressed\n";
		}
		
		gdk_gl_drawable_gl_end (current_gldrawable);
	}else{
		cout << "Unable to obtain gl drawable context.\n";
	}
	
    return FALSE;
}


    gboolean
on_drawingarea1_button_release_event   (GtkWidget       *widget,
	GdkEventButton  *event,
	gpointer         user_data)
{
	GdkGLContext *current_glcontext = gtk_widget_get_gl_context(widget);
	GdkGLDrawable *current_gldrawable = gtk_widget_get_gl_drawable(widget);

	// must set the active opengl context
	if (gdk_gl_drawable_gl_begin(current_gldrawable, current_glcontext))
	{
		if ( event->state & GDK_BUTTON1_MASK )
		theViewer->LButtonUp( event );
		else if ( event->state & GDK_BUTTON2_MASK )
		theViewer->MButtonUp( event );
		else if ( event->state & GDK_BUTTON3_MASK )
		theViewer->RButtonUp( event );
		
		gdk_gl_drawable_gl_end (current_gldrawable);
	}else{
		cout << "Unable to obtain gl drawable context.\n";
	}
	
    return FALSE;
}


    gboolean
on_drawingarea1_motion_notify_event    (GtkWidget       *widget,
	GdkEventMotion  *event,
	gpointer         user_data)
{
	GdkGLContext *current_glcontext = gtk_widget_get_gl_context(widget);
	GdkGLDrawable *current_gldrawable = gtk_widget_get_gl_drawable(widget);

	// must set the active opengl context
	if (gdk_gl_drawable_gl_begin(current_gldrawable, current_glcontext))
	{
    	theViewer->MouseMove( event );
		
		gdk_gl_drawable_gl_end (current_gldrawable);
	}else{
		cout << "Unable to obtain gl drawable context.\n";
	}
	
    return FALSE;
}


    gboolean
on_drawingarea1_key_press_event        (GtkWidget       *widget,
	GdkEventKey     *event,
	gpointer         user_data)
{

    return FALSE;
}


    gboolean
on_drawingarea1_key_release_event      (GtkWidget       *widget,
	GdkEventKey     *event,
	gpointer         user_data)
{

    return FALSE;
}


    gboolean
on_drawingarea1_enter_notify_event     (GtkWidget       *widget,
	GdkEventCrossing *event,
	gpointer         user_data)
{

    return FALSE;
}


    gboolean
on_drawingarea1_leave_notify_event     (GtkWidget       *widget,
	GdkEventCrossing *event,
	gpointer         user_data)
{

    return FALSE;
}


    gboolean
on_drawingarea1_destroy_event          (GtkWidget       *widget,
	GdkEvent        *event,
	gpointer         user_data)
{

    return FALSE;
}


    gboolean
on_drawingarea1_map_event              (GtkWidget       *widget,
	GdkEvent        *event,
	gpointer         user_data)
{
	GdkGLContext *current_glcontext = gtk_widget_get_gl_context(widget);
	GdkGLDrawable *current_gldrawable = gtk_widget_get_gl_drawable(widget);

	// must set the active opengl context
	if (gdk_gl_drawable_gl_begin(current_gldrawable, current_glcontext))
	{
		theViewer->MapView();
		
		gdk_gl_drawable_gl_end (current_gldrawable);
	}else{
		cout << "Unable to obtain gl drawable context.\n";
	}
				
    return FALSE;
}


gboolean
on_drawingarea1_scroll_event           (GtkWidget       *widget,
                                        GdkEventScroll        *event,
                                        gpointer         user_data)
{
	GdkGLContext *current_glcontext = gtk_widget_get_gl_context(widget);
	GdkGLDrawable *current_gldrawable = gtk_widget_get_gl_drawable(widget);

	// must set the active opengl context
	if (gdk_gl_drawable_gl_begin(current_gldrawable, current_glcontext))
	{
		if(!(event->state & GDK_CONTROL_MASK))
		{
		if(event->direction == GDK_SCROLL_UP)
			theViewer->ZoomViewBy(5);
		if(event->direction == GDK_SCROLL_DOWN)
			theViewer->ZoomViewBy(-5);
		theViewer->Refresh();
		}
	
		gdk_gl_drawable_gl_end (current_gldrawable);
	}else{
		cout << "Unable to obtain gl drawable context.\n";
	}
	
    return FALSE;
}


    void
on_popup_change_background_color1_activate            (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(theViewer->GetGtkDrawingArea()));
	GtkWidget* mainwindow = glade_xml_get_widget(MainWindowXML, "MainWindow");
    GtkWidget *dialog;
    GdkColor color;
    GtkColorSelection *colorsel;
    gint response;

    Standard_Real R1;
    Standard_Real G1;
    Standard_Real B1;
    theViewer->getView()->BackgroundColor(Quantity_TOC_RGB,R1,G1,B1);

    //std::cout << "R " << R1 << " G " << G1 << " B " << B1 << std::endl;

    color.red = (guint16)(R1*255*257);
    color.green = (guint16)(G1*255*257);
    color.blue = (guint16)(B1*255*257);

    dialog = gtk_color_selection_dialog_new ("Select Background color");

    gtk_window_set_transient_for (GTK_WINDOW (dialog), GTK_WINDOW (mainwindow));

    colorsel = GTK_COLOR_SELECTION (GTK_COLOR_SELECTION_DIALOG (dialog)->colorsel);
    gtk_color_selection_set_previous_color (colorsel, &color);
    gtk_color_selection_set_current_color (colorsel, &color);
    gtk_color_selection_set_has_palette (colorsel, TRUE);

    response = gtk_dialog_run (GTK_DIALOG (dialog));

    if (response == GTK_RESPONSE_OK)
    {
	gtk_color_selection_get_current_color (colorsel,
		&color);
	R1 = double(color.red/256)/255.0; 
	G1 = double(color.green/256)/255.0;
	B1 = double(color.blue/256)/255.0;
	theViewer->getView()->SetBackgroundColor(Quantity_TOC_RGB,R1,G1,B1);
	theViewer->getView()->Redraw();
    }
    gtk_widget_destroy (dialog);
}


    void
on_popup_wireframe1_activate           (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    theViewer->ShowWaitCursor( true );
    Handle(AIS_InteractiveContext) myContext = theViewer->getContext();
    for( myContext->InitCurrent(); myContext->MoreCurrent(); myContext->NextCurrent() )
	myContext->SetDisplayMode( myContext->Current(), 0, false );
    myContext->UpdateCurrentViewer();
    theViewer->SelectionChanged();
    theViewer->ShowWaitCursor( false );
}


    void
on_popup_shading1_activate             (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    theViewer->ShowWaitCursor( true );
    Handle(AIS_InteractiveContext) myContext = theViewer->getContext();
    for( myContext->InitCurrent(); myContext->MoreCurrent(); myContext->NextCurrent() )
	myContext->SetDisplayMode( myContext->Current(), 1, false );
    myContext->UpdateCurrentViewer();
    theViewer->SelectionChanged();
    theViewer->ShowWaitCursor( false );
}


    void
on_popup_color1_activate               (GtkMenuItem     *menuitem,
	gpointer         user_data)
{    
    GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(theViewer->GetGtkDrawingArea()));
	GtkWidget* mainwindow = glade_xml_get_widget(MainWindowXML, "MainWindow");

    GtkWidget *dialog;
    GdkColor color;
    GtkColorSelection *colorsel;
    gint response;

    Quantity_Color aShapeColor;
    Standard_Real R1;
    Standard_Real G1;
    Standard_Real B1;

    Handle(AIS_InteractiveContext) myContext = theViewer->getContext();
    myContext->InitCurrent();
    Handle_AIS_InteractiveObject Current = myContext->Current() ;
    if ( Current->HasColor () )
    {
	aShapeColor = myContext->Color( Current );	
	color.red = (guint16)(aShapeColor.Red()*255*257);
	color.green = (guint16)(aShapeColor.Green()*255*257);
	color.blue = (guint16)( aShapeColor.Blue()*255*257);
    }
    else
    {
	color.red = 65535;
	color.green = 65535;
	color.blue = 65535;
    }

    dialog = gtk_color_selection_dialog_new ("Select Object color");

    gtk_window_set_transient_for (GTK_WINDOW (dialog), GTK_WINDOW (mainwindow));

    colorsel = GTK_COLOR_SELECTION (GTK_COLOR_SELECTION_DIALOG (dialog)->colorsel);
    gtk_color_selection_set_previous_color (colorsel, &color);
    gtk_color_selection_set_current_color (colorsel, &color);
    gtk_color_selection_set_has_palette (colorsel, TRUE);

    response = gtk_dialog_run (GTK_DIALOG (dialog));

    if (response == GTK_RESPONSE_OK)
    {
	gtk_color_selection_get_current_color (colorsel,
		&color);
	R1 = double(color.red/256)/255.0; 
	G1 = double(color.green/256)/255.0;
	B1 = double(color.blue/256)/255.0;
	Quantity_Color color( R1, G1, B1, Quantity_TOC_RGB );
	for (; myContext->MoreCurrent(); myContext->NextCurrent() )
	    myContext->SetColor( myContext->Current(), color.Name() );
    }
    gtk_widget_destroy (dialog);
}


    void
on_popup_material1_activate            (GtkMenuItem     *menuitem,
	gpointer         user_data)
{

}

    void 
set_material                           (int theMaterial)
{
    Handle(AIS_InteractiveContext) myContext = theViewer->getContext();
    for ( myContext->InitCurrent(); myContext->MoreCurrent (); myContext->NextCurrent () )
	myContext->SetMaterial( myContext->Current(), (Graphic3d_NameOfMaterial)theMaterial );
}

    void
on_brass1_activate                     (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    set_material(Graphic3d_NOM_BRASS);
}


    void
on_bronze1_activate                    (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    set_material(Graphic3d_NOM_BRONZE);
}


    void
on_copper1_activate                    (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    set_material(Graphic3d_NOM_COPPER);
}


    void
on_gold1_activate                      (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    set_material(Graphic3d_NOM_GOLD);
}


    void
on_pewter1_activate                    (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    set_material(Graphic3d_NOM_PEWTER);
}


    void
on_plaster1_activate                   (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    set_material(Graphic3d_NOM_PLASTER);
}


    void
on_plastic1_activate                   (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    set_material(Graphic3d_NOM_PLASTIC);
}


    void
on_silver1_activate                    (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    set_material(Graphic3d_NOM_SILVER);
}


    void
on_popup_transparency1_activate        (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(theViewer->GetGtkDrawingArea()));
	GtkWidget* mainwindow = glade_xml_get_widget(MainWindowXML, "MainWindow");
    GtkWidget* TranspDlg = create_Transparency();
    gtk_window_set_transient_for (GTK_WINDOW (TranspDlg), GTK_WINDOW (mainwindow));
    gtk_dialog_run(GTK_DIALOG(TranspDlg));    
    gtk_widget_destroy(TranspDlg);
}


    void
on_popup_delete1_activate              (GtkMenuItem     *menuitem,
	gpointer         user_data)
{
    Handle(AIS_InteractiveContext) myContext = theViewer->getContext();
    for ( myContext->InitCurrent(); myContext->MoreCurrent(); myContext->NextCurrent() )
	myContext->Erase( myContext->Current(), false, true );
    myContext->ClearSelected();
    theViewer->SelectionChanged();
}


    void
on_transparency_spinbutton_value_changed  (GtkSpinButton   *spinbutton,
	gpointer         user_data)
{
    gdouble theTrans = gtk_spin_button_get_value(spinbutton);
    Handle(AIS_InteractiveContext) myContext = theViewer->getContext();
    for( myContext->InitCurrent(); myContext->MoreCurrent(); myContext->NextSelected() )
	myContext->SetTransparency( myContext->Current(), ((Standard_Real)theTrans) / 10.0 );
}

    void
on_hlr_toolbutton_toggled              (GtkToggleToolButton *toggletoolbutton,
	gpointer         user_data)
{
    gboolean hlr_active = gtk_toggle_tool_button_get_active(toggletoolbutton);
    if(hlr_active)
	theViewer->HlrOff();
    else
	theViewer->HlrOn();
}


#include <AIS_ListOfInteractive.hxx>
#include <AIS_ListIteratorOfListOfInteractive.hxx>
    void
on_display_mode_toolbutton_toggled     (GtkToggleToolButton *toggletoolbutton,
	gpointer         user_data)
{
    Handle(AIS_InteractiveContext) myContext = theViewer->getContext();
    AIS_ListOfInteractive aListOfIO;
    AIS_ListIteratorOfListOfInteractive li;
    myContext->DisplayedObjects (aListOfIO);
    gboolean shading_active = gtk_toggle_tool_button_get_active(toggletoolbutton);
    theViewer->ShowWaitCursor(true);
    for( li.Initialize(aListOfIO); li.More(); li.Next() )
	myContext->SetDisplayMode( li.Value(), (gint)shading_active, false );
    myContext->UpdateCurrentViewer();
    theViewer->SelectionChanged();
    theViewer->ShowWaitCursor(false);
}


    void
on_antialiasing_toolbutton_toggled     (GtkToggleToolButton *toggletoolbutton,
	gpointer         user_data)
{
    gboolean antial_active = gtk_toggle_tool_button_get_active(toggletoolbutton);
    if(antial_active)
	theViewer->getView()->SetAntialiasingOn();
    else
	theViewer->getView()->SetAntialiasingOff();
    theViewer->getViewer()->Update();
}


    void
on_snapshot_toolbutton_clicked         (GtkToolButton   *toolbutton,
	gpointer         user_data)
{
    GladeXML *MainWindowXML = glade_get_widget_tree(GTK_WIDGET(toolbutton));
	GtkWidget* mainwindow = glade_xml_get_widget(MainWindowXML, "MainWindow");
    GtkWidget *dialog;
    GtkWidget* msgbox;
    GtkFileFilter *filter;
    char *fName = 0;

    dialog = gtk_file_chooser_dialog_new ("Save Snapshot Image File",
	    GTK_WINDOW(mainwindow),
	    GTK_FILE_CHOOSER_ACTION_SAVE,
	    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	    GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
	    NULL);
    gtk_window_set_modal(GTK_WINDOW(dialog), FALSE);

    filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Gif Files  *.gif, *.GIF");
    gtk_file_filter_add_pattern(filter, "*.gif");
    gtk_file_filter_add_pattern(filter, "*.GIF");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER (dialog), filter);

    filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "XWindow Dump Files  *.xwd, *.XWD");
    gtk_file_filter_add_pattern(filter, "*.xwd");
    gtk_file_filter_add_pattern(filter, "*.XWD");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER (dialog), filter);

    filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Bitmap Files  *.bmp, *.BMP");
    gtk_file_filter_add_pattern(filter, "*.bmp");
    gtk_file_filter_add_pattern(filter, "*.BMP");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER (dialog), filter);
    gtk_file_chooser_set_filter(GTK_FILE_CHOOSER (dialog), filter);

    filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "All Files *");
    gtk_file_filter_add_pattern(filter, "*");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER (dialog), filter);    

    gint response = gtk_dialog_run (GTK_DIALOG (dialog));

    if (response == GTK_RESPONSE_ACCEPT)
    {
	fName = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
	gtk_widget_destroy (dialog);	
    }
    else
    {
	gtk_widget_destroy (dialog);
    }
    if (fName)
    {
	std::string str(fName);
	if(!theViewer->SnapShot(str))
	{
	    msgbox = gtk_message_dialog_new(GTK_WINDOW (mainwindow),
		    GtkDialogFlags(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
		    GtkMessageType(GTK_MESSAGE_ERROR),
		    GtkButtonsType(GTK_BUTTONS_OK),
		    "Failed To Create Snapshot File!");
	    gtk_dialog_run(GTK_DIALOG (msgbox));
	    gtk_widget_destroy (msgbox);
	}
    }
}


// Material Dialog Callbacks
    void
on_brass_radiobutton_toggled           (GtkToggleButton *togglebutton,
	gpointer         user_data)
{
    if(gtk_toggle_button_get_active(togglebutton))
    {
	set_material(Graphic3d_NOM_BRASS);
    }
}


    void
on_bronze_radiobutton_toggled          (GtkToggleButton *togglebutton,
	gpointer         user_data)
{
    if(gtk_toggle_button_get_active(togglebutton))
    {
	set_material(Graphic3d_NOM_BRONZE);
    }
}


    void
on_copper_radiobutton_toggled          (GtkToggleButton *togglebutton,
	gpointer         user_data)
{
    if(gtk_toggle_button_get_active(togglebutton))
    {
	set_material(Graphic3d_NOM_COPPER);
    }
}


    void
on_gold_radiobutton_toggled            (GtkToggleButton *togglebutton,
	gpointer         user_data)
{
    if(gtk_toggle_button_get_active(togglebutton))
    {
	set_material(Graphic3d_NOM_GOLD);
    }
}


    void
on_pewter_radiobutton_toggled          (GtkToggleButton *togglebutton,
	gpointer         user_data)
{
    if(gtk_toggle_button_get_active(togglebutton))
    {
	set_material(Graphic3d_NOM_PEWTER);
    }
}


    void
on_plaster_radiobutton_toggled         (GtkToggleButton *togglebutton,
	gpointer         user_data)
{
    if(gtk_toggle_button_get_active(togglebutton))
    {
	set_material(Graphic3d_NOM_PLASTER);
    }
}


    void
on_plastic_radiobutton_toggled         (GtkToggleButton *togglebutton,
	gpointer         user_data)
{
    if(gtk_toggle_button_get_active(togglebutton))
    {
	set_material(Graphic3d_NOM_PLASTIC);
    }
}


    void
on_silver_radiobutton_toggled          (GtkToggleButton *togglebutton,
	gpointer         user_data)
{
    if(gtk_toggle_button_get_active(togglebutton))
    {
	set_material(Graphic3d_NOM_SILVER);
    }
}


