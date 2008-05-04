#ifndef OccAngleLine2DH
#define OccAngleLine2DH

#include "OccPrimitiveBase.h"

class OccAngleLine2D : public OccPrimitiveBase, public AngleLine2D
{
	public:
		OccAngleLine2D (Handle(AIS_InteractiveContext) ais_context,
																			 const Line2DPointer line1, const Line2DPointer line2, double angle);

		void Display() {return OccPrimitiveBase::Display();}
		bool IsSelected() {return OccPrimitiveBase::IsSelected();}
		void SetSelectable(bool input_selectable) {PrimitiveBase::SetSelectable(input_selectable); OccPrimitiveBase::SetSelectable(selectable_);}

		void UpdateDisplay();

		void GenerateAISObject();

	private:
		gp_Pnt oc_point1_;
		gp_Pnt oc_point2_;
		gp_Pnt oc_point3_;
		gp_Pnt oc_point4_;

		TopoDS_Edge oc_shape1_;
		TopoDS_Edge oc_shape2_;

 		Handle(Geom_Plane) oc_plane_;
};
typedef boost::shared_ptr<OccAngleLine2D> OccAngleLine2DPointer;



#endif //OccAngleLine2DH
