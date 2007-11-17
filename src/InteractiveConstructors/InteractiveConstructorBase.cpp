#include "InteractiveConstructorBase.h"

#include <IntAna_IntConicQuad.hxx>

InteractiveConstructorBase::InteractiveConstructorBase(OccSketchPointer parent_sketch, Handle(V3d_View) current_view, Handle(V3d_Viewer) current_viewer):
parent_sketch_(parent_sketch),
current_viewer_(current_viewer),
current_view_(current_view)
{

}


// Get global coordinates from x,y mouse coordinates (projects mouse click to current active sketch plane)
// returns true if projection was successful, otherwise returns false
bool InteractiveConstructorBase::GetGlobalLocation(int x_screen, int y_screen, double &x, double &y, double &z, bool grid_snap)
{
	bool result = false;

	double Xv, Yv, Zv;
	double Vx, Vy, Vz;
	gp_Pln aPlane(current_view_->Viewer()->PrivilegedPlane());

#ifdef OCC_PATCHED
	if(grid_snap)
		current_view_->ConvertToGrid( x_screen, y_screen, Xv, Yv, Zv );
	else
		current_view_->Convert( x_screen, y_screen, Xv, Yv, Zv );
#else
	// The + 1 overcomes a fault in OCC, in "OpenGl_togl_unproject_raster.c",
	// which transforms the Y axis ordinate. The function uses the height of the
	// window, not the Y maximum which is (height - 1).
	if(grid_snap)
		current_view_->ConvertToGrid( x_screen, y_screen + 1, Xv, Yv, Zv );
	else
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
				gp_Pnt theSolution(theIntersection.Point(1));
			
				x = theSolution.X();
				y = theSolution.Y();
				z = theSolution.Z();

				result = true;
			}
		}
	}

	return result;
}
