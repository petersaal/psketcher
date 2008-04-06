#OpenCascade
INCLUDE_DIRECTORIES (${CASROOT}/inc)
INCLUDE_DIRECTORIES (${CASROOT}/src/WOKTclLib)

ADD_LIBRARY (Ark3dOpenCascadeBinding STATIC OccPrimitiveBase.cpp OccPoint.cpp OccPoint2D.cpp OccLine.cpp OccLine2D.cpp OccArc2D.cpp OccParallelLine2D.cpp OccDistancePoint2D.cpp OccAngleLine2D.cpp OccTangentEdge2D.cpp OccSketch.cpp)
