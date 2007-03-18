#ifndef OcPrimitivesH
#define OcPrimitivesH

#include "../ConstraintSolver/Primitives.h"
#include <AIS_InteractiveObject.hxx>

// OcPrimitiveBase class
class OcPrimitiveBase
{
	public:

	private:
		Handle(AIS_InteractiveContext) ais_context_;
		Handle(AIS_InteractiveObject) ais_object_;
}

// point class
class OcPoint : public OcPrimitiveBase
{
	public:
		OcPoint (const PointPointer point, Handle(AIS_InteractiveContext) ais_context);

	private:
		PointPointer point_;

};
typedef boost::shared_ptr<OcPoint> OcPointPointer;

// line class
class OcLine : public OcPrimitiveBase
{
	public:
		OcLine (const LinePointer line, Handle(AIS_InteractiveContext) ais_context);

	private:
		LinePointer line_;

};
typedef boost::shared_ptr<OcLine> OcLinePointer;

#endif //OcPrimitivesH