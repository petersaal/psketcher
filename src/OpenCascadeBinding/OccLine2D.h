#ifndef OccLine2DH
#define OccLine2DH

#include "OccPrimitiveBase.h"

// line class
class OccLine2D : public OccPrimitiveBase, public Line2D
{
	public:
		OccLine2D (Handle(AIS_InteractiveContext) ais_context, const Point2DPointer point1, const Point2DPointer point2, SketchPlanePointer sketch_plane);

		void Display() {return OccPrimitiveBase::Display();}
		bool IsSelected() {return OccPrimitiveBase::IsSelected();}
		void SetSelectable(bool input_selectable) {PrimitiveBase::SetSelectable(input_selectable); OccPrimitiveBase::SetSelectable(selectable_);}

		void UpdateDisplay();

		TopoDS_Shape GetTopoDS_Shape();

	private:
		Handle(Geom_CartesianPoint) oc_point1_;
		Handle(Geom_CartesianPoint) oc_point2_;
};
typedef boost::shared_ptr<OccLine2D> OccLine2DPointer;



#endif //OccLine2DH
