#include "OccTangentEdge2D.h"

#include "OccLine2D.h"
#include "OccArc2D.h"

OccTangentEdge2D::OccTangentEdge2D (Handle(AIS_InteractiveContext) ais_context,
                       Edge2DBasePointer edge1, EdgePointNumber point_num_1, 
                       Edge2DBasePointer edge2, EdgePointNumber point_num_2):
OccPrimitiveBase(ais_context),
TangentEdge2D(edge1,point_num_1,edge2,point_num_2)
{
	GenerateAISObject();

	// Display the newly create ais_object
	Display();
}


void OccTangentEdge2D::UpdateDisplay()
{
	// first, erase the pervious AIS_ParallelRelation from the display because we'll have to recreate it
	Erase();
	
	GenerateAISObject();

	Display();

	OccPrimitiveBase::UpdateDisplay();
}

void OccTangentEdge2D::GenerateAISObject()
{
	// First, create opencascade versions of each of the two edges
	// the edges can be either a line or an arc
	// this will need to be updated as more edge types are introduced
	
	Edge2DBasePointer current_edge = edge1_;
	bool done = false;
	TopoDS_Shape current_shape;
	int loop_count = 0;
	while(!done)
	{
		if(dynamic_cast<Line2D*>(current_edge.get()) != 0){
			Line2DPointer current_line = boost::dynamic_pointer_cast<Line2D>(current_edge);
			OccLine2DPointer occ_line(new OccLine2D(ais_context_,current_line->GetPoint1(),current_line->GetPoint2(),current_line->GetSketchPlane()));
			current_shape = occ_line->GetTopoDS_Shape();
		}else if(dynamic_cast<Arc2D*>(current_edge.get()) != 0){
			Arc2DPointer current_arc = boost::dynamic_pointer_cast<Arc2D>(current_edge);

			OccArc2DPointer occ_arc(new OccArc2D(ais_context_,current_arc->GetSCenter(),current_arc->GetTCenter(),current_arc->GetTheta1(),current_arc->GetTheta2(),current_arc->GetRadius(),current_arc->GetSketchPlane()));

			current_shape = occ_arc->GetTopoDS_Shape();
		}else {
			throw PrimitiveException();
		}
			
		loop_count++;
	
		if(loop_count == 2)
		{
			done = true;
			oc_shape2_ = current_shape;
		} else {
			oc_shape1_ = current_shape;
			current_edge = edge2_;
		}
	}


	if(oc_shape1_.IsNull() || oc_shape2_.IsNull())
		throw PrimitiveException();

	// define the plane that is needed by the AIS_parallelRelation object
	double coef_a, coef_b, coef_c, coef_d;
	edge1_->GetSketchPlane()->GetABCD(coef_a,coef_b,coef_c,coef_d);
	oc_plane_ = new Geom_Plane(coef_a, coef_b, coef_c, coef_d);

	// create the interactive ais_object, this is what will actually be displayed
	ais_object_list_.push_back(new AIS_TangentRelation(oc_shape1_,oc_shape2_,oc_plane_));
}