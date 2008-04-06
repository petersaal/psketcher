#ifndef OccParallelLine2DH
#define OccParallelLine2DH

#include "OccPrimitiveBase.h"

class OccParallelLine2D : public OccPrimitiveBase, public ParallelLine2D
{
	public:
		OccParallelLine2D (Handle(AIS_InteractiveContext) ais_context,
																			 const Line2DPointer line1, const Line2DPointer line2);

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
typedef boost::shared_ptr<OccParallelLine2D> OccParallelLine2DPointer;


#endif //OccParallelLine2DH
