#include "Ark3DWidget.h"

#include "../OpenCascadeBinding/OccPrimitives.h"

Ark3DWidget::Ark3DWidget( const Handle_AIS_InteractiveContext& aContext,
						  QWidget *parent,
						  Qt::WindowFlags wflags) :
QoccViewWidget(aContext, parent, wflags)
{
	interactive_primitive_ = 0;

/*
	// create the python interpretor instance
	PyImport_AppendInittab( "ark3d_module", &initark3d_module );
	Py_Initialize();
	//init_ark3d_module();
	PyRun_SimpleString("import ark3d_module \n");
	PyRun_SimpleString("import sys \n");
	PyRun_SimpleString("sys.path.append('./src/PythonBinding/') \n");
	PyRun_SimpleString("import ark3d_module \n");
	
	PyRun_SimpleString("point1 =  ark3d_module.CreatePoint(0.0,1.0,2.0) \n");
	PyRun_SimpleString("print point1.GetYDOF().GetValue() \n");
*/
}

Ark3DWidget::~Ark3DWidget()
{
	if(interactive_primitive_ != 0)
		delete interactive_primitive_;
}

void Ark3DWidget::paintEvent        ( QPaintEvent* e )
{
	QoccViewWidget::paintEvent(e);  // the first time this method is run, it will intialize myView 

	// Check to see if myView has been initialized and, if so, generate the current sketch if it doesn't already exist
	if(current_sketch_.get() == 0 && GetViewInitialized())
		GenerateDefaultSketch();
}

void Ark3DWidget::resizeEvent       ( QResizeEvent* e )
{
	QoccViewWidget::resizeEvent(e);
}

void Ark3DWidget::mousePressEvent   ( QMouseEvent* e )
{
	QoccViewWidget::mousePressEvent(e);
}

void Ark3DWidget::mouseReleaseEvent ( QMouseEvent* e )
{
	QoccViewWidget::mouseReleaseEvent(e);
}

void Ark3DWidget::mouseMoveEvent    ( QMouseEvent* e )
{
	QoccViewWidget::mouseMoveEvent(e);
}
void Ark3DWidget::wheelEvent        ( QWheelEvent* e )
{
	QoccViewWidget::wheelEvent(e);
}

void Ark3DWidget::leaveEvent		   ( QEvent * e)
{
	QoccViewWidget::leaveEvent(e);
}

void Ark3DWidget::GenerateDefaultSketch()
{
	// create the current Ark3D sketch
	VectorPointer normal( new Vector(0.0,0.0,1.0));
	VectorPointer up( new Vector(0.0,1.0,0.0));
	PointPointer base( new Point(0.0,0.0,0.0));
	current_sketch_ = OccSketchPointer(new OccSketch(GetContext(),normal, up, base));

	// set the Privileged Plane to be the current sketch plane
	GetViewer()->SetPrivilegedPlane(current_sketch_->Get_gp_Ax3());

	// set the grid size 
	GetViewer()->SetRectangularGridValues (0.0 /* x origin */, 0.0 /* y origin */, 1.0 /* XStep */, 1.0 /* YStep */, 0.0 /* RotationAngle */);
	GetViewer()->SetRectangularGridGraphicValues (50.0 /* XSize */, 50.0 /* YSize */, 0.0 /* OffSet */);

	// turn on the display of the grid (grid coincides with the privileged plane)
	GetViewer()->ActivateGrid(Aspect_GT_Rectangular,Aspect_GDM_Lines);
}

void Ark3DWidget::GenerateTestSketch()
{
	Point2DPointer point1 = current_sketch_->AddPoint2D(0.0,0.0,false,false);  // none of the dof's can vary
	Point2DPointer point2 = current_sketch_->AddPoint2D(10.0,0.0,true,false);  // only x dof can vary
	Point2DPointer point3 = current_sketch_->AddPoint2D(10.0,10.0,true,true);  // x and y dof's can vary

	Arc2DPointer arc1 = current_sketch_->AddArc2D(1.5,6.0,(mmcPI/2.0)*.8,(mmcPI)*1.2,2.0,true,true,true,true,false);

	Line2DPointer line1 = current_sketch_->AddLine2D(point1,point2);
	Line2DPointer line2 = current_sketch_->AddLine2D(point2,point3);
	Line2DPointer line3 = current_sketch_->AddLine2D(point3,arc1->GetPoint1());
	Line2DPointer line4 = current_sketch_->AddLine2D(arc1->GetPoint2(),point1);

	// These 5 constraints will fully constrain the four free DOF's defined about
	ConstraintEquationBasePointer constraint1 = current_sketch_->AddDistancePoint2D(point1,point2,6.0);
	ConstraintEquationBasePointer constraint2 = current_sketch_->AddDistancePoint2D(point2,point3,12.0);
	ConstraintEquationBasePointer constraint3 = current_sketch_->AddParallelLine2D(line1,line3);
	ConstraintEquationBasePointer constraint4 = current_sketch_->AddParallelLine2D(line2,line4);
	ConstraintEquationBasePointer constraint5 = current_sketch_->AddAngleLine2D(line1,line2,mmcPI/2.0);

	ConstraintEquationBasePointer constraint6 = current_sketch_->AddTangentEdge2D(line3,Point2,arc1,Point1);
	ConstraintEquationBasePointer constraint7 = current_sketch_->AddTangentEdge2D(line4,Point1,arc1,Point2);
	
	// create an edge loop
	EdgeLoop2DPointer edge_loop1(new EdgeLoop2D());
	edge_loop1->AddEdge(line1);
	edge_loop1->AddEdge(line2);
	edge_loop1->AddEdge(line3);
	edge_loop1->AddEdge(arc1);
	edge_loop1->AddEdge(line4);
	cout << "Is loop valid: " << edge_loop1->IsLoopValid() << endl;

	current_sketch_->ApplySelectionMask(Points);
	current_sketch_->ApplySelectionMask(Edges);
}

void Ark3DWidget::SolveConstraints() 
{
	if(current_sketch_ != 0)
	{
		current_sketch_->SolveConstraints();
		current_sketch_->UpdateDisplay();
	}
}

void Ark3DWidget::ExecutePythonScript()
{
/*
    FILE *fp = fopen ("./src/PythonScripts/test_sketch.py", "r+");
    PyRun_SimpleFile (fp, "./src/PythonScripts/test_sketch.py");
		fclose(fp);
*/
}