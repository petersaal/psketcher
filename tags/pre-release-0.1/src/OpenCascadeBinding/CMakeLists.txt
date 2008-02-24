#OpenCascade
INCLUDE_DIRECTORIES (${CASROOT}/inc)
INCLUDE_DIRECTORIES (${CASROOT}/src/WOKTclLib)

ADD_LIBRARY (Ark3dOpenCascadeBinding STATIC OccPrimitives.cpp OccSketch.cpp)
