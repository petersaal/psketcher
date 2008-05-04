#ifndef OccTangentEdge2DH
#define OccTangentEdge2DH

#include "OccPrimitiveBase.h"

class OccTangentEdge2D : public OccPrimitiveBase, public TangentEdge2D
{
	public:
		OccTangentEdge2D (Handle(AIS_InteractiveContext) ais_context,
                       Edge2DBasePointer edge1, EdgePointNumber point_num_1, 
                       Edge2DBasePointer edge2, EdgePointNumber point_num_2);

		void Display() {return OccPrimitiveBase::Display();}
		bool IsSelected() {return OccPrimitiveBase::IsSelected();}
		void SetSelectable(bool input_selectable) {PrimitiveBase::SetSelectable(input_selectable); OccPrimitiveBase::SetSelectable(selectable_);}

		void UpdateDisplay();

		void GenerateAISObject();

	private:
		TopoDS_Shape oc_shape1_;
		TopoDS_Shape oc_shape2_;

 		Handle(Geom_Plane) oc_plane_;
};
typedef boost::shared_ptr<OccTangentEdge2D> OccTangentEdge2DPointer;


#endif //OccTangentEdge2DH
