# The fllowing forces cmake to show the compiler command line arguements, usefull for debugging
SET (CMAKE_VERBOSE_MAKEFILE TRUE)

ADD_DEFINITIONS (-DPACKAGE="\\"ark3d\\"" -DGETTEXT_PACKAGE="\\"ark3d\\""
-DPACKAGE_DATA_DIR="\\"./res\\"" -DPACKAGE_LOCALE_DIR="\\"/home/grem/opt/ark3d/share/locale\\"")

# Add local link paths
LINK_DIRECTORIES ("../mmcMatrix")
LINK_DIRECTORIES ("../NumOptimization")
LINK_DIRECTORIES ("../ConstraintSolver")
LINK_DIRECTORIES ("../OpenCascadeBinding")
LINK_DIRECTORIES ("../LuaBinding")

# Add include paths and libraries for ark3d's dependencies
#OpenGL
FIND_PACKAGE (OpenGL REQUIRED)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

# The following module is included so that the pkg_check_modules macro can be used below
find_package(PkgConfig)

#gtk+-2.0
pkg_check_modules(GTK gtk+-2.0)
LINK_DIRECTORIES (${GTK_LIBRARY_DIRS})
INCLUDE_DIRECTORIES (${GTK_INCLUDE_DIRS})

#Glade-2.0
pkg_check_modules(LIBGLADE libglade-2.0)
LINK_DIRECTORIES (${LIBGLADE_LIBRARY_DIRS})
INCLUDE_DIRECTORIES (${LIBGLADE_INCLUDE_DIRS})

#gmodule
pkg_check_modules(GMODULE gmodule-2.0)
LINK_DIRECTORIES (${GMODULE_LIBRARY_DIRS})
INCLUDE_DIRECTORIES (${GMODULE_INCLUDE_DIRS})

#gtkglext
pkg_check_modules(GTKGLEXT gtkglext-1.0)
LINK_DIRECTORIES (${GTKGLEXT_LIBRARY_DIRS})
INCLUDE_DIRECTORIES (${GTKGLEXT_INCLUDE_DIRS})

#OpenCascade
LINK_DIRECTORIES (${OPENCASCADE_LINK_DIR})
INCLUDE_DIRECTORIES (${CASROOT}/inc)
INCLUDE_DIRECTORIES (${CASROOT}/src/WOKTclLib)

#gsl
pkg_check_modules(GSL gsl)
LINK_DIRECTORIES (${GSL_LIBRARY_DIRS})
INCLUDE_DIRECTORIES (${GSL_INCLUDE_DIRS})

#ginac
pkg_check_modules(GINAC ginac)
LINK_DIRECTORIES (${GINAC_LIBRARY_DIRS})
INCLUDE_DIRECTORIES (${GINAC_INCLUDE_DIRS})

#lualib50
pkg_check_modules(LUALIB lualib50)
LINK_DIRECTORIES (${LUALIB_LIBRARY_DIRS})
INCLUDE_DIRECTORIES (${LUALIB_INCLUDE_DIRS})

ADD_EXECUTABLE (ark3d Callbacks.cpp Interface.cpp OccGtkGLView.cpp Support.cpp Translate.cpp MakeBottle.cpp main.cpp ../OpenCascadeBinding/test.cpp)
TARGET_LINK_LIBRARIES (ark3d ${OPENGL_LIBRARIES})
TARGET_LINK_LIBRARIES (ark3d ${GTK_LIBRARIES})
TARGET_LINK_LIBRARIES (ark3d ${LIBGLADE_LIBRARIES})
TARGET_LINK_LIBRARIES (ark3d ${GMODULE_LIBRARIES})
TARGET_LINK_LIBRARIES (ark3d ${GTKGLEXT_LIBRARIES})
TARGET_LINK_LIBRARIES (ark3d ${OPENCASCADE_LIBS})
TARGET_LINK_LIBRARIES (ark3d Ark3d)
TARGET_LINK_LIBRARIES (ark3d Ark3dOpenCascadeBinding)
TARGET_LINK_LIBRARIES (ark3d mmcMatrix)
TARGET_LINK_LIBRARIES (ark3d ${GSL_LIBRARIES})
TARGET_LINK_LIBRARIES (ark3d ${GINAC_LIBRARIES})
TARGET_LINK_LIBRARIES (ark3d bfgs)
TARGET_LINK_LIBRARIES (ark3d ${LUALIB_LIBRARIES})
TARGET_LINK_LIBRARIES (ark3d Ark3dLuaModule)


