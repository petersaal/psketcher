#OpenCascade
INCLUDE_DIRECTORIES (${CASROOT}/inc)
INCLUDE_DIRECTORIES (${CASROOT}/src/WOKTclLib)

ADD_LIBRARY (Ark3dQtBinding STATIC QtPrimitiveBase.cpp QtPoint.cpp QtPoint2D.cpp QtLine.cpp QtLine2D.cpp QtArc2D.cpp QtParallelLine2D.cpp QtDistancePoint2D.cpp QtAngleLine2D.cpp QtTangentEdge2D.cpp QtSketch.cpp)
