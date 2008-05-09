#include "InteractiveConstructorBase.h"

#include <IntAna_IntConicQuad.hxx>

InteractiveConstructorBase::InteractiveConstructorBase(QtSketchPointer parent_sketch, Handle(V3d_View) current_view, Handle(V3d_Viewer) current_viewer):
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

	// @fixme implement GetGlobalLocation(...) method

	return result;
}
