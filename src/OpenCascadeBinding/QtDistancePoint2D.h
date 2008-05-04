#ifndef OccDistancePoint2DH
#define OccDistancePoint2DH

#include "OccPrimitiveBase.h"

class OccDistancePoint2D : public OccPrimitiveBase, public DistancePoint2D
{
	public:
		OccDistancePoint2D (Handle(AIS_InteractiveContext) ais_context,
																			   const Point2DPointer point1, const Point2DPointer point2, 
																				 double distance);

		void Display() {return OccPrimitiveBase::Display();}
		bool IsSelected() {return OccPrimitiveBase::IsSelected();}
		void SetSelectable(bool input_selectable) {PrimitiveBase::SetSelectable(input_selectable); OccPrimitiveBase::SetSelectable(selectable_);}

		void UpdateDisplay();

		void GenerateAISObject();

	private:
		gp_Pnt oc_point1_;
		gp_Pnt oc_point2_;

		TopoDS_Vertex oc_shape1_;
		TopoDS_Vertex oc_shape2_;

 		Handle(Geom_Plane) oc_plane_;
};
typedef boost::shared_ptr<OccDistancePoint2D> OccDistancePoint2DPointer;



#endif //OccDistancePoint2DH
