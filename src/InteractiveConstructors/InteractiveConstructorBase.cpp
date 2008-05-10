#include "InteractiveConstructorBase.h"

InteractiveConstructorBase::InteractiveConstructorBase(QtSketchPointer parent_sketch):
parent_sketch_(parent_sketch)
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
