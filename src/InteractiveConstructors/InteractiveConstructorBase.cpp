#include "InteractiveConstructorBase.h"

InteractiveConstructorBase::InteractiveConstructorBase(OccSketchPointer parent_sketch, Handle(V3d_View) current_view, Handle(V3d_Viewer) current_viewer):
parent_sketch_(parent_sketch),
current_viewer_(current_viewer),
current_view_(current_view)
{

}
