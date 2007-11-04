PROJECT (ark3d)

set (ARK3D_VERSION 0.1)
ADD_DEFINITIONS (-DVERSION="\\"${ARK3D_VERSION}\\"")

# Some of the opencascade headers require the following compiler defines
ADD_DEFINITIONS (-DHAVE_IOSTREAM -DHAVE_LIMITS_H -DHAVE_FSTREAM -DCSFDB -DHAVE_GETTEXT -DENABLE_NLS)

# FindPkcConfig is in the following directory since it is not part of the
# official cmake release yet (it's in CVS)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_scripts)

# Define variables specific to the local OpenCascade Installation
SET (CASROOT $ENV{CASROOT})
SET (OPENCASCADE_LINK_DIR "/opt/OpenCASCADE-6.2/lib")

# List the Opencascade libraries that need to be linked to ark3d
SET (OPENCASCADE_LIBS "-lTKernel -lTKMath -lTKG2d -lTKG3d -lTKGeomBase -lTKBRep -lTKGeomAlgo -lTKTopAlgo -lTKPrim -lTKBool -lTKFeat -lTKFillet -lTKOffset -lTKHLR -lTKService -lTKV2d -lTKV3d -lTKMesh -lTKPCAF -lTKLCAF -lTKPLCAF -lTKCDF -lTKCAF -lPTKernel -lTKIGES -lTKSTEP -lTKSTEPBase -lTKSTEPAttr -lTKSTEP209 -lTKSTL -lTKVRML -lTKShHealing -lTKXSBase -lTKPShape -lTKShapeSchema -lTKOpenGl")

add_subdirectory (src/Interface)
add_subdirectory (src/NumOptimization)
add_subdirectory (src/mmcMatrix)
add_subdirectory (src/ConstraintSolver)
add_subdirectory (src/OpenCascadeBinding)
add_subdirectory (src/PythonBinding)
add_subdirectory (src/InteractiveConstructors)

