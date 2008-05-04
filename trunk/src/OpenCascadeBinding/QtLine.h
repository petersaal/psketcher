#ifndef OccLineH
#define OccLineH

#include "OccPrimitiveBase.h"

// line class
class OccLine : public OccPrimitiveBase, public Line
{
	public:
		OccLine (Handle(AIS_InteractiveContext) ais_context, const PointPointer point1, const PointPointer point2);

		void Display() {return OccPrimitiveBase::Display();}
		bool IsSelected() {return OccPrimitiveBase::IsSelected();}
		void SetSelectable(bool input_selectable) {PrimitiveBase::SetSelectable(input_selectable); OccPrimitiveBase::SetSelectable(selectable_);}

		void UpdateDisplay();

	private:
		Handle(Geom_CartesianPoint) oc_point1_;
		Handle(Geom_CartesianPoint) oc_point2_;
};
typedef boost::shared_ptr<OccLine> OccLinePointer;



#endif //OccLineH
