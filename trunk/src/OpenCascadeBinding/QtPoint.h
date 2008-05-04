#ifndef OccPointH
#define OccPointH

#include "OccPrimitiveBase.h"

// point class
class OccPoint : public OccPrimitiveBase, public Point
{
	public:
		OccPoint (Handle(AIS_InteractiveContext) ais_context, double x, double y, double z, bool x_free = false, bool y_free = false, bool z_free = false);

		void Display() {return OccPrimitiveBase::Display();}
		bool IsSelected() {return OccPrimitiveBase::IsSelected();}
		void SetSelectable(bool input_selectable) {PrimitiveBase::SetSelectable(input_selectable); OccPrimitiveBase::SetSelectable(selectable_);}

		void UpdateDisplay();
	private:

		Handle(Geom_CartesianPoint) oc_point_;
};
typedef boost::shared_ptr<OccPoint> OccPointPointer;


#endif //OccPointH
