#include "Primitives.h"

using namespace std;

DOF :: DOF(double value, bool free)
{
	free_ = free;
	value_ = value;

}

DOF :: DOF(const char *name, double value, bool free)
{
	free_ = free;
	variable_.set_name(name);
	value_ = value;
}


Point :: Point (double x,double y,double z):
x_(new DOF(x,false)),
y_(new DOF(y,false)),
z_(new DOF(z,false))
{

}

Line :: Line(PointPointer point1, PointPointer point2)
{
	x1_ = point1->GetXDOF();
	y1_ = point1->GetYDOF();
	z1_ = point1->GetZDOF();

	x2_ = point2->GetXDOF();
	y2_ = point2->GetYDOF();
	z2_ = point2->GetZDOF();
}

