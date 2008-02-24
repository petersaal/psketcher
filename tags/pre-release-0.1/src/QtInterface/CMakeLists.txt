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
set(qtocc_RCCS
    qocc.qrc
)
qt4_wrap_cpp(qtocc_MOCS ${qtocc_MOC_SRCS})
qt4_add_resources(qtocc_RCC_SRCS ${qtocc_RCCS})
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
    ${qtocc_MOCS}
    ${qtocc_RCC_SRCS})

TARGET_LINK_LIBRARIES(qtocc ${OPENGL_LIBRARIES})
TARGET_LINK_LIBRARIES(qtocc ${GLUT_Xmu_LIBRARY})
target_link_libraries(qtocc ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(qtocc ${OPENCASCADE_LIBS}) 
