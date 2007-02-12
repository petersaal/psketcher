include(UsePkgConfig)

# The fllowing forces cmake to show the compiler command line arguements, usefull for debugging
SET (CMAKE_VERBOSE_MAKEFILE TRUE)

ADD_EXECUTABLE (ark3d Callbacks.cpp Interface.cpp OccGtkGLView.cpp Support.cpp Translate.cpp main.cpp)

# Add include paths and libraries for ark3d's dependencies
#OpenGL
FIND_PACKAGE (OpenGL REQUIRED)
TARGET_LINK_LIBRARIES (ark3d ${OPENGL_LIBRARIES})
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

# The following module is included so that the pkg_check_modules macro can be used below
find_package(PkgConfig)

#gtk+-2.0
pkg_check_modules(GTK gtk+-2.0)
TARGET_LINK_LIBRARIES (ark3d ${GTK_LIBRARIES})
LINK_DIRECTORIES (${GTK_LIBRARY_DIRS})
INCLUDE_DIRECTORIES (${GTK_INCLUDE_DIRS})

#Glade-2.0
pkg_check_modules(LIBGLADE libglade-2.0)
TARGET_LINK_LIBRARIES (ark3d ${LIBGLADE_LIBRARIES})
LINK_DIRECTORIES (${LIBGLADE_LIBRARY_DIRS})
INCLUDE_DIRECTORIES (${LIBGLADE_INCLUDE_DIRS})

#gmodule
pkg_check_modules(GMODULE gmodule-2.0)
TARGET_LINK_LIBRARIES (ark3d ${GMODULE_LIBRARIES})
LINK_DIRECTORIES (${GMODULE_LIBRARY_DIRS})
INCLUDE_DIRECTORIES (${GMODULE_INCLUDE_DIRS})

#gtkglext
pkg_check_modules(GTKGLEXT gtkglext-1.0)
TARGET_LINK_LIBRARIES (ark3d ${GTKGLEXT_LIBRARIES})
LINK_DIRECTORIES (${GTKGLEXT_LIBRARY_DIRS})
INCLUDE_DIRECTORIES (${GTKGLEXT_INCLUDE_DIRS})

#OpenCascade
TARGET_LINK_LIBRARIES (ark3d ${OPENCASCADE_LIBS})
LINK_DIRECTORIES (OPENCASCADE_LINK_DIR)
INCLUDE_DIRECTORIES (${CASROOT}/inc)
INCLUDE_DIRECTORIES (${CASROOT}/src/WOKTclLib)
