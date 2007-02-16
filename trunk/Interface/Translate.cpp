#if !defined(CSFDB)
#error CSFDB precompiler directive is mandatory for CasCade 
#endif

#include "Translate.h"

#include <AIS_Shape.hxx>
#include <AIS_InteractiveObject.hxx>

#include <FSD_File.hxx>

#include <ShapeSchema.hxx>
#include <Storage_Data.hxx>
#include <Storage_Root.hxx>
#include <Storage_HSeqOfRoot.hxx>
#include <PTopoDS_HShape.hxx>
#include <PTColStd_PersistentTransientMap.hxx>
#include <PTColStd_TransientPersistentMap.hxx>

#include <IGESControl_Reader.hxx>
#include <IGESControl_Writer.hxx>
#include <IGESControl_Controller.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>
#include <STEPControl_StepModelType.hxx>
#include <Interface_Static.hxx>
#include <Interface_TraceFile.hxx>

#include <StlAPI_Writer.hxx>
#include <VrmlAPI_Writer.hxx>

#include <MgtBRep.hxx>
#include <MgtBRep_TriangleMode.hxx>

#include <BRepTools.hxx>
#include <BRep_Tool.hxx>
#include <BRep_Builder.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>
#include <TopExp_Explorer.hxx>
#include <TopTools_HSequenceOfShape.hxx>

#include <Geom_Line.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Plane.hxx>
#include <Geom_Surface.hxx>


#include <Standard_ErrorHandler.hxx>
#include <Standard_CString.hxx>

// ---------------------------- TranslateDlg -----------------------------------------

#include "Interface.h"
#include "Support.h"

	
class TranslateDlg
{
    public:
	TranslateDlg( GtkWidget* parent );
	~TranslateDlg();
	GtkWidget* getDialog() { return myDialog; }
	int                   getMode() const;
	void                  setMode( int );
	void                  addMode( int, const std::string& );
	void                  clear();

    private:
	GtkWidget*	      myParent;
	GtkWidget*	      myDialog;
	GtkComboBox*          myBox;
	GList*                myList;
	GList*                myListStart;
};

TranslateDlg::TranslateDlg( GtkWidget* parent )
: myParent(parent)
{
    /*
    QGroupBox* paramGroup = new QGroupBox( 1, Qt::Vertical, this );
    paramGroup->setFrameStyle( QFrame::NoFrame );
    if ( paramGroup->layout() )
	paramGroup->layout()->setMargin( 0 );
    myBox = new QComboBox( paramGroup );
    addWidgets( 0, paramGroup, 0 );
    */
    myList = NULL;
    myListStart = NULL;
    myDialog = create_TransDlg();
    myBox = GTK_COMBO_BOX(lookup_widget(myDialog, "combobox1"));
}

TranslateDlg::~TranslateDlg()
{
}

int TranslateDlg::getMode() const
{
    if ( gtk_combo_box_get_active(myBox) < 0 || gtk_combo_box_get_active(myBox) > (int)g_list_length(myList) - 1 )
	return -1;
    else
	return *((int*)g_list_nth_data(myList, gtk_combo_box_get_active(myBox)));
}

void TranslateDlg::setMode( int mode )
{
    int idx = g_list_index(myList, &mode );
    if ( idx >= 0 )
	gtk_combo_box_set_active(myBox, idx);
}

void TranslateDlg::addMode( int mode, const std::string& name )
{
    gtk_widget_show(GTK_WIDGET(myBox));
    gtk_combo_box_append_text(myBox, name.data());
    myListStart = g_list_append(myList, &mode);
    gtk_widget_queue_resize(GTK_WIDGET(myBox));
    gtk_widget_queue_resize(GTK_WIDGET(myDialog));
}

void TranslateDlg::clear()
{
    int cnt;
    for(cnt = 0; cnt < g_list_length(myList); ++cnt)
    {
	gtk_combo_box_remove_text(myBox, cnt);
    }
    g_list_free(myList);    
    gtk_widget_hide(GTK_WIDGET(myBox));
    gtk_widget_queue_resize(GTK_WIDGET(myBox));
    gtk_widget_queue_resize(GTK_WIDGET(myDialog));
}

// ---------------------------- Translate -----------------------------------------

Translate::Translate(GtkWidget* widget) : myParent(widget), myDlg(0)
{
}

Translate::~Translate()
{
    if ( myDlg )
	delete myDlg;
}

std::string Translate::info() const
{
    return myInfo;
}

bool Translate::importModel( const int format, const Handle(AIS_InteractiveContext)& ic )
{
    //myInfo = std::string();
    std::string fileName = selectFileName( format, true );
    if ( fileName.empty() )
	return true;

    /*
    if ( !QFileInfo( fileName ).exists() )
    {
	myInfo = tr( "INF_TRANSLATE_FILENOTFOUND" ).arg( fileName );
	return false;
    }
    */

    gdk_window_set_cursor(GTK_WIDGET(myParent)->window, gdk_cursor_new(GDK_CLOCK));
    Handle(TopTools_HSequenceOfShape) shapes = importModel( format, fileName );
    gdk_window_set_cursor(GTK_WIDGET(myParent)->window, NULL);

    if ( shapes.IsNull() || !shapes->Length() )
	return false;

    for ( int i = 1; i <= shapes->Length(); i++ )
	ic->Display( new AIS_Shape( shapes->Value( i ) ), false );
    ic->UpdateCurrentViewer();
    return true;
}

Handle(TopTools_HSequenceOfShape) Translate::importModel( const int format, const std::string& file )
{
    Handle(TopTools_HSequenceOfShape) shapes;
    try {
	switch ( format )
	{
	    case FormatBREP:
		shapes = importBREP( file );
		break;
	    case FormatIGES:
		shapes = importIGES( file );
		break;
	    case FormatSTEP:
		shapes = importSTEP( file );
		break;
	    case FormatCSFDB:
		shapes = importCSFDB( file );
		break;
	}
    } catch ( Standard_Failure ) {
	shapes.Nullify();
    }
    return shapes;
}

bool Translate::exportModel( const int format, const Handle(AIS_InteractiveContext)& ic )
{
    //myInfo = std::string();
    std::string fileName = selectFileName( format, false );
    if ( fileName.empty() )
	return true;

    Handle(TopTools_HSequenceOfShape) shapes = getShapes( ic );
    if ( shapes.IsNull() || !shapes->Length() )
	return false;

    gdk_window_set_cursor(GTK_WIDGET(myParent)->window, gdk_cursor_new(GDK_CLOCK));
    bool stat = exportModel( format, fileName, shapes );
    gdk_window_set_cursor(GTK_WIDGET(myParent)->window, NULL);

    return stat;
}

bool Translate::exportModel( const int format, const std::string& file, const Handle(TopTools_HSequenceOfShape)& shapes )
{
    bool status;
    try {
	switch ( format )
	{
	    case FormatBREP:
		status = exportBREP( file, shapes );
		break;
	    case FormatIGES:
		status = exportIGES( file, shapes );
		break;
	    case FormatSTEP:
		status = exportSTEP( file, shapes );
		break;
	    case FormatCSFDB:
		status = exportCSFDB( file, shapes );
		break;
	    case FormatSTL:
		status = exportSTL( file, shapes );
		break;
	    case FormatVRML:
		status = exportVRML( file, shapes );
		break;
	}
    } catch ( Standard_Failure ) {
	status = false;
    }
    return status;
}

Handle(TopTools_HSequenceOfShape) Translate::getShapes( const Handle(AIS_InteractiveContext)& ic )
{
    Handle(TopTools_HSequenceOfShape) aSequence;
    Handle(AIS_InteractiveObject) picked;
    for ( ic->InitCurrent(); ic->MoreCurrent(); ic->NextCurrent() )
    {
	Handle(AIS_InteractiveObject) obj = ic->Current();
	if ( obj->IsKind( STANDARD_TYPE( AIS_Shape ) ) )
	{
	    TopoDS_Shape shape = Handle(AIS_Shape)::DownCast(obj)->Shape();
	    if ( aSequence.IsNull() )
		aSequence = new TopTools_HSequenceOfShape();
	    aSequence->Append( shape );
	}
    }
    return aSequence;
}

/*!
  Selects a file from standard dialog acoording to selection 'filter'
  */
std::string Translate::selectFileName( const int format, const bool import )
{
    TranslateDlg* theDlg = getDialog( format, import );

    gtk_window_set_transient_for(GTK_WINDOW(theDlg->getDialog()), GTK_WINDOW(myParent));

    gint response = gtk_dialog_run(GTK_DIALOG(theDlg->getDialog()));

    //qApp->processEvents();

    std::string file;
    
    if ( response == GTK_RESPONSE_CANCEL )
    {
	gtk_widget_destroy(GTK_WIDGET(theDlg->getDialog()));
	return file;
    }
    
    gchar* fName = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (theDlg->getDialog()));
    file.assign(fName);

    std::cout << file << std::endl;
    
    /*
    file = theDlg->selectedFile();
    if ( !QFileInfo( file ).extension().length() )
    {
	std::string selFilter = theDlg->selectedFilter();
	int idx = selFilter.find( "(*." );
	if ( idx != -1 )
	{
	    std::string tail = selFilter.mid( idx + 3 );
	    idx = tail.find( " " );
	    if ( idx == -1 )
		idx = tail.find( ")" );
	    std::string ext = tail.left( idx );
	    if ( ext.length() )
		file += std::string( "." ) + ext;
	}
    }
    */
    gtk_widget_destroy(GTK_WIDGET(theDlg->getDialog()));
    return file;
}

TranslateDlg* Translate::getDialog( const int format, const bool import )
{
    if ( !myDlg )
	myDlg = new TranslateDlg( myParent );
    GtkFileFilter *filter = gtk_file_filter_new();
    gchar* data;

    if ( format < 0 )
	return myDlg;

    switch(format)
    {
	case FormatBREP:
	    gtk_file_filter_set_name(filter, "BREP Files  *.brep *.rle");
	    gtk_file_filter_add_pattern(filter, "*.brep");
	    gtk_file_filter_add_pattern(filter, "*.rle");
	    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER (myDlg->getDialog()), filter);
	    data = g_strdup_printf("/../data/occ");
	    break;
	case FormatIGES:
	    gtk_file_filter_set_name(filter, "IGES Files  *.igs *.iges");
	    gtk_file_filter_add_pattern(filter, "*.igs");
	    gtk_file_filter_add_pattern(filter, "*.iges");
	    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER (myDlg->getDialog()), filter);
	    data = g_strdup_printf("/../data/iges");
	    break;
	case FormatSTEP:
	    gtk_file_filter_set_name(filter, "STEP Files  *.stp *.step");
	    gtk_file_filter_add_pattern(filter, "*.stp");
	    gtk_file_filter_add_pattern(filter, "*.step");
	    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER (myDlg->getDialog()), filter);
	    data = g_strdup_printf("/../data/step");
	    break;
	case FormatCSFDB:
	    gtk_file_filter_set_name(filter, "CSFDB Files  *.csfdb");
	    gtk_file_filter_add_pattern(filter, "*.csfdb");
	    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER (myDlg->getDialog()), filter);
	    data = g_strdup_printf("/../data/csfdb");
	    break;
	case FormatVRML:
	    gtk_file_filter_set_name(filter, "VRML Files  *.vrml *.wrl");
	    gtk_file_filter_add_pattern(filter, "*.vrml");
	    gtk_file_filter_add_pattern(filter, "*.wrl");
	    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER (myDlg->getDialog()), filter);
	    data = g_strdup_printf("/../data/vrml");
	    break;
	case FormatSTL:
	    gtk_file_filter_set_name(filter, "StL Files  *.stl");
	    gtk_file_filter_add_pattern(filter, "*.stl");
	    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER (myDlg->getDialog()), filter);
	    data = g_strdup_printf("/../data/stl");
	    break;
	default:
	    break;
    }

    if ( import ) 
    {
	filter = gtk_file_filter_new();
	gtk_file_filter_set_name(filter, "All Files  *.*");
	gtk_file_filter_add_pattern(filter, "*.*");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER (myDlg->getDialog()), filter);
    }

    //cout << filter << endl;    

    if ( import )
    {
	gtk_file_chooser_set_action(GTK_FILE_CHOOSER(myDlg->getDialog()),
                                             GTK_FILE_CHOOSER_ACTION_OPEN);
	gtk_window_set_title(GTK_WINDOW(myDlg->getDialog()), "Import File");
	//((QFileDialog*)myDlg)->setMode( QFileDialog::ExistingFile );
    }
    else
    {
	gtk_file_chooser_set_action(GTK_FILE_CHOOSER(myDlg->getDialog()),
                                             GTK_FILE_CHOOSER_ACTION_SAVE);
	gtk_window_set_title(GTK_WINDOW(myDlg->getDialog()), "Export File");
	//((QFileDialog*)myDlg)->setMode( QFileDialog::AnyFile );
    }

    std::string dpath;
    dpath.assign(data);
    std::string datadir = std::string(getenv("CASROOT")) + dpath;
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(myDlg->getDialog()), datadir.data());
    //myDlg->setSelection( datadir );

    myDlg->clear();

    if ( !import )
    {
	switch ( format )
	{
	    case FormatSTEP:
		myDlg->addMode( STEPControl_ManifoldSolidBrep, "Manifold Solid Brep") ;
		myDlg->addMode( STEPControl_FacetedBrep, "Faceted Brep");
		myDlg->addMode( STEPControl_ShellBasedSurfaceModel, "Shell Based Surface Model");
		myDlg->addMode( STEPControl_GeometricCurveSet, "Geometric Curve Set");
		break;
	    case FormatCSFDB:
		myDlg->addMode( MgtBRep_WithTriangle, "Export With Triangles");
		myDlg->addMode( MgtBRep_WithoutTriangle, "Export Without Triangles");
		break;
	}
    }

    return myDlg;
}

// ----------------------------- Import functionality -----------------------------

Handle(TopTools_HSequenceOfShape) Translate::importBREP( const std::string& file )
{
    Handle(TopTools_HSequenceOfShape) aSequence;
    TopoDS_Shape aShape;
    BRep_Builder aBuilder;

    Standard_Boolean result = BRepTools::Read( aShape, (Standard_CString)file.data(), aBuilder );
    if ( result )
    {
	aSequence = new TopTools_HSequenceOfShape();
	aSequence->Append( aShape );
    }
    return aSequence;
}

Handle(TopTools_HSequenceOfShape) Translate::importIGES( const std::string& file )
{
    Handle(TopTools_HSequenceOfShape) aSequence;
    IGESControl_Reader Reader;
    int status = Reader.ReadFile( (Standard_CString)file.data() );

    if ( status == IFSelect_RetDone )
    {
	aSequence = new TopTools_HSequenceOfShape();
	Reader.TransferRoots();
	TopoDS_Shape aShape = Reader.OneShape();
	aSequence->Append( aShape );
    }
    return aSequence;
}

Handle(TopTools_HSequenceOfShape) Translate::importSTEP( const std::string& file )
{
    Handle(TopTools_HSequenceOfShape) aSequence;

    STEPControl_Reader aReader;
    IFSelect_ReturnStatus status = aReader.ReadFile( (Standard_CString)file.data() );
    if ( status == IFSelect_RetDone )
    {
	Interface_TraceFile::SetDefault();
	bool failsonly = false;
	aReader.PrintCheckLoad( failsonly, IFSelect_ItemsByEntity );

	int nbr = aReader.NbRootsForTransfer();
	aReader.PrintCheckTransfer( failsonly, IFSelect_ItemsByEntity );
	for ( Standard_Integer n = 1; n <= nbr; n++ )
	{
	    bool ok = aReader.TransferRoot( n );
	    int nbs = aReader.NbShapes();
	    if ( nbs > 0 )
	    {
		aSequence = new TopTools_HSequenceOfShape();
		for ( int i = 1; i <= nbs; i++ )
		{
		    TopoDS_Shape shape = aReader.Shape( i );
		    aSequence->Append( shape );
		}
	    }
	}
    }
    return aSequence;
}

Handle(TopTools_HSequenceOfShape) Translate::importCSFDB( const std::string& file )
{
    Handle(TopTools_HSequenceOfShape) aSequence;

    // Check file type
    if ( FSD_File::IsGoodFileType( (Standard_CString)file.data() ) != Storage_VSOk )
	return aSequence;

    static FSD_File fileDriver;
    TCollection_AsciiString aName( (Standard_CString)file.data() );
    if ( fileDriver.Open( aName, Storage_VSRead ) != Storage_VSOk )
	return aSequence;

    Handle(ShapeSchema) schema = new ShapeSchema();
    Handle(Storage_Data) data  = schema->Read( fileDriver );
    if ( data->ErrorStatus() != Storage_VSOk )
	return aSequence;

    fileDriver.Close();

    aSequence = new TopTools_HSequenceOfShape();
    Handle(Storage_HSeqOfRoot) roots = data->Roots();
    for ( int i = 1; i <= roots->Length() ; i++ )
    {
	Handle(Storage_Root) r = roots->Value( i );
	Handle(Standard_Persistent) p = r->Object();
	Handle(PTopoDS_HShape) aPShape = Handle(PTopoDS_HShape)::DownCast(p);
	if ( !aPShape.IsNull() )
	{
	    PTColStd_PersistentTransientMap aMap;
	    TopoDS_Shape aTShape;
	    MgtBRep::Translate( aPShape, aMap, aTShape, MgtBRep_WithTriangle );
	    aSequence->Append( aTShape );
	}
    }

    return aSequence;
}

// ----------------------------- Export functionality -----------------------------

bool Translate::exportBREP( const std::string& file, const Handle(TopTools_HSequenceOfShape)& shapes )
{
    if ( shapes.IsNull() || shapes->IsEmpty() )
	return false;

    TopoDS_Shape shape = shapes->Value( 1 );
    return BRepTools::Write( shape, (Standard_CString)file.data() ); 
}

bool Translate::exportIGES( const std::string& file, const Handle(TopTools_HSequenceOfShape)& shapes )
{
    if ( shapes.IsNull() || shapes->IsEmpty() )
	return false;

    IGESControl_Controller::Init();
    IGESControl_Writer writer( Interface_Static::CVal( "XSTEP.iges.unit" ),
	    Interface_Static::IVal( "XSTEP.iges.writebrep.mode" ) );

    for ( int i = 1; i <= shapes->Length(); i++ )
	writer.AddShape ( shapes->Value( i ) );
    writer.ComputeModel();
    return writer.Write( (Standard_CString)file.data() );
}

bool Translate::exportSTEP( const std::string& file, const Handle(TopTools_HSequenceOfShape)& shapes )
{
    if ( shapes.IsNull() || shapes->IsEmpty() )
	return false;

    TranslateDlg* theDlg = getDialog( -1, false );
    STEPControl_StepModelType type = (STEPControl_StepModelType)theDlg->getMode();
    if ( type < 0 )
	return false;

    IFSelect_ReturnStatus status;

    if ( type == STEPControl_FacetedBrep && !checkFacetedBrep( shapes ) )
    {
	myInfo = "At least one shape doesn't contain facetes.";
	return false;
    }

    STEPControl_Writer writer;
    for ( int i = 1; i <= shapes->Length(); i++ )
    {
	status = writer.Transfer( shapes->Value( i ), type );
	if ( status != IFSelect_RetDone )
	    return false;
    }

    status = writer.Write( (Standard_CString)file.data() );

    switch ( status )
    {
	case IFSelect_RetError:
	    myInfo = "Incorrect Data.";
	    break;
	case IFSelect_RetFail:
	    myInfo = "Writing has failed";
	    break;
	case IFSelect_RetVoid:
	    myInfo = "Nothing to transfer.";
	    break;
    }
    return status == IFSelect_RetDone;
}

#include <TCollection_ExtendedString.hxx>
bool Translate::exportCSFDB( const std::string& file, const Handle(TopTools_HSequenceOfShape)& shapes )
{
    if ( shapes.IsNull() || shapes->IsEmpty() )
	return false;

    TranslateDlg* theDlg = getDialog( -1, false );
    MgtBRep_TriangleMode type = (MgtBRep_TriangleMode)theDlg->getMode();
    if ( type < 0 )
	return false;

    static FSD_File fileDriver;

    Handle(ShapeSchema) schema = new ShapeSchema();
    Handle(Storage_Data) data  = new Storage_Data();
    data->ClearErrorStatus();

    data->SetApplicationName( TCollection_ExtendedString( "Sample Import / Export" ) );
    data->SetApplicationVersion( "1" );
    data->SetDataType( TCollection_ExtendedString( "Shapes" ) );
    data->AddToUserInfo( "Storing a persistent set of shapes in a flat file" );
    data->AddToComments( TCollection_ExtendedString( "Application is based on CasCade 6.1.0 Professional" ) );

    if ( fileDriver.Open( (Standard_CString)file.data(), Storage_VSWrite ) != Storage_VSOk )
    {
	gchar* str = g_strdup_printf("Can't save file %s", file.data());
	myInfo.assign(str);
	return false;
    }

    PTColStd_TransientPersistentMap aMap;
    for ( int i = 1; i <= shapes->Length(); i++ )
    {
	TopoDS_Shape shape = shapes->Value( i );
	if ( shape.IsNull() )
	{
	    myInfo = "Some shapes are invalid.";
	    return false;
	}

	Handle(PTopoDS_HShape) pshape = MgtBRep::Translate( shape, aMap, type );
	TCollection_AsciiString objName = TCollection_AsciiString( "Object_" ) + TCollection_AsciiString( i );
	data->AddRoot( objName, pshape );
    }

    schema->Write( fileDriver, data );
    fileDriver.Close();

    if ( data->ErrorStatus() != Storage_VSOk )
    {
	myInfo = "Can't store persistent data.";
	return false;
    } 
    return true;
}

bool Translate::exportSTL( const std::string& file, const Handle(TopTools_HSequenceOfShape)& shapes )
{
    if ( shapes.IsNull() || shapes->IsEmpty() )
	return false;

    TopoDS_Compound res;
    BRep_Builder builder;
    builder.MakeCompound( res );

    for ( int i = 1; i <= shapes->Length(); i++ )
    {
	TopoDS_Shape shape = shapes->Value( i );
	if ( shape.IsNull() ) 
	{
	    myInfo = "Some shapes are invalid.";
	    return false;
	}
	builder.Add( res, shape );
    }

    StlAPI_Writer writer;
    writer.Write( res, (Standard_CString)file.data() );

    return true;
}

bool Translate::exportVRML( const std::string& file, const Handle(TopTools_HSequenceOfShape)& shapes )
{
    if ( shapes.IsNull() || shapes->IsEmpty() )
	return false;

    TopoDS_Compound res;
    BRep_Builder builder;
    builder.MakeCompound( res );

    for ( int i = 1; i <= shapes->Length(); i++ )
    {
	TopoDS_Shape shape = shapes->Value( i );
	if ( shape.IsNull() )
	{
	    myInfo = "Some shapes are invalid.";
	    return false;
	}
	builder.Add( res, shape );
    }

    VrmlAPI_Writer writer;
    writer.Write( res, (Standard_CString)file.data() );

    return true;
}

bool Translate::checkFacetedBrep( const Handle(TopTools_HSequenceOfShape)& shapes )
{
    bool err = false;
    for ( int i = 1; i <= shapes->Length(); i++ )
    {
	TopoDS_Shape shape = shapes->Value( i );
	for ( TopExp_Explorer fexp( shape, TopAbs_FACE ); fexp.More() && !err; fexp.Next() )
	{
	    Handle(Geom_Surface) surface = BRep_Tool::Surface( TopoDS::Face( fexp.Current() ) );
	    if ( !surface->IsKind( STANDARD_TYPE( Geom_Plane ) ) )
		err = true;
	}
	for ( TopExp_Explorer eexp( shape, TopAbs_EDGE ); eexp.More() && !err; eexp.Next() )
	{
	    Standard_Real fd, ld;
	    Handle(Geom_Curve) curve = BRep_Tool::Curve( TopoDS::Edge( eexp.Current() ), fd, ld );
	    if ( !curve->IsKind( STANDARD_TYPE( Geom_Line ) ) )
		err = true;
	}
    }
    return !err;
}
