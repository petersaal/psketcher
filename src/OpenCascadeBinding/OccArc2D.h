#ifndef OccArc2DH
#define OccArc2DH

#include "OccPrimitiveBase.h"

// line class
class OccArc2D : public OccPrimitiveBase, public Arc2D
{
	public:
		OccArc2D (Handle(AIS_InteractiveContext) ais_context, double s_center, double t_center, double theta_1, double theta_2, double radius, 
			      SketchPlanePointer sketch_plane, bool s_center_free = false, bool t_center_free = false, bool theta_1_free = false, bool theta_2_free = false, 
			      bool radius_free = false);
		OccArc2D (Handle(AIS_InteractiveContext) ais_context,DOFPointer s_center, DOFPointer t_center, DOFPointer theta_1, DOFPointer theta_2, DOFPointer radius, SketchPlanePointer sketch_plane);

		void Display() {return OccPrimitiveBase::Display();}
		bool IsSelected() {return OccPrimitiveBase::IsSelected();}
		void SetSelectable(bool input_selectable) {PrimitiveBase::SetSelectable(input_selectable); OccPrimitiveBase::SetSelectable(selectable_);}

		void UpdateDisplay();

		void GenerateAISObject();

		TopoDS_Shape GetTopoDS_Shape();

	private:

};
typedef boost::shared_ptr<OccArc2D> OccArc2DPointer;



#endif //OccArc2DH
