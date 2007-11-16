#include "Point2DConstructor.h"

#include <IntAna_IntConicQuad.hxx>

Point2DConstructor::Point2DConstructor(OccSketchPointer parent_sketch, Handle(V3d_View) current_view, Handle(V3d_Viewer) current_viewer):
InteractiveConstructorBase(parent_sketch, current_view, current_viewer),
primitive_finished_(false)
{

}

void Point2DConstructor::CreateObject()
{
	if(primitive_finished_)
		parent_sketch_->AddPoint2D(current_s_, current_t_, false, false);
}

bool Point2DConstructor::LeftButtonUp(MouseEventPropertiesPointer event_props)
{
	int x_screen = event_props->GetXPosition();
	int y_screen = event_props->GetYPosition();

	double Xv, Yv, Zv;
	double Vx, Vy, Vz;
	gp_Pln aPlane(current_view_->Viewer()->PrivilegedPlane());

#ifdef OCC_PATCHED
	current_view_->Convert( x_screen, y_screen, Xv, Yv, Zv );
#else
	// The + 1 overcomes a fault in OCC, in "OpenGl_togl_unproject_raster.c",
	// which transforms the Y axis ordinate. The function uses the height of the
	// window, not the Y maximum which is (height - 1).
	current_view_->Convert( x_screen, y_screen + 1, Xv, Yv, Zv );
#endif

	current_view_->Proj( Vx, Vy, Vz );
	gp_Lin aLine(gp_Pnt(Xv, Yv, Zv), gp_Dir(Vx, Vy, Vz));
	IntAna_IntConicQuad theIntersection( aLine, aPlane, Precision::Angular() );
	if (theIntersection.IsDone())
	{
		if (!theIntersection.IsParallel())
		{
			if (theIntersection.NbPoints() > 0)
			{
				double x,y,z;

				gp_Pnt theSolution(theIntersection.Point(1));
			
				x = theSolution.X();
				y = theSolution.Y();
				z = theSolution.Z();
				
				parent_sketch_->GetSketchPlane()->GetSTLocation(x,y,z,current_s_,current_t_);

				primitive_finished_ = true;
			}
		}
	} else {
		primitive_finished_ = false;
	}
}