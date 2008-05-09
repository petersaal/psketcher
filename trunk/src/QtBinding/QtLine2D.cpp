#include "QtLine2D.h"


QtLine2D::QtLine2D (QGraphicsItem * parent, const Point2DPointer point1, const Point2DPointer point2, SketchPlanePointer sketch_plane) :
QtPrimitiveBase(parent),
Line2D(point1, point2, sketch_plane)
{
	double x1, y1, z1, x2, y2, z2;	

	Get3DLocations(x1, y1, z1, x2, y2, z2);

	// Display the newly create ais_object
	Display();
}

void QtLine2D::UpdateDisplay()
{
	double x1, y1, z1, x2, y2, z2;	

	Get3DLocations(x1, y1, z1, x2, y2, z2);


	QtPrimitiveBase::UpdateDisplay();
}
