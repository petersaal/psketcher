#ifndef OccPrimitivesH
#define OccPrimitivesH

#include "../ConstraintSolver/Primitives.h"
#include <AIS_InteractiveContext.hxx>
#include <AIS_InteractiveObject.hxx>
#include <AIS_Point.hxx>
#include <AIS_Line.hxx>
#include <AIS_ParallelRelation.hxx>
#include <AIS_LengthDimension.hxx>
#include <Geom_CartesianPoint.hxx>
#include <Geom_Plane.hxx>
#include <gp_Pnt.hxx>

#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Shape.hxx>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>

// OccPrimitiveBase class
class OccPrimitiveBase
{
	public:
		OccPrimitiveBase(Handle(AIS_InteractiveContext) ais_context) {ais_context_ = ais_context;}

		virtual ~OccPrimitiveBase() { /*Erase();*/  /*@TODO need to uncomment Erase(); so that AIS interactive objects are not displayed after this oject no longer exits*/ }

		virtual void Display();

		void Erase();

		// update coordinates of primitive
		virtual void UpdateDisplay() {ais_object_->Redisplay();}

	protected:
		Handle(AIS_InteractiveContext) ais_context_;
		Handle(AIS_InteractiveObject) ais_object_;
};

// point class
class OccPoint : public OccPrimitiveBase, public Point
{
	public:
		OccPoint (Handle(AIS_InteractiveContext) ais_context, double x, double y, double z, bool x_free = false, bool y_free = false, bool z_free = false);

		void Display() {return OccPrimitiveBase::Display();}

		void UpdateDisplay();
	private:

		Handle(Geom_CartesianPoint) oc_point_;
};
typedef boost::shared_ptr<OccPoint> OccPointPointer;

// point class
class OccPoint2D : public OccPrimitiveBase, public Point2D
{
	public:
		OccPoint2D (Handle(AIS_InteractiveContext) ais_context,double s, double t, SketchPlanePointer sketch_plane, bool s_free = false, bool t_free = false);

		void Display() {return OccPrimitiveBase::Display();}

		void UpdateDisplay();

	private:
		Handle(Geom_CartesianPoint) oc_point_;

};
typedef boost::shared_ptr<OccPoint2D> OccPoint2DPointer;

// line class
class OccLine : public OccPrimitiveBase, public Line
{
	public:
		OccLine (Handle(AIS_InteractiveContext) ais_context, const PointPointer point1, const PointPointer point2);

		void Display() {return OccPrimitiveBase::Display();}

		void UpdateDisplay();

	private:
		Handle(Geom_CartesianPoint) oc_point1_;
		Handle(Geom_CartesianPoint) oc_point2_;
};
typedef boost::shared_ptr<OccLine> OccLinePointer;

// line class
class OccLine2D : public OccPrimitiveBase, public Line2D
{
	public:
		OccLine2D (Handle(AIS_InteractiveContext) ais_context, const Point2DPointer point1, const Point2DPointer point2, SketchPlanePointer sketch_plane);

		void Display() {return OccPrimitiveBase::Display();}

		void UpdateDisplay();

	private:
		Handle(Geom_CartesianPoint) oc_point1_;
		Handle(Geom_CartesianPoint) oc_point2_;
};
typedef boost::shared_ptr<OccLine2D> OccLine2DPointer;

class OccParallelConstraintLine2DLine2D : public OccPrimitiveBase, public ParallelConstraintLine2DLine2D
{
	public:
		OccParallelConstraintLine2DLine2D (Handle(AIS_InteractiveContext) ais_context,
																			 const Line2DPointer line1, const Line2DPointer line2);

		void Display() {return OccPrimitiveBase::Display();}

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
typedef boost::shared_ptr<OccParallelConstraintLine2DLine2D> OccParallelConstraintLine2DLine2DPointer;

class OccDistanceConstraintPoint2DPoint2D : public OccPrimitiveBase, public DistanceConstraintPoint2DPoint2D
{
	public:
		OccDistanceConstraintPoint2DPoint2D (Handle(AIS_InteractiveContext) ais_context,
																			   const Point2DPointer point1, const Point2DPointer point2, 
																				 double distance);

		void Display() {return OccPrimitiveBase::Display();}

		void UpdateDisplay();

		void GenerateAISObject();

	private:
		gp_Pnt oc_point1_;
		gp_Pnt oc_point2_;

		TopoDS_Vertex oc_shape1_;
		TopoDS_Vertex oc_shape2_;

 		Handle(Geom_Plane) oc_plane_;
};
typedef boost::shared_ptr<OccDistanceConstraintPoint2DPoint2D> OccDistanceConstraintPoint2DPoint2DPointer;
#endif //OccPrimitivesH
