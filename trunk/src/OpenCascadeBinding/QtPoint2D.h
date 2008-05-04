#ifndef OccPoint2DH
#define OccPoint2DH

#include "OccPrimitiveBase.h"

// point class
class OccPoint2D : public OccPrimitiveBase, public Point2D
{
	public:
		OccPoint2D (Handle(AIS_InteractiveContext) ais_context,double s, double t, SketchPlanePointer sketch_plane, bool s_free = false, bool t_free = false);
		OccPoint2D (Handle(AIS_InteractiveContext) ais_context, DOFPointer s, DOFPointer t, SketchPlanePointer sketch_plane);

		void Display() {return OccPrimitiveBase::Display();}
		bool IsSelected() {return OccPrimitiveBase::IsSelected();}
		void SetSelectable(bool input_selectable) {PrimitiveBase::SetSelectable(input_selectable); OccPrimitiveBase::SetSelectable(selectable_);}
		void UpdateDisplay();

	private:
		Handle(Geom_CartesianPoint) oc_point_;

};
typedef boost::shared_ptr<OccPoint2D> OccPoint2DPointer;


#endif //OccPoint2DH
