PROJECT (qtocc)

# Some of the opencascade headers require the following compiler defines
ADD_DEFINITIONS (-DHAVE_IOSTREAM -DHAVE_LIMITS_H -DHAVE_FSTREAM -DCSFDB -DHAVE_GETTEXT -DENABLE_NLS)

# Define variables specific to the local OpenCascade Installation
SET (CASROOT $ENV{CASROOT})
SET (OPENCASCADE_LINK_DIR "/opt/OpenCASCADE-6.2/lib")

# List the Opencascade libraries that need to be linked to ark3d
SET (OPENCASCADE_LIBS "-lTKernel -lTKMath -lTKG2d -lTKG3d -lTKGeomBase -lTKBRep -lTKGeomAlgo -lTKTopAlgo -lTKPrim -lTKBool -lTKFeat -lTKFillet -lTKOffset -lTKHLR -lTKService -lTKV2d -lTKV3d -lTKMesh -lTKPCAF -lTKLCAF -lTKPLCAF -lTKCDF -lTKCAF -lPTKernel -lTKIGES -lTKSTEP -lTKSTEPBase -lTKSTEPAttr -lTKSTEP209 -lTKSTL -lTKVRML -lTKShHealing -lTKXSBase -lTKPShape -lTKShapeSchema -lTKOpenGl")

# Evering before this line can be removed once merged with Ark3D
# The fllowing forces cmake to show the compiler command line arguements, usefull for debugging
SET (CMAKE_VERBOSE_MAKEFILE TRUE)


find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
set(qtocc_SRCS
    main.cpp
    qoccapplication.cpp
    qoccdocument.cpp
    qoccharnesswindow.cpp
    qoccinputoutput.cpp
    qoccinternal.h.cpp
    qoccmakebottle.cpp
    qoccviewercontext.cpp
    qoccviewwidget.cpp
)
set(qtocc_MOC_SRCS
    ./qoccapplication.h
    ./qoccdocument.h
    ./qoccharnesswindow.h
    ./qoccviewwidget.h
    ./qoccinputoutput.h
    ./qoccviewercontext.h
)
qt4_wrap_cpp(qtocc_MOCS ${qtocc_MOC_SRCS})
add_definitions(-DQT_NO_DEBUG)

#OpenGL
FIND_PACKAGE (OpenGL REQUIRED)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

#Glut
FIND_PACKAGE (GLUT REQUIRED)

#OpenCascade
LINK_DIRECTORIES (${OPENCASCADE_LINK_DIR})
INCLUDE_DIRECTORIES (${CASROOT}/inc)
INCLUDE_DIRECTORIES (${CASROOT}/src/WOKTclLib)

add_executable(qtocc
    ${qtocc_SRCS}
    ${qtocc_MOCS})

TARGET_LINK_LIBRARIES(qtocc ${OPENGL_LIBRARIES})
TARGET_LINK_LIBRARIES(qtocc ${GLUT_Xmu_LIBRARY})
target_link_libraries(qtocc ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(qtocc ${OPENCASCADE_LIBS}) 
