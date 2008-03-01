# The fllowing forces cmake to show the compiler command line arguements, usefull for debugging
SET (CMAKE_VERBOSE_MAKEFILE TRUE)

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
set(ark3d_SRCS
    main.cpp
    qoccapplication.cpp
    qoccdocument.cpp
    Ark3DWindow.cpp
    qoccinputoutput.cpp
    qoccinternal.h.cpp
    qoccmakebottle.cpp
    qoccviewercontext.cpp
    qoccviewwidget.cpp
)
set(ark3d_MOC_SRCS
    ./qoccapplication.h
    ./qoccdocument.h
    ./Ark3DWindow.h
    ./qoccviewwidget.h
    ./qoccinputoutput.h
    ./qoccviewercontext.h
)
set(ark3d_RCCS
    qocc.qrc
)
qt4_wrap_cpp(ark3d_MOCS ${ark3d_MOC_SRCS})
qt4_add_resources(ark3d_RCC_SRCS ${ark3d_RCCS})
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

add_executable(ark3d
    ${ark3d_SRCS}
    ${ark3d_MOCS}
    ${ark3d_RCC_SRCS})

TARGET_LINK_LIBRARIES(ark3d ${OPENGL_LIBRARIES})
TARGET_LINK_LIBRARIES(ark3d ${GLUT_Xmu_LIBRARY})
target_link_libraries(ark3d ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(ark3d ${OPENCASCADE_LIBS}) 
