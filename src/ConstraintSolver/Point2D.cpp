
#include "IndependentDOF.h"
#include "Point2D.h"


using namespace std;
using namespace GiNaC;

// Return the global coordinates for a Point2D object
void Point2D::Get3DLocation(double & x_location,double & y_location, double & z_location)const
{
	sketch_plane_->Get3DLocation(s_->GetValue(), t_->GetValue(), x_location, y_location, z_location);
}


Point2D :: Point2D ( double s, double t, SketchPlanePointer sketch_plane, bool s_free, bool t_free):
s_(new IndependentDOF(s,s_free)),
t_(new IndependentDOF(t,t_free)),
Primitive2DBase(sketch_plane)
{
	dof_list_.push_back(s_);
	dof_list_.push_back(t_);
}

Point2D :: Point2D ( DOFPointer s, DOFPointer t, SketchPlanePointer sketch_plane):
s_(s),
t_(t),
Primitive2DBase(sketch_plane)
{
	dof_list_.push_back(s_);
	dof_list_.push_back(t_);
}

void Point2D::ApplySelectionMask(SelectionMask mask)
{
	if(mask == All || mask == Points)
		SetSelectable(true);
	else
		SetSelectable(false);
}

mmcMatrix Point2D::GetmmcMatrix()const
{
	mmcMatrix result(2,1);
	result(0,0) = s_->GetValue();
	result(1,0) = t_->GetValue();

	return result;
}	